#include "CPSViewI.h"
#include <CFuncs.h>
#include <CLineList2D.h>
#include <CTriangle2D.h>
#include <CFreeType.h>
#include <CArcToBezier.h>
#include <CBezierToLine.h>
#include <algorithm>

class PSViewPathCharRenderer : public CFreeTypePathRenderer {
 public:
  PSViewPathCharRenderer(PSViewPath *path, const CMatrix2D &m, const CPoint2D &p) :
   path_(path), m_(m), p_(p), scale_(1.0) {
    CFontPtr font = CFreeTypeMgrInst->getFont();

    scale_ = 1.0/font->getSize();
  }

  void init() {
    path_->init();
  }

  void moveTo(const CPoint2D &p) {
    CPoint2D tp = adjustPoint(p);

    path_->moveTo(tp);
  }

  void lineTo(const CPoint2D &p) {
    CPoint2D tp = adjustPoint(p);

    path_->lineTo(tp);
  }

  void bezier2To(const CPoint2D &p1, const CPoint2D &p2) {
    CPoint2D tp1 = adjustPoint(p1);
    CPoint2D tp2 = adjustPoint(p2);

    path_->bezier2To(tp1, tp2);
  }

  void bezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) {
    CPoint2D tp1 = adjustPoint(p1);
    CPoint2D tp2 = adjustPoint(p2);
    CPoint2D tp3 = adjustPoint(p3);

    path_->bezier3To(tp1, tp2, tp3);
  }

  void close() {
    path_->close();
  }

  void stroke() { }

  void fill() { }

  CPoint2D adjustPoint(const CPoint2D &p) {
    CPoint2D tp;

    m_.multiplyPoint(p*scale_, tp);

    return p_ + tp;
  }

 private:
  PSViewPath      *path_;
  const CMatrix2D &m_;
  CPoint2D         p_;
  double           scale_;

 private:
  PSViewPathCharRenderer(PSViewPathCharRenderer &rhs);
  PSViewPathCharRenderer &operator=(PSViewPathCharRenderer &rhs);
};

class PSViewPathBBoxVisitor : public PSViewPathVisitor {
 public:
  PSViewPathBBoxVisitor() :
   bbox_() {
  }

 ~PSViewPathBBoxVisitor() { }

  void moveTo(const CPoint2D &p) {
    bbox_ += p;
  }

  void lineTo(const CPoint2D &p) {
    bbox_ += p;
  }

  void bezier2To(const CPoint2D &p1, const CPoint2D &p2) {
    bbox_ += p1;
    bbox_ += p2;
  }

  void bezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) {
    bbox_ += p1;
    bbox_ += p2;
    bbox_ += p3;
  }

  void arcTo(const CPoint2D &c, double xr, double yr, double theta, double delta) {
    vector<C3Bezier2D> beziers;

    CArcToBezier::ArcToBeziers(c.x, c.y, xr, yr, theta, theta + delta, beziers);

    uint num_beziers = beziers.size();

    for (uint i = 0; i < num_beziers; ++i) {
      const C3Bezier2D &bezier = beziers[i];

      bezier3To(bezier.getControlPoint1(), bezier.getControlPoint2(), bezier.getLastPoint());
    }

#if 0
    CMathGeom2D::EllipsePointAtAngle(c.x, c.y, xr, yr, theta        , &x1, &y1);
    CMathGeom2D::EllipsePointAtAngle(c.x, c.y, xr, yr, theta + delta, &x2, &y2);

    bbox_ += CPoint2D(x1, y1);
    bbox_ += CPoint2D(x2, y2);

    double a1 = CMathGen::RadToDeg(theta);
    double a2 = CMathGen::RadToDeg(theta + delta);

    if (a1 < 0) a1 += 360; if (a1 > 360) a1 -= 360;
    if (a2 < 0) a2 += 360; if (a2 > 360) a2 -= 360;

    double da = 0.0;

    if      (a2 - a1 > 270) da = (a2 - a1)/8;
    else if (a2 - a1 > 180) da = (a2 - a1)/6;
    else if (a2 - a1 >  90) da = (a2 - a1)/4;
    else                    da = (a2 - a1)/2;

    for (double a = a1 + da; a < a2; a += da) {
      double x, y;

      CMathGeom2D::EllipsePointAtAngle(c.x, c.y, xr, yr, CMathGen::DegToRad(a), &x, &y);

      bbox_ += CPoint2D(x, y);
    }
#endif
  }

  void close() {
  }

  const CBBox2D &getBBox() const { return bbox_; }

 private:
  CBBox2D bbox_;
};

class PSViewPathFlattenVisitor : public PSViewPathVisitor {
 public:
  PSViewPathFlattenVisitor() :
   flatPath_() {
  }

 ~PSViewPathFlattenVisitor() { }

  void moveTo(const CPoint2D &p) {
    flatPath_.moveTo(p);
  }

  void lineTo(const CPoint2D &p) {
    flatPath_.lineTo(p);
  }

  void bezier2To(const CPoint2D &p1, const CPoint2D &p2) {
    CPoint2D p;

    flatPath_.getCurrentPoint(p);

    C2Bezier2D bezier(p, p1, p2);

    vector<CPoint2D> points;

    CBezierToLine b_to_l;

    b_to_l.toLines(bezier, points);

    uint num_points = points.size();

    for (uint i = 1; i < num_points; ++i)
      flatPath_.lineTo(points[i]);
  }

  void bezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) {
    CPoint2D p;

    flatPath_.getCurrentPoint(p);

    C3Bezier2D bezier(p, p1, p2, p3);

    vector<CPoint2D> points;

    CBezierToLine b_to_l;

    b_to_l.toLines(bezier, points);

    uint num_points = points.size();

    for (uint i = 1; i < num_points; ++i)
      flatPath_.lineTo(points[i]);
  }

  void arcTo(const CPoint2D &c, double xr, double yr, double theta, double delta) {
    vector<C3Bezier2D> beziers;

    CArcToBezier::ArcToBeziers(c.x, c.y, xr, yr, theta, theta + delta, beziers);

    uint num_beziers = beziers.size();

    for (uint i = 0; i < num_beziers; ++i) {
      const C3Bezier2D &bezier = beziers[i];

      bezier3To(bezier.getControlPoint1(), bezier.getControlPoint2(), bezier.getLastPoint());
    }
  }

  void close() {
    flatPath_.close();
  }

  const PSViewPath &getPath() const { return flatPath_; }

 private:
  PSViewPath flatPath_;
};

//-------

PSViewPath::
PSViewPath() :
 parts_      (),
 closed_     (false),
 start_      (0,0),
 current_    (0,0),
 current_set_(false)
{
}

PSViewPath::
PSViewPath(const PSViewPath &path) :
 parts_      (),
 closed_     (path.closed_),
 start_      (path.start_),
 current_    (path.current_),
 current_set_(path.current_set_)
{
  PartList::const_iterator p1, p2;

  for (p1 = path.parts_.begin(), p2 = path.parts_.end(); p1 != p2; ++p1)
    parts_.push_back((*p1)->dup());
}

PSViewPath::
~PSViewPath()
{
  reset();
}

PSViewPath &
PSViewPath::
operator=(const PSViewPath &path)
{
  reset();

  PartList::const_iterator p1, p2;

  for (p1 = path.parts_.begin(), p2 = path.parts_.end(); p1 != p2; ++p1)
    parts_.push_back((*p1)->dup());

  closed_      = path.closed_;
  start_       = path.start_;
  current_     = path.current_;
  current_set_ = path.current_set_;

  return *this;
}

PSViewPath *
PSViewPath::
dup() const
{
  return new PSViewPath(*this);
}

void
PSViewPath::
init()
{
  reset();
}

void
PSViewPath::
reset()
{
  std::for_each(parts_.begin(), parts_.end(), CDeletePointer());

  parts_.clear();

  closed_      = false;
  current_set_ = false;
}

bool
PSViewPath::
moveTo(const CPoint2D &p)
{
  uint numParts = parts_.size();

  if (numParts > 0 && parts_[numParts - 1]->getId() == PSVIEW_PATH_MOVE_TO) {
    delete parts_[numParts - 1];

    parts_.pop_back();
  }

  parts_.push_back(new PSViewPathMoveTo(p));

  closed_      = false;
  start_       = p;
  current_     = start_;
  current_set_ = true;

  return true;
}

bool
PSViewPath::
rmoveTo(const CPoint2D &p)
{
  if (current_set_)
    return moveTo(current_ + p);
  else
    return moveTo(p);
}

bool
PSViewPath::
lineTo(const CPoint2D &p)
{
  if (! current_set_)
    return moveTo(p);

  if (closed_)
    moveTo(start_);

  parts_.push_back(new PSViewPathLineTo(p));

  closed_  = false;
  current_ = p;

  return true;
}

bool
PSViewPath::
rlineTo(const CPoint2D &p)
{
  if (current_set_)
    return lineTo(current_ + p);
  else
    return moveTo(p);
}

bool
PSViewPath::
bezier2To(const CPoint2D &p1, const CPoint2D &p2)
{
  if (! current_set_)
    moveTo(CPoint2D(0, 0));

  parts_.push_back(new PSViewPathBezier2To(p1, p2));

  closed_  = false;
  current_ = p2;

  return true;
}

bool
PSViewPath::
rbezier2To(const CPoint2D &p1, const CPoint2D &p2)
{
  if (! current_set_)
    bezier2To(p1, p2);

  bezier2To(p1 + current_, p2 + current_);

  return true;
}

bool
PSViewPath::
bezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3)
{
  if (! current_set_)
    moveTo(CPoint2D(0, 0));

  parts_.push_back(new PSViewPathBezier3To(p1, p2, p3));

  closed_  = false;
  current_ = p3;

  return true;
}

bool
PSViewPath::
rbezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3)
{
  if (! current_set_)
    bezier3To(p1, p2, p3);

  bezier3To(p1 + current_, p2 + current_, p3 + current_);

  return true;
}

bool
PSViewPath::
arc(const CPoint2D &c, double xr, double yr, double angle1, double angle2)
{
  double a1 = CMathGen::RadToDeg(angle1);
  double a2 = CMathGen::RadToDeg(angle2);

  int    fa, fs;
  double x0, y0, x1, y1;

  CMathGeom2D::ConvertToSVGArc(c.x, c.y, xr, yr, a1, a2 - a1, 0.0,
                               &x0, &y0, &x1, &y1, &fa, &fs);

  if (! current_set_)
    moveTo(CPoint2D(x0, y0));

  parts_.push_back(new PSViewPathArcTo(c, xr, yr, angle1, angle2 - angle1));

  closed_  = false;
  current_ = CPoint2D(x1, y1);

  return true;
}

bool
PSViewPath::
arcN(const CPoint2D &c, double xr, double yr, double angle1, double angle2)
{
  double a1 = CMathGen::RadToDeg(angle1);
  double a2 = CMathGen::RadToDeg(angle2);

  int    fa, fs;
  double x0, y0, x1, y1;

  CMathGeom2D::ConvertToSVGArc(c.x, c.y, xr, yr, a1, a1 - a2, 0.0,
                               &x0, &y0, &x1, &y1, &fa, &fs);

  if (! current_set_)
    moveTo(CPoint2D(x0, y0));

  parts_.push_back(new PSViewPathArcTo(c, xr, yr, angle1, angle1 - angle2));

  closed_  = false;
  current_ = CPoint2D(x1, y1);

  return true;
}

bool
PSViewPath::
arcTo(const CPoint2D &p1, const CPoint2D &p2, double xr, double yr)
{
  if (! current_set_)
    moveTo(p1);

  CPoint2D p0(current_);

  double x[3] = { p0.x, p1.x, p2.x };
  double y[3] = { p0.y, p1.y, p2.y };

  CLineList2D path(x, y, 3);

  double xc, yc, xt1, yt1, xt2, yt2;

  bool flag = path.arcThrough(xr, yr, &xc, &yc, &xt1, &yt1, &xt2, &yt2);

  if (flag) {
    if (xt1 != p0.x || yt1 != p0.y)
      lineTo(CPoint2D(xt1, yt1));

    CTriangle2D triangle(p0, p1, p2);

    if (triangle.orientation() == 1)
      arc (CPoint2D(xc, yc), xr, yr, atan2(xt1 - xc, yt1 - yc), atan2(xt2 - xc, yt2 - yc));
    else
      arcN(CPoint2D(xc, yc), xr, yr, atan2(xt1 - xc, yt1 - yc), atan2(xt2 - xc, yt2 - yc));
  }
  else
    lineTo(p1);

  return true;
}

bool
PSViewPath::
addChar(int c, const CMatrix2D &m, double *x, double *y)
{
  PSViewPathCharRenderer renderer(this, m, CPoint2D(*x, *y));

  double x1 = 0.0, y1 = 0.0;

  CFreeTypeMgrInst->pathChar(&renderer, char(c), &x1, &y1);

  CPoint2D tp = renderer.adjustPoint(CPoint2D(x1, y1));

  *x = tp.x;
  *y = tp.y;

  return true;
}

void
PSViewPath::
close()
{
  if (! current_set_ || closed_) {
    closed_ = true;
    return;
  }

  parts_.push_back(new PSViewPathClose);

  current_ = start_;
  closed_  = true;
}

bool
PSViewPath::
getCurrentPoint(CPoint2D &p)
{
  if (! current_set_)
    return false;

  p = current_;

  return true;
}

void
PSViewPath::
flatten()
{
  PSViewPathFlattenVisitor visitor;

  process(visitor);

  *this = visitor.getPath();
}

void
PSViewPath::
reverse()
{
  assert(false);
}

void
PSViewPath::
setStrokeAdjust(bool)
{
}

void
PSViewPath::
getBounds(double *xmin, double *ymin, double *xmax, double *ymax)
{
  PSViewPathBBoxVisitor visitor;

  process(visitor);

  const CBBox2D &bbox = visitor.getBBox();

  *xmin = bbox.getXMin();
  *ymin = bbox.getYMin();
  *xmax = bbox.getXMax();
  *ymax = bbox.getYMax();
}

void
PSViewPath::
process(PSViewPathVisitor &visitor)
{
  visitor.setPath(this);

  visitor.init();

  PartList::const_iterator p1, p2;

  for (p1 = parts_.begin(), p2 = parts_.end(); p1 != p2; ++p1)
    (*p1)->visit(visitor);

  visitor.term();
}

void
PSViewPath::
print(std::ostream &os) const
{
  PartList::const_iterator p1, p2;

  for (p1 = parts_.begin(), p2 = parts_.end(); p1 != p2; ++p1)
    (*p1)->print(os);
}
