#include <CPSViewI.h>
#include <CRGBA.h>
#include <CHSV.h>
#include <CLineList2D.h>
#include <CBezierToLine.h>

#include <CPSViewGStateFont.h>
#include <CPSViewGStatePattern.h>

class PSViewTokenPathVisitor : public PSViewPathVisitor {
 private:
  CPSView     *psview_;
  PSViewToken *move_to_token_;
  PSViewToken *line_to_token_;
  PSViewToken *curve_to_token_;
  PSViewToken *close_path_token_;

 public:
  PSViewTokenPathVisitor(CPSView *psview,
                         PSViewToken *move_to_token,
                         PSViewToken *line_to_token,
                         PSViewToken *curve_to_token,
                         PSViewToken *close_path_token) :
   psview_(psview), move_to_token_(move_to_token), line_to_token_(line_to_token),
   curve_to_token_(curve_to_token), close_path_token_(close_path_token) {
  }

  void moveTo(const CPoint2D &point) {
    PSViewToken *token1 = new PSViewRealToken(psview_, point.x);
    PSViewToken *token2 = new PSViewRealToken(psview_, point.y);

    psview_->getOperandStack()->push(token1);
    psview_->getOperandStack()->push(token2);

    move_to_token_->execute();
  }

  void lineTo(const CPoint2D &point) {
    PSViewToken *token1 = new PSViewRealToken(psview_, point.x);
    PSViewToken *token2 = new PSViewRealToken(psview_, point.y);

    psview_->getOperandStack()->push(token1);
    psview_->getOperandStack()->push(token2);

    line_to_token_->execute();
  }

  void bezier2To(const CPoint2D &point1, const CPoint2D &point2) {
    PSViewToken *token1 = new PSViewRealToken(psview_, point1.x);
    PSViewToken *token2 = new PSViewRealToken(psview_, point1.y);
    PSViewToken *token3 = new PSViewRealToken(psview_, point2.x);
    PSViewToken *token4 = new PSViewRealToken(psview_, point2.y);

    psview_->getOperandStack()->push(token1);
    psview_->getOperandStack()->push(token2);
    psview_->getOperandStack()->push(token3);
    psview_->getOperandStack()->push(token4);

    curve_to_token_->execute();
  }

  void bezier3To(const CPoint2D &point1, const CPoint2D &point2, const CPoint2D &point3) {
    PSViewToken *token1 = new PSViewRealToken(psview_, point1.x);
    PSViewToken *token2 = new PSViewRealToken(psview_, point1.y);
    PSViewToken *token3 = new PSViewRealToken(psview_, point2.x);
    PSViewToken *token4 = new PSViewRealToken(psview_, point2.y);
    PSViewToken *token5 = new PSViewRealToken(psview_, point3.x);
    PSViewToken *token6 = new PSViewRealToken(psview_, point3.y);

    psview_->getOperandStack()->push(token1);
    psview_->getOperandStack()->push(token2);
    psview_->getOperandStack()->push(token3);
    psview_->getOperandStack()->push(token4);
    psview_->getOperandStack()->push(token5);
    psview_->getOperandStack()->push(token6);

    curve_to_token_->execute();
  }

  void arcTo(const CPoint2D &, double, double, double, double) {
    // TODO
  }

  void close() {
    close_path_token_->execute();
  }

 private:
  PSViewTokenPathVisitor(const PSViewTokenPathVisitor &rhs);
  PSViewTokenPathVisitor &operator=(const PSViewTokenPathVisitor &rhs);
};

class PSViewPathRenderer : public PSViewPathVisitor {
 private:
  CPSViewRenderer *renderer_;

 public:
  PSViewPathRenderer(CPSViewRenderer *renderer) :
   renderer_(renderer) {
    renderer->pathInit();
  }

  void moveTo(const CPoint2D &p) {
    renderer_->pathMoveTo(p);
  }

  void lineTo(const CPoint2D &p) {
    renderer_->pathLineTo(p);
  }

  void bezier2To(const CPoint2D &p1, const CPoint2D &p2) {
    renderer_->pathBezier2To(p1, p2);
  }

  void bezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) {
    renderer_->pathBezier3To(p1, p2, p3);
  }

  void arcTo(const CPoint2D &c, double xr, double yr, double theta, double delta) {
    renderer_->pathArc(c, xr, yr, theta, theta + delta);
  }

  void close() {
    renderer_->pathClose();
  }

 private:
  PSViewPathRenderer(const PSViewPathRenderer &rhs);
  PSViewPathRenderer &operator=(const PSViewPathRenderer &rhs);
};

class PSViewPathStroker : public PSViewPathVisitor {
 public:
  PSViewPathStroker(PSViewGState *gstate) :
   gstate_ (gstate),
   start_  (0,0),
   current_(0,0),
   closed_ (false),
   inner_  (),
   outer_  (),
   path_   () {
    path_ = new PSViewPath;
  }

  void init() {
  }

  void term() {
    buildPath();
  }

  void moveTo(const CPoint2D &p) {
    start_   = p;
    current_ = p;
    closed_  = false;
  }

  void lineTo(const CPoint2D &p) {
    strokeLine(current_, p);

    current_ = p;
    closed_  = false;
  }

  void bezier2To(const CPoint2D &p1, const CPoint2D &p2) {
    strokeBezier2(current_, p1, p2);

    current_ = p2;
    closed_  = false;
  }

  void bezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) {
    strokeBezier3(current_, p1, p2, p3);

    current_ = p3;
    closed_  = false;
  }

  void arcTo(const CPoint2D &c, double xr, double yr, double theta, double delta) {
    strokeArc(c, xr, yr, theta, delta);

    int    fa, fs;
    double x0, y0, x1, y1;

    CMathGeom2D::ConvertToSVGArc(c.x, c.y, xr, yr, theta, delta, 0.0,
                                 &x0, &y0, &x1, &y1, &fa, &fs);

    current_ = CPoint2D(x1, y1);
    closed_  = false;
  }

  void close() {
    if (! closed_) {
      strokeLine(current_, start_);

      closed_ = true;
    }

    buildPath();
  }

  PSViewPath *getPath() const { return path_; }

 private:
  void strokeBezier2(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) {
    C2Bezier2D bezier(p1, p2, p3);

    vector<CPoint2D> points;

    CBezierToLine b_to_l;

    b_to_l.toLines(bezier, points);

    uint numPoints = points.size();

    for (uint i = 1; i < numPoints; ++i)
      strokeLine(points[i - 1], points[i]);
  }

  void strokeBezier3(const CPoint2D &p1, const CPoint2D &p2,
                     const CPoint2D &p3, const CPoint2D &p4) {
    C3Bezier2D bezier(p1, p2, p3, p4);

    vector<CPoint2D> points;

    CBezierToLine b_to_l;

    b_to_l.toLines(bezier, points);

    uint numPoints = points.size();

    for (uint i = 1; i < numPoints; ++i)
      strokeLine(points[i - 1], points[i]);
  }

  void strokeArc(const CPoint2D &, double, double, double, double) {
  }

  void strokeLine(const CPoint2D &p1, const CPoint2D &p2) {
    double w = gstate_->getLineWidth()/2.0;

    double x21 = p2.x - p1.x;
    double y21 = p2.y - p1.y;

    if (x21 == 0.0 && y21 == 0.0) return;

    double g = atan2(y21, x21);

    double dx = w*cos(g);
    double dy = w*sin(g);

    inner_.push_back(CLine2D(CPoint2D(p1.x + dx, p1.y - dy),
                             CPoint2D(p2.x + dx, p2.y - dy)));
    outer_.push_back(CLine2D(CPoint2D(p2.x - dx, p2.y + dy),
                             CPoint2D(p1.x - dx, p1.y + dy)));
  }

  void buildPath() {
    uint num_inner = inner_.size();
    uint num_outer = outer_.size();

    if (num_inner == 0 || num_outer == 0)
      return;

    path_->init();

    if (! closed_) { // not closed so inner + outer form a single shape
      const CPoint2D *p1 = &inner_[0].start();
      const CPoint2D *p2 = &inner_[0].end  ();

      path_->moveTo(*p1);

      for (uint i = 1; i < num_inner; ++i) {
        const CPoint2D *p3 = &inner_[i].start();
        const CPoint2D *p4 = &inner_[i].end  ();

        CPoint2D pi;
        double   mu1, mu2;

        CMathGeom2D::IntersectLine(CLine2D(*p1, *p2), CLine2D(*p3, *p4), &pi, &mu1, &mu2);

        if (mu1 >= 0.0 && mu1 <= 1.0) {
          path_->lineTo(pi);
        }
        else {
          path_->lineTo(pi);
          //path_->lineTo(*p2);
          //path_->lineTo(*p3);
        }

        p1 = p3;
        p2 = p4;
      }

      for (uint i = 0; i < num_outer; ++i) {
        const CPoint2D *p3 = &outer_[num_outer - i - 1].start();
        const CPoint2D *p4 = &outer_[num_outer - i - 1].end  ();

        CPoint2D pi;
        double   mu1, mu2;

        CMathGeom2D::IntersectLine(CLine2D(*p1, *p2), CLine2D(*p3, *p4), &pi, &mu1, &mu2);

        if (mu1 > 0.0 && mu1 < 1.0) {
          path_->lineTo(pi);
        }
        else {
          path_->lineTo(pi);
          //path_->lineTo(*p2);
          //path_->lineTo(*p3);
        }

        p1 = p3;
        p2 = p4;
      }

      path_->lineTo(*p2);

      path_->close();
    }
    else { // closed inner anf outer form separate shapes
      // inner
      {
      const CPoint2D *p1 = &inner_[num_inner - 1].start();
      const CPoint2D *p2 = &inner_[num_inner - 1].end  ();

      const CPoint2D *p3 = &inner_[0].start();
      const CPoint2D *p4 = &inner_[0].end  ();

      CPoint2D pi;
      double   mu1, mu2;

      CMathGeom2D::IntersectLine(CLine2D(*p1, *p2), CLine2D(*p3, *p4), &pi, &mu1, &mu2);

      path_->moveTo(pi);

      p1 = p3;
      p2 = p4;

      for (uint i = 1; i < num_inner; ++i) {
        const CPoint2D *p3 = &inner_[i].start();
        const CPoint2D *p4 = &inner_[i].end  ();

        CPoint2D pi;
        double   mu1, mu2;

        CMathGeom2D::IntersectLine(CLine2D(*p1, *p2), CLine2D(*p3, *p4), &pi, &mu1, &mu2);

        path_->lineTo(pi);

        p1 = p3;
        p2 = p4;
      }

      path_->close();
      }

      // outer
      {
      const CPoint2D *p1 = &outer_[0].start();
      const CPoint2D *p2 = &outer_[0].end  ();

      const CPoint2D *p3 = &outer_[num_outer - 1].start();
      const CPoint2D *p4 = &outer_[num_outer - 1].end  ();

      CPoint2D pi;
      double   mu1, mu2;

      CMathGeom2D::IntersectLine(CLine2D(*p1, *p2), CLine2D(*p3, *p4), &pi, &mu1, &mu2);

      path_->moveTo(pi);

      p1 = p3;
      p2 = p4;

      for (uint i = 1; i < num_outer; ++i) {
        const CPoint2D *p3 = &outer_[num_outer - i - 1].start();
        const CPoint2D *p4 = &outer_[num_outer - i - 1].end  ();

        CPoint2D pi;
        double   mu1, mu2;

        CMathGeom2D::IntersectLine(CLine2D(*p1, *p2), CLine2D(*p3, *p4), &pi, &mu1, &mu2);

        path_->lineTo(pi);

        p1 = p3;
        p2 = p4;
      }

      path_->close();
      }
    }

    inner_.clear();
    outer_.clear();
  }

 private:
  PSViewGState         *gstate_;
  CPoint2D              start_;
  CPoint2D              current_;
  bool                  closed_;
  vector<CLine2D>       inner_;
  vector<CLine2D>       outer_;
  CAutoPtr<PSViewPath>  path_;

 private:
  PSViewPathStroker(const PSViewPathStroker &rhs);
  PSViewPathStroker &operator=(const PSViewPathStroker &rhs);
};

//-----------

PSViewGStateMgr::
PSViewGStateMgr(CPSView *psview) :
 psview_               (psview),
 CMYKColorSpaceName_   (&psview->getNameMgr()->getName("DeviceCMYK")),
 RGBColorSpaceName_    (&psview->getNameMgr()->getName("DeviceRGB")),
 GrayColorSpaceName_   (&psview->getNameMgr()->getName("DeviceGray")),
 PatternColorSpaceName_(&psview->getNameMgr()->getName("Pattern")),
 font_dir_             (),
 default_ctm_matrix_   (),
 current_font_id_      (0),
 default_font_         (NULL)
{
  font_dir_ = COSUser::getUserHome() + "/data/PSView/fonts";

  init();
}

PSViewGStateMgr::
~PSViewGStateMgr()
{
}

void
PSViewGStateMgr::
init()
{
  default_ctm_matrix_.setIdentity();

  //------

  createDefaultFont();
}

CMatrix2D *
PSViewGStateMgr::
getDefaultMatrix()
{
  return &default_ctm_matrix_;
}

void
PSViewGStateMgr::
createDefaultFont()
{
  default_font_ = new PSViewDictionaryToken(getPSView()->getDictionaryMgr(), 10);

  //------

  PSViewToken *key = new PSViewNameToken(getPSView(), "FontType");

  PSViewToken *value = new PSViewIntegerToken(getPSView(), PSVIEW_FONT_TYPE_DEFAULT);

  default_font_->addValue(key, value);

  //------

  CMatrix2D matrix;

  matrix.setIdentity();

  key   = new PSViewNameToken(getPSView(), "FontMatrix");
  value = new PSViewArrayToken(getPSView(), &matrix);

  default_font_->addValue(key, value);
}

//-----------

PSViewGState::
PSViewGState(PSViewGStateMgr *mgr) :
 mgr_               (mgr),
 ctm_matrix_        (),
 color_space_       (NULL),
 pattern_color_     (false),
 pen_               (),
 brush_             (),
 cmyk_              (0, 0, 0, 0),
 cmyk_valid_        (false),
 hsb_               (0, 0, 0),
 hsb_valid_         (false),
 stroke_adjust_     (false),
 black_generation_  (),
 undercolor_removal_(),
 font_              (),
 graphics_          (NULL),
 pattern_           (),
 path_              (),
 clippath_          ()
{
  pen_.setColor(CRGBA(0,0,0));
  pen_.setMitreLimit(10.0);
  pen_.setFlatness(2.0);

  brush_.setColor(CRGBA(0,0,0));

  ctm_matrix_.setIdentity();

  color_space_ = &mgr_->getGrayColorSpace();
  font_        = new PSViewGStateFont;
  graphics_    = getPSView()->getGraphics();
  path_        = new PSViewPath;
  clippath_    = new PSViewPath;

  setFont(mgr_->getDefaultFont());

  initGraphics();

  newPath();
}

PSViewGState::
PSViewGState(const PSViewGState &gstate) :
 mgr_               (gstate.mgr_          ),
 ctm_matrix_        (gstate.ctm_matrix_   ),
 color_space_       (gstate.color_space_  ),
 pattern_color_     (gstate.pattern_color_),
 pen_               (gstate.pen_          ),
 brush_             (gstate.brush_        ),
 cmyk_              (gstate.cmyk_         ),
 cmyk_valid_        (gstate.cmyk_valid_   ),
 hsb_               (gstate.hsb_          ),
 hsb_valid_         (gstate.hsb_valid_    ),
 stroke_adjust_     (gstate.stroke_adjust_),
 black_generation_  (                     ),
 undercolor_removal_(                     ),
 font_              (                     ),
 graphics_          (NULL                 ),
 pattern_           (                     ),
 path_              (                     ),
 clippath_          (                     )
{
  if (gstate.black_generation_.isValid())
    black_generation_ = gstate.black_generation_->dup();

  if (gstate.undercolor_removal_.isValid())
    undercolor_removal_ = gstate.undercolor_removal_->dup();

  if (gstate.font_.isValid())
    font_ = new PSViewGStateFont(*gstate.font_);

  graphics_ = getPSView()->getGraphics();

  if (gstate.pattern_.isValid())
    pattern_ = new PSViewGStatePattern(*gstate.pattern_);

  if (gstate.path_.isValid())
    path_ = gstate.path_->dup();

  if (gstate.clippath_.isValid())
    clippath_ = gstate.clippath_->dup();
}

PSViewGState::
~PSViewGState()
{
}

const PSViewGState &
PSViewGState::
operator=(const PSViewGState &gstate)
{
  if (this == &gstate)
    return *this;

  ctm_matrix_         = gstate.ctm_matrix_;
  color_space_        = gstate.color_space_;
  pattern_color_      = gstate.pattern_color_;
  pen_                = gstate.pen_;
  brush_              = gstate.brush_;
  cmyk_               = gstate.cmyk_;
  cmyk_valid_         = gstate.cmyk_valid_;
  hsb_                = gstate.hsb_;
  hsb_valid_          = gstate.hsb_valid_;
  stroke_adjust_      = gstate.stroke_adjust_;
  black_generation_   = NULL;
  undercolor_removal_ = NULL;
  font_               = NULL;
  pattern_            = NULL;
  path_               = NULL;
  clippath_           = NULL;

  if (gstate.black_generation_.isValid())
    black_generation_ = gstate.black_generation_->dup();

  if (gstate.undercolor_removal_.isValid())
    undercolor_removal_ = gstate.undercolor_removal_->dup();

  if (gstate.font_.isValid())
    font_ = new PSViewGStateFont(*gstate.font_);

  graphics_ = getPSView()->getGraphics();

  if (gstate.pattern_.isValid())
    pattern_ = new PSViewGStatePattern(*gstate.pattern_);

  if (gstate.path_.isValid())
    path_ = gstate.path_->dup();

  if (gstate.clippath_.isValid())
    clippath_ = gstate.clippath_->dup();

  return *this;
}

int
PSViewGState::
compare(PSViewGState *)
{
  return 1;
}

void
PSViewGState::
initGraphics()
{
  initMatrix();
  newPath   ();
  initClip  ();

  setColorSpace  (mgr_->getGrayColorSpace());
  setPatternColor(false);
  setGray        (0.0);
  setFlat        (2.0);
  setLineWidth   (1.0);
  setLineCap     (LINE_CAP_TYPE_BUTT);
  setLineJoin    (LINE_JOIN_TYPE_MITRE);
  setMitreLimit  (10.0);
  setStrokeAdjust(false);
  setDashPattern (NULL, 0, 0.0);
}

void
PSViewGState::
initMatrix()
{
  setCTMMatrix(mgr_->getDefaultMatrix());
}

void
PSViewGState::
setGraphics()
{
  if (graphics_ != NULL) {
    CPen pen = pen_;

    double tx, ty;

    multiplyDistByCTMMatrix(pen_.getWidth(), pen_.getWidth(), &tx, &ty);

    pen.setWidth(tx);

    graphics_->setPen(pen);

    graphics_->setBrush(brush_);
  }
}

void
PSViewGState::
newPath()
{
  path_->init();
}

void
PSViewGState::
initClip()
{
  if (graphics_ != NULL) {
    clippath_->init();

    double xmin = 0.0, ymin = 0.0, xmax = CPSView::PAGE_WIDTH - 1, ymax = CPSView::PAGE_HEIGHT - 1;

    clippath_->moveTo(CPoint2D(xmin, ymin));
    clippath_->lineTo(CPoint2D(xmax, ymin));
    clippath_->lineTo(CPoint2D(xmax, ymax));
    clippath_->lineTo(CPoint2D(xmin, ymax));

    clippath_->close();
  }
}

void
PSViewGState::
moveTo(double x, double y)
{
  double x1, y1;

  multiplyByCTMMatrix(x, y, &x1, &y1);

  path_->moveTo(CPoint2D(x1, y1));
}

void
PSViewGState::
rmoveTo(double x, double y)
{
  double x1, y1;

  multiplyDistByCTMMatrix(x, y, &x1, &y1);

  if (! path_->rmoveTo(CPoint2D(x1, y1)))
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_NO_CURRENT_POINT);
}

void
PSViewGState::
lineTo(double x, double y)
{
  double x1, y1;

  multiplyByCTMMatrix(x, y, &x1, &y1);

  if (! path_->lineTo(CPoint2D(x1, y1)))
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_NO_CURRENT_POINT);
}

void
PSViewGState::
rlineTo(double x, double y)
{
  double x1, y1;

  multiplyDistByCTMMatrix(x, y, &x1, &y1);

  if (! path_->rlineTo(CPoint2D(x1, y1)))
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_NO_CURRENT_POINT);
}

void
PSViewGState::
arc(double x, double y, double r, double angle1, double angle2)
{
  double x1, y1;

  multiplyByCTMMatrix(x, y, &x1, &y1);

  double rx1, ry1;

  multiplyDistByCTMMatrix(r, r, &rx1, &ry1);

  path_->arc(CPoint2D(x1, y1), rx1, ry1, angle1, angle2);
}

void
PSViewGState::
arcN(double x, double y, double r, double angle1, double angle2)
{
  double x1, y1;

  multiplyByCTMMatrix(x, y, &x1, &y1);

  double rx1, ry1;

  multiplyDistByCTMMatrix(r, r, &rx1, &ry1);

  path_->arcN(CPoint2D(x1, y1), rx1, ry1, angle1, angle2);
}

void
PSViewGState::
arcTo(double x1, double y1, double x2, double y2, double r, double *xt1, double *yt1,
      double *xt2, double *yt2)
{
  double x0, y0;

  getCurrentPoint(&x0, &y0);

  //---

  // get angles of two tangents intersecting at (x1, y1)
  double x01 = x0 - x1;
  double y01 = y0 - y1;
  double x21 = x2 - x1;
  double y21 = y2 - y1;

  double id1 = 1.0/sqrt(x01*x01 + y01*y01);
  double id2 = 1.0/sqrt(x21*x21 + y21*y21);

  //double g1 = atan2(y01, x01);
  //double g2 = atan2(y21, x21);

  //---

  // get angles of perpendiculars
  double pg1 = atan2(-x01, y01);
  double pg2 = atan2(-x21, y21);

  //---

  // get included angle
  double a = CMathGeom2D::IncludedAngle(CPoint2D(x0, y0), CPoint2D(x1, y1), CPoint2D(x2, y2));

  // get distance along lines to tangents (adjacent edge of triangle with angle of half included
  // angle and opposite of 'r')
  double d = r/tan(a/2);

  // calc tangent points (d along line from (x1, y1) using tangent gradient
  //*xt1 = x1 + d*cos(g1); *yt1 = y1 + d*sin(g1);
  //*xt2 = x1 + d*cos(g2); *yt2 = y1 + d*sin(g2);

  *xt1 = x1 + d*x01*id1; *yt1 = y1 + d*y01*id1;
  *xt2 = x1 + d*x21*id2; *yt2 = y1 + d*y21*id2;

  // intersect lines perpendicular to tangent points to get center
  double xi, yi, mu1, mu2;

  CPoint2D pi;

  CMathGeom2D::IntersectLine(CLine2D(CPoint2D(*xt1, *yt1),
                                     CPoint2D(*xt1 + cos(pg1), *yt1 + sin(pg1))),
                             CLine2D(CPoint2D(*xt2, *yt2),
                                     CPoint2D(*xt2 + cos(pg2), *yt2 + sin(pg2))),
                             &pi, &mu1, &mu2);

  xi = pi.x;
  yi = pi.y;

  //------

  // arc angles from center to tangent points
  double ag1 = atan2(*yt1 - yi, *xt1 - xi);
  double ag2 = atan2(*yt2 - yi, *xt2 - xi);

  // draw arc
  arc(xi, yi, r, ag1, ag2);
}

void
PSViewGState::
curveTo(double x1, double y1, double x2, double y2, double x3, double y3)
{
  double tx1, ty1, tx2, ty2, tx3, ty3;

  multiplyByCTMMatrix(x1, y1, &tx1, &ty1);
  multiplyByCTMMatrix(x2, y2, &tx2, &ty2);
  multiplyByCTMMatrix(x3, y3, &tx3, &ty3);

  if (! path_->bezier3To(CPoint2D(tx1, ty1), CPoint2D(tx2, ty2), CPoint2D(tx3, ty3)))
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_NO_CURRENT_POINT);
}

void
PSViewGState::
rcurveTo(double x1, double y1, double x2, double y2, double x3, double y3)
{
  double tx1, ty1, tx2, ty2, tx3, ty3;

  multiplyDistByCTMMatrix(x1, y1, &tx1, &ty1);
  multiplyDistByCTMMatrix(x2, y2, &tx2, &ty2);
  multiplyDistByCTMMatrix(x3, y3, &tx3, &ty3);

  if (! path_->rbezier3To(CPoint2D(tx1, ty1), CPoint2D(tx2, ty2), CPoint2D(tx3, ty3)))
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_NO_CURRENT_POINT);
}

bool
PSViewGState::
getCurrentPoint(double *x, double *y)
{
  CPoint2D p1;

  bool flag = path_->getCurrentPoint(p1);

  if (! flag)
    return false;

  multiplyByInverseCTMMatrix(p1.x, p1.y, x, y);

  return true;
}

void
PSViewGState::
show(const string &str)
{
  PSViewGStateFont *font = getFont();

  PSViewFontType type = font->getType();

  if      (type == PSVIEW_FONT_TYPE_DEFAULT) {
  }
  else if (type == PSVIEW_FONT_TYPE_0) {
  }
  else if (type == PSVIEW_FONT_TYPE_1)
    CStrUtil::eprintf("PSView: Show not implemented for Type 1 Fonts\n");
  else if (type == PSVIEW_FONT_TYPE_3) {
  }

  //------

  if (type == PSVIEW_FONT_TYPE_0) {
    PSViewDictionaryToken *font_dictionary = font->getDictionary();

    string str1 = " ";

    int i = 0;

    int len = str.size();

    while (i < len) {
      PSViewDictionaryToken *font_dictionary1 = NULL;

      str1[0] = getType0Char(str, &i, &font_dictionary1);

      setFont1(font_dictionary1);

      show(str1);

      setFont1(font_dictionary);
    }

    return;
  }

  //------

  CPoint2D p;

  bool flag = path_->getCurrentPoint(p);

  if (! flag) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_NO_CURRENT_POINT);
    return;
  }

  //------

  int len = str.size();

  for (int i = 1; i <= len; i++) {
    if      (type == PSVIEW_FONT_TYPE_DEFAULT)
      showDefaultChar(str[i - 1], &p.x, &p.y);
    else if (type == PSVIEW_FONT_TYPE_1)
      showDefaultChar(str[i - 1], &p.x, &p.y);
    else if (type == PSVIEW_FONT_TYPE_3)
      showType3Char  (str[i - 1], &p.x, &p.y);
  }

  //------

  double x1, y1;

  multiplyByInverseCTMMatrix(p.x, p.y, &x1, &y1);

  moveTo(x1, y1);
}

void
PSViewGState::
ashow(double ax, double ay, const string &str)
{
  PSViewGStateFont *font = getFont();

  PSViewFontType type = font->getType();

  if      (type == PSVIEW_FONT_TYPE_DEFAULT) {
  }
  else if (type == PSVIEW_FONT_TYPE_0) {
  }
  else if (type == PSVIEW_FONT_TYPE_1)
    CStrUtil::eprintf("PSView: AShow not implemented for Type 1 Fonts\n");
  else if (type == PSVIEW_FONT_TYPE_3) {
  }

  //------

  if (type == PSVIEW_FONT_TYPE_0) {
    PSViewDictionaryToken *font_dictionary = font->getDictionary();

    string str1 = " ";

    int i = 0;

    int len = str.size();

    while (i < len) {
      PSViewDictionaryToken *font_dictionary1 = NULL;

      str1[0] = getType0Char(str, &i, &font_dictionary1);

      setFont1(font_dictionary1);

      ashow(ax, ay, str1);

      setFont1(font_dictionary);
    }

    return;
  }

  //------

  CPoint2D p;

  bool flag = path_->getCurrentPoint(p);

  if (! flag) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_NO_CURRENT_POINT);
    return;
  }

  //------

  int len = str.size();

  for (int i = 1; i <= len; i++) {
    if      (type == PSVIEW_FONT_TYPE_DEFAULT)
      showDefaultChar(str[i - 1], &p.x, &p.y);
    else if (type == PSVIEW_FONT_TYPE_1)
      showDefaultChar(str[i - 1], &p.x, &p.y);
    else if (type == PSVIEW_FONT_TYPE_3)
      showType3Char  (str[i - 1], &p.x, &p.y);

    p.x += ax;
    p.y += ay;
  }

  //------

  double x1, y1;

  multiplyByInverseCTMMatrix(p.x, p.y, &x1, &y1);

  moveTo(x1, y1);
}

void
PSViewGState::
widthShow(double cx, double cy, int c, const string &str)
{
  PSViewGStateFont *font = getFont();

  PSViewFontType type = font->getType();

  if      (type == PSVIEW_FONT_TYPE_DEFAULT) {
  }
  else if (type == PSVIEW_FONT_TYPE_0) {
  }
  else if (type == PSVIEW_FONT_TYPE_1)
    CStrUtil::eprintf("PSView: WidthShow not implemented for Type 1 Fonts\n");
  else if (type == PSVIEW_FONT_TYPE_3) {
  }

  //------

  if (type == PSVIEW_FONT_TYPE_0) {
    PSViewDictionaryToken *font_dictionary = font->getDictionary();

    string str1 = " ";

    int i = 0;

    int len = str.size();

    while (i < len) {
      PSViewDictionaryToken *font_dictionary1 = NULL;

      str1[0] = getType0Char(str, &i, &font_dictionary1);

      setFont1(font_dictionary1);

      widthShow(cx, cy, c, str1);

      setFont1(font_dictionary);
    }

    return;
  }

  //------

  CPoint2D p;

  bool flag = path_->getCurrentPoint(p);

  if (! flag) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_NO_CURRENT_POINT);
    return;
  }

  //------

  int len = str.size();

  for (int i = 1; i <= len; i++) {
    if      (type == PSVIEW_FONT_TYPE_DEFAULT)
      showDefaultChar(str[i - 1], &p.x, &p.y);
    else if (type == PSVIEW_FONT_TYPE_1)
      showDefaultChar(str[i - 1], &p.x, &p.y);
    else if (type == PSVIEW_FONT_TYPE_3)
      showType3Char  (str[i - 1], &p.x, &p.y);

    if (str[i - 1] == c) {
      p.x += cx;
      p.y += cy;
    }
  }

  //------

  double x1, y1;

  multiplyByInverseCTMMatrix(p.x, p.y, &x1, &y1);

  moveTo(x1, y1);
}

void
PSViewGState::
awidthShow(double cx, double cy, int c, double ax, double ay, const string &str)
{
  PSViewGStateFont *font = getFont();

  PSViewFontType type = font->getType();

  if      (type == PSVIEW_FONT_TYPE_DEFAULT) {
  }
  else if (type == PSVIEW_FONT_TYPE_0) {
  }
  else if (type == PSVIEW_FONT_TYPE_1)
    CStrUtil::eprintf("PSView: AShow not implemented for Type 1 Fonts\n");
  else if (type == PSVIEW_FONT_TYPE_3) {
  }

  //------

  if (type == PSVIEW_FONT_TYPE_0) {
    PSViewDictionaryToken *font_dictionary = font->getDictionary();

    string str1 = " ";

    int i = 0;

    int len = str.size();

    while (i < len) {
      PSViewDictionaryToken *font_dictionary1 = NULL;

      str1[0] = getType0Char(str, &i, &font_dictionary1);

      setFont1(font_dictionary1);

      awidthShow(cx, cy, c, ax, ay, str1);

      setFont1(font_dictionary);
    }

    return;
  }

  //------

  CPoint2D p;

  bool flag = path_->getCurrentPoint(p);

  if (! flag) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_NO_CURRENT_POINT);
    return;
  }

  //------

  int len = str.size();

  for (int i = 1; i <= len; i++) {
    if      (type == PSVIEW_FONT_TYPE_DEFAULT)
      showDefaultChar(str[i - 1], &p.x, &p.y);
    else if (type == PSVIEW_FONT_TYPE_1)
      showDefaultChar(str[i - 1], &p.x, &p.y);
    else if (type == PSVIEW_FONT_TYPE_3)
      showType3Char  (str[i - 1], &p.x, &p.y);

    p.x += ax;
    p.y += ay;

    if (str[i - 1] == c) {
      p.x += cx;
      p.y += cy;
    }
  }

  //------

  double x1, y1;

  multiplyByInverseCTMMatrix(p.x, p.y, &x1, &y1);

  moveTo(x1, y1);
}

void
PSViewGState::
glyphShow(const PSViewName &name)
{
  PSViewGStateFont *font = getFont();

  PSViewFontType type = font->getType();

  if      (type == PSVIEW_FONT_TYPE_DEFAULT)
    CStrUtil::eprintf("PSView: %s not implemented for Internal Fonts\n", "GlyphShow");
  else if (type == PSVIEW_FONT_TYPE_0)
    CStrUtil::eprintf("PSView: %s not implemented for Type 0 Fonts\n", "GlyphShow");
  else if (type == PSVIEW_FONT_TYPE_1)
    CStrUtil::eprintf("PSView: %s not implemented for Type 1 Fonts\n", "GlyphShow");
  else if (type == PSVIEW_FONT_TYPE_3) {
  }

  //------

  CPoint2D p;

  bool flag = path_->getCurrentPoint(p);

  if (! flag) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_NO_CURRENT_POINT);
    return;
  }

  //------

  if (type == PSVIEW_FONT_TYPE_3) {
    PSViewGStateFont3Data *font_data = font->getFont3Data();

    if (font_data->build_type == PSVIEW_FONT_BUILD_TYPE_GLYPH) {
      getPSView()->getOperandStack()->push(font->getDictionary());

      PSViewToken *token = new PSViewNameToken(getPSView(), name);

      getPSView()->getOperandStack()->push(token);

      //------

      getPSView()->getGStateTokenMgr()->save();

      newPath();

      moveTo(0, 0);

      double x1, y1, x2, y2;

      CMatrix2D *matrix = getCTMMatrix();

      matrix->getValues(&x1, &y1, &x2, &y2, NULL, NULL);
      matrix->setValues( x1,  y1,  x2,  y2,    0,    0);

      CMatrix2D tmatrix;

      tmatrix.setTranslation(p.x, p.y);

      preMultiplyCTMMatrix(&tmatrix);

      postMultiplyCTMMatrix(&font_data->matrix);

      setCharWidth(0.0, 0.0);

      font_data->execute();

      getCharWidth(&x1, &y1);

      getPSView()->getGStateTokenMgr()->restore();

      p.x += x1;
      p.y += y1;
    }
    else {
      PSViewArrayToken *array_token = (PSViewArrayToken *) font_data->encoding;

      int c;

      const PSViewName *name1 = &name;

      for (int i = 1; i <= 2; i++) {
        for (c = 1; c <= 256; c++) {
          PSViewToken *token = array_token->getValue(c);

          if (! token->isName()) {
            getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);

            return;
          }

          PSViewNameToken *name_token = (PSViewNameToken *) token;

          if (name_token->getValue().compare(*name1) == 0)
            break;
        }

        if (c <= 256)
          break;

        if (i == 2) {
          getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);

          return;
        }

        name1 = &getPSView()->getNameMgr()->getName(".notdef");
      }

      showType3Char(c, &p.x, &p.y);
    }
  }
  else {
    int c = ' ';

    showDefaultChar(c, &p.x, &p.y);
  }

  //------

  double x1, y1;

  multiplyByInverseCTMMatrix(p.x, p.y, &x1, &y1);

  moveTo(x1, y1);
}

void
PSViewGState::
kshow(PSViewToken *proc, const string &str)
{
  PSViewGStateFont *font = getFont();

  PSViewFontType type = font->getType();

  if      (type == PSVIEW_FONT_TYPE_DEFAULT) {
  }
  else if (type == PSVIEW_FONT_TYPE_0) {
  }
  else if (type == PSVIEW_FONT_TYPE_1)
    CStrUtil::eprintf("PSView: KShow not implemented for Type 1 Fonts\n");
  else if (type == PSVIEW_FONT_TYPE_3) {
  }

  //------

  CPoint2D p;

  bool flag = path_->getCurrentPoint(p);

  if (! flag) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_NO_CURRENT_POINT);
    return;
  }

  //------

  if (type == PSVIEW_FONT_TYPE_0) {
    PSViewDictionaryToken *font_dictionary = font->getDictionary();

    string str1 = " ";

    int i = 0;

    int len = str.size();

    while (i < len) {
      PSViewDictionaryToken *font_dictionary1 = NULL;

      str1[0] = getType0Char(str, &i, &font_dictionary1);

      setFont1(font_dictionary1);

      kshow(proc, str1);

      setFont1(font_dictionary);
    }

    return;
  }

  //------

  int len = str.size();

  for (int i = 1; i <= len; i++) {
    if (i > 1) {
      PSViewToken *token1 = new PSViewIntegerToken(getPSView(), str[i - 2]);
      PSViewToken *token2 = new PSViewIntegerToken(getPSView(), str[i - 1]);

      getPSView()->getOperandStack()->push(token1);
      getPSView()->getOperandStack()->push(token2);

      proc->execute();

      delete token1;
      delete token2;
    }

    if      (type == PSVIEW_FONT_TYPE_DEFAULT)
      showDefaultChar(str[i - 1], &p.x, &p.y);
    else if (type == PSVIEW_FONT_TYPE_1)
      showDefaultChar(str[i - 1], &p.x, &p.y);
    else if (type == PSVIEW_FONT_TYPE_3)
      showType3Char  (str[i - 1], &p.x, &p.y);
  }

  //------

  double x1, y1;

  multiplyByInverseCTMMatrix(p.x, p.y, &x1, &y1);

  moveTo(x1, y1);
}

void
PSViewGState::
stringWidth(const string &str, double *wx, double *wy)
{
  PSViewGStateFont *font = getFont();

  PSViewFontType type = font->getType();

  if      (type == PSVIEW_FONT_TYPE_DEFAULT) {
  }
  else if (type == PSVIEW_FONT_TYPE_0) {
  }
  else if (type == PSVIEW_FONT_TYPE_1)
    CStrUtil::eprintf("PSView: %s not implemented for Type 1 Fonts\n", "StringWidth");
  else if (type == PSVIEW_FONT_TYPE_3) {
  }

  //------

  if (type == PSVIEW_FONT_TYPE_0) {
    PSViewDictionaryToken *font_dictionary = font->getDictionary();

    string str1 = " ";

    int i = 0;

    int len = str.size();

    while (i < len) {
      PSViewDictionaryToken *font_dictionary1 = NULL;

      str1[0] = getType0Char(str, &i, &font_dictionary1);

      setFont1(font_dictionary1);

      stringWidth(str1, wx, wy);

      setFont1(font_dictionary);
    }

    return;
  }

  //------

  *wx = 0;
  *wy = 0;

  int len = str.size();

  for (int i = 1; i <= len; i++) {
    if      (type == PSVIEW_FONT_TYPE_DEFAULT)
      showDefaultChar(str[i - 1], wx, wy);
    else if (type == PSVIEW_FONT_TYPE_1)
      showDefaultChar(str[i - 1], wx, wy);
    else if (type == PSVIEW_FONT_TYPE_3)
      showType3Char  (str[i - 1], wx, wy);
  }
}

void
PSViewGState::
showDefaultChar(int c, double *x, double *y)
{
  PSViewGStateFont *font = getFont();

  PSViewGStateFont3Data *font_data = font->getFont3Data();

  PSViewPathRenderer renderer(graphics_);

  CMatrix2D m = ctm_matrix_*font_data->matrix;

  path_->addChar(c, m, x, y);

  path_->process(renderer);

  brush_.setFillRule(FILL_TYPE_WINDING);

  setGraphics();

  graphics_->pathFill();

  newPath();
}

void
PSViewGState::
defaultCharPath(int c, double *x, double *y)
{
  PSViewGStateFont *font = getFont();

  PSViewGStateFont3Data *font_data = font->getFont3Data();

  PSViewPathRenderer renderer(graphics_);

  CMatrix2D m = ctm_matrix_*font_data->matrix;

  path_->addChar(c, m, x, y);
}

int
PSViewGState::
getType0Char(const string &str, int *pos, PSViewDictionaryToken **font_dictionary)
{
  PSViewGStateFont *font = getFont();

  PSViewGStateFont0Data *font_data = font->getFont0Data();

  //------

  if (*font_dictionary == NULL)
    *font_dictionary = font_data->font_array[0];

  if      (font_data->map_type == 2) {
    int font_num = font_data->encoding[str[*pos] % font_data->encoding_size];

    (*pos)++;

    *font_dictionary = font_data->font_array[font_num % font_data->num_fonts];
  }
  else if (font_data->map_type == 3) {
    if (str[*pos] == font_data->escape_char) {
      (*pos)++;

      int font_num = font_data->encoding[str[*pos] % font_data->encoding_size];

      *font_dictionary = font_data->font_array[font_num % font_data->num_fonts];

      (*pos)++;
    }
  }
  else
    CStrUtil::eprintf("PSView: MapType %d not supported for Composite Fonts", font_data->map_type);

  return str[(*pos)++];
}

void
PSViewGState::
showType3Char(int c, double *x, double *y)
{
  PSViewGStateFont *font = getFont();

  PSViewGStateFont3Data *font_data = font->getFont3Data();

  //------

  getPSView()->getOperandStack()->push(font->getDictionary());

  //------

  c = ((uint) c) & 0xFF;

  PSViewToken *show_char;

  if (font_data->build_type == PSVIEW_FONT_BUILD_TYPE_GLYPH) {
    PSViewArrayToken *array_token = (PSViewArrayToken *) font_data->encoding;

    show_char = array_token->getValue(c + 1);
  }
  else
    show_char = new PSViewIntegerToken(getPSView(), c);

  getPSView()->getOperandStack()->push(show_char);

  //------

  getPSView()->getGStateTokenMgr()->save();

  newPath();

  double x1, y1, x2, y2;

  CMatrix2D *matrix = getCTMMatrix();

  matrix->getValues(&x1, &y1, &x2, &y2, NULL, NULL);
  matrix->setValues( x1,  y1,  x2,  y2,    0,    0);

  CMatrix2D tmatrix;

  tmatrix.setTranslation(*x, *y);

  preMultiplyCTMMatrix(&tmatrix);

  postMultiplyCTMMatrix(&font_data->matrix);

  setCharWidth(0.0, 0.0);

  font_data->execute();

  getCharWidth(&x1, &y1);

  getPSView()->getGStateTokenMgr()->restore();

  //------

  *x += x1;
  *y += y1;
}

void
PSViewGState::
charPath(const string &str, int flag)
{
  PSViewGStateFont *font = getFont();

  PSViewFontType type = font->getType();

  if      (type == PSVIEW_FONT_TYPE_DEFAULT) {
    CPoint2D p;

    path_->getCurrentPoint(p);

    int len = str.size();

    for (int i = 1; i <= len; i++)
      defaultCharPath(str[i - 1], &p.x, &p.y);

    return;
  }
  else if (type == PSVIEW_FONT_TYPE_0) {
    CStrUtil::eprintf("PSView: CharPath not implemented for Type 0 Fonts\n");
    return;
  }
  else if (type == PSVIEW_FONT_TYPE_1) {
    CStrUtil::eprintf("PSView: CharPath not implemented for Type 1 Fonts\n");
    return;
  }
  else if (type == PSVIEW_FONT_TYPE_3) {
  }

  //------

  PSViewToken *key1 = new PSViewNameToken(getPSView(), "fill");
  PSViewToken *key2 = new PSViewNameToken(getPSView(), "stroke");

  PSViewDictionaryToken *dict1 = getPSView()->getDictionaryMgr()->lookupDictionary(key1);
  PSViewDictionaryToken *dict2 = getPSView()->getDictionaryMgr()->lookupDictionary(key2);

  PSViewToken *value1 = getPSView()->getDictionaryMgr()->lookup(key1);
  PSViewToken *value2 = getPSView()->getDictionaryMgr()->lookup(key2);

  PSViewToken *value = new PSViewOperatorToken(getPSView(), "charpathfill");

  dict1->addValue(key1, value);

  if (flag)
    value = new PSViewOperatorToken(getPSView(), "charpathstroketrue");
  else
    value = new PSViewOperatorToken(getPSView(), "charpathstrokefalse");

  dict2->addValue(key2, value);

  //------

  CPoint2D p;

  bool flag1 = path_->getCurrentPoint(p);

  if (! flag1) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_NO_CURRENT_POINT);

    return;
  }

  //------

  int len = str.size();

  for (int i = 1; i <= len; i++)
    addType3CharPath(str[i - 1], &p.x, &p.y);

  //------

  double x1, y1;

  multiplyByInverseCTMMatrix(p.x, p.y, &x1, &y1);

  moveTo(x1, y1);

  //------

  dict1->addValue(key1, value1);
  dict2->addValue(key2, value2);
}

void
PSViewGState::
addType3CharPath(int c, double *x, double *y)
{
  PSViewGStateFont *font = getFont();

  PSViewGStateFont3Data *font_data = font->getFont3Data();

  //------

  getPSView()->getOperandStack()->push(font->getDictionary());

  //------

  c = ((uint) c) & 0xFF;

  PSViewToken *show_char;

  if (font_data->build_type == PSVIEW_FONT_BUILD_TYPE_GLYPH) {
    PSViewArrayToken *array_token = (PSViewArrayToken *) font_data->encoding;

    show_char = array_token->getValue(c + 1);
  }
  else
    show_char = new PSViewIntegerToken(getPSView(), c);

  getPSView()->getOperandStack()->push(show_char);

  //------

  getPSView()->getGStateTokenMgr()->save();

  newPath();

  double x1, y1, x2, y2;

  CMatrix2D *matrix = getCTMMatrix();

  matrix->getValues(&x1, &y1, &x2, &y2, NULL, NULL);
  matrix->setValues( x1,  y1,  x2,  y2,    0,    0);

  CMatrix2D tmatrix;

  tmatrix.setTranslation(*x, *y);

  preMultiplyCTMMatrix(&tmatrix);

  postMultiplyCTMMatrix(&font_data->matrix);

  setCharWidth(0.0, 0.0);

  font_data->execute();

  getCharWidth(&x1, &y1);

  getPSView()->getGStateTokenMgr()->restore();

  newPath();

  //------

  *x += x1;
  *y += y1;
}

void
PSViewGState::
image(char *image_data, int width, int height, int bits_per_sample, CMatrix2D *matrix,
      double *decode_array, int decode_size)
{
#ifdef DEBUG
  debugPrintImage(image_data, width, height, bits_per_sample, matrix, decode_array, decode_size);
#endif

  CMatrix2D imatrix;

  bool flag = matrix->invert(imatrix);

  if (! flag) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED_RESULT);
    return;
  }

  if (graphics_ == NULL) return;

  int num_colors = 1 << bits_per_sample;

  int bit_num  = 7;
  int byte_num = 0;

  CRGBA rgba(1,1,1);

  double y = 0.0;

  for (int i = 0; i < height; ++i) {
    double x = 0.0;

    int bit_mask = 1 << bit_num;

    for (int j = 0; j < width; ++j) {
      int color_bit = bits_per_sample - 1;

      int color = 0;

      for (int k = 0; k < bits_per_sample; ++k) {
        if (image_data[byte_num] & bit_mask)
          color |= 1 << color_bit;

        bit_num--;

        if (bit_num < 0) {
          bit_num = 7;

          ++byte_num;
        }

        bit_mask = 1 << bit_num;
      }

      if (color < decode_size)
        rgba.setGray(decode_array[color]);
      else
        rgba.setGray(((double) color)/(num_colors - 1));

      double x11, y11, x21, y21;

      imatrix.multiplyPoint(x    , y    , &x11, &y11);
      imatrix.multiplyPoint(x + 1, y + 1, &x21, &y21);

      double x12, y12, x22, y22;

      ctm_matrix_.multiplyPoint(x11, y11, &x12, &y12);
      ctm_matrix_.multiplyPoint(x21, y21, &x22, &y22);

      CPoint2D points[4];

      points[0].x = x12; points[0].y = y12;
      points[1].x = x22; points[1].y = y12;
      points[2].x = x22; points[2].y = y22;
      points[3].x = x12; points[3].y = y22;

      pen_  .setColor(CRGBA(0,0,0,0));
      brush_.setColor(rgba);

      path_->init();

      path_->moveTo(points[0]);
      path_->lineTo(points[1]);
      path_->lineTo(points[2]);
      path_->lineTo(points[3]);

      path_->close();

      PSViewPathRenderer renderer(graphics_);

      path_->process(renderer);

      brush_.setFillRule(FILL_TYPE_WINDING);

      setGraphics();

      graphics_->pathFill();

      x += 1.0;
    }

    if (bit_num != 7) {
      bit_num = 7;

      ++byte_num;
    }

    y += 1.0;
  }
}

void
PSViewGState::
debugPrintImage(char *image_data, int width, int height, int bits_per_sample,
                CMatrix2D *, double *, int)
{
  CStrUtil::printf("Image\n");
  CStrUtil::printf("  Width  = %d\n", width);
  CStrUtil::printf("  Height = %d\n", height);
  CStrUtil::printf("  Bits   = %d\n", bits_per_sample);
  CStrUtil::printf("  Data :-\n");

  int width1 = width;

  if ((width % 8) != 0)
    width1 += 8 - (width % 8);

  int num_bits  = width1*height*bits_per_sample;
  int num_bytes = num_bits/8;

  int i = 0;

  for ( ; i < num_bytes; ++i) {
    if (i % 8 == 0)
      CStrUtil::printf("    ");
    else
      CStrUtil::printf(", ");

    CStrUtil::printf("%x", image_data[i] & 0xFF);

    if (i % 8 == 7)
      CStrUtil::printf("\n");
  }

  if (i % 8 != 0)
    CStrUtil::printf("\n");
}

void
PSViewGState::
imageMask(char *image_data, int width, int height, int polarity, CMatrix2D *matrix)
{
#ifdef DEBUG
  debugPrintImageMask(image_data, width, height, polarity, matrix);
#endif

  CMatrix2D imatrix;

  bool flag = matrix->invert(imatrix);

  if (! flag) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED_RESULT);
    return;
  }

  if (graphics_ == NULL)
    return;

  int bit_num  = 7;
  int byte_num = 0;

  double y = 0.0;

  for (int i = 0; i < height; ++i) {
    double x = 0.0;

    int bit_mask = 1 << bit_num;

    for (int j = 0; j < width; ++j) {
      int color = 0;

      if (image_data[byte_num] & bit_mask)
        color = 1;

      bit_num--;

      if (bit_num < 0) {
        bit_num = 7;

        ++byte_num;
      }

      bit_mask = 1 << bit_num;

      if      ((color == 0 && ! polarity) ||
               (color == 1 &&   polarity)) {
        double x11, y11, x21, y21;

        imatrix.multiplyPoint(x    , y    , &x11, &y11);
        imatrix.multiplyPoint(x + 1, y + 1, &x21, &y21);

        double x12, y12, x22, y22;

        ctm_matrix_.multiplyPoint(x11, y11, &x12, &y12);
        ctm_matrix_.multiplyPoint(x21, y21, &x22, &y22);

        CPoint2D points[4];

        points[0].x = x12; points[0].y = y12;
        points[1].x = x22; points[1].y = y12;
        points[2].x = x22; points[2].y = y22;
        points[3].x = x12; points[3].y = y22;

        path_->init();

        path_->moveTo(points[0]);
        path_->lineTo(points[1]);
        path_->lineTo(points[2]);
        path_->lineTo(points[3]);

        path_->close();

        PSViewPathRenderer renderer(graphics_);

        path_->process(renderer);

        brush_.setFillRule(FILL_TYPE_WINDING);

        setGraphics();

        graphics_->pathFill();
      }

      x += 1.0;
    }

    if (bit_num != 7) {
      bit_num = 7;

      ++byte_num;
    }

    y += 1.0;
  }
}

void
PSViewGState::
debugPrintImageMask(char *image_data, int width, int height, int polarity, CMatrix2D *)
{
  CStrUtil::printf("ImageMask\n");
  CStrUtil::printf("  Width    = %d\n", width);
  CStrUtil::printf("  Height   = %d\n", height);
  CStrUtil::printf("  Polarity = %d\n", polarity);
  CStrUtil::printf("  Data :-\n");

  int width1 = width;

  if ((width % 8) != 0)
    width1 += 8 - (width % 8);

  int num_bits  = width1*height;
  int num_bytes = num_bits/8;

  int i = 0;

  for ( ; i < num_bytes; ++i) {
    if (i % 8 == 0)
      CStrUtil::printf("    ");
    else
      CStrUtil::printf(", ");

    CStrUtil::printf("%x", image_data[i] & 0xFF);

    if (i % 8 == 7)
      CStrUtil::printf("\n");
  }

  if (i % 8 != 0)
    CStrUtil::printf("\n");
}

void
PSViewGState::
closePath()
{
  path_->close();
}

void
PSViewGState::
rectStroke(double x, double y, double width, double height, CMatrix2D *matrix)
{
  getPSView()->getGStateTokenMgr()->save();

  newPath();

  moveTo ( x    , y     );
  rlineTo( width, 0.0   );
  rlineTo( 0.0  , height);
  rlineTo(-width, 0.0   );

  closePath();

  if (matrix != NULL)
    postMultiplyCTMMatrix(matrix);

  stroke();

  getPSView()->getGStateTokenMgr()->restore();
}

void
PSViewGState::
rectFill(double x, double y, double width, double height)
{
  getPSView()->getGStateTokenMgr()->save();

  newPath();

  moveTo ( x    , y     );
  rlineTo( width, 0.0   );
  rlineTo( 0.0  , height);
  rlineTo(-width, 0.0   );

  closePath();

  fill();

  getPSView()->getGStateTokenMgr()->restore();
}

void
PSViewGState::
rectClip(double *x, double *y, double *width, double *height, int num_rects)
{
  newPath();

  for (int i = 0; i < num_rects; i++) {
    moveTo ( x[i]    , y[i]     );
    rlineTo( width[i], 0.0      );
    rlineTo( 0.0     , height[i]);
    rlineTo(-width[i], 0.0      );

    closePath();
  }

  clip();

  newPath();
}

void
PSViewGState::
stroke()
{
  if (getPSView()->getDebug()) {
    CStrUtil::printf("PSViewGState::stroke\n");

    path_->print();
  }

  path_->setStrokeAdjust(stroke_adjust_);

  PSViewPathRenderer renderer(graphics_);

  path_->process(renderer);

  setGraphics();

  graphics_->pathStroke();

  newPath();
}

void
PSViewGState::
strokePath()
{
  if (getPSView()->getDebug()) {
    CStrUtil::printf("PSViewGState::strokePath\n");

    path_->print();
  }

  PSViewPathStroker stroker(this);

  path_->process(stroker);

  path_ = stroker.getPath();
}

void
PSViewGState::
fill()
{
  if (getPSView()->getDebug()) {
    CStrUtil::printf("PSViewGState::fill\n");

    path_->print();
  }

  if (pattern_color_ && pattern_.isValid())
    patternFill();
  else {
    PSViewPathRenderer renderer(graphics_);

    path_->process(renderer);

    brush_.setFillRule(FILL_TYPE_WINDING);

    setGraphics();

    graphics_->pathFill();
  }

  newPath();
}

void
PSViewGState::
patternFill()
{
  double xmin, ymin, xmax, ymax;

  path_->getBounds(&xmin, &ymin, &xmax, &ymax);

  double x_step = pattern_->getXStep();
  double y_step = pattern_->getYStep();

  double x1, y1, x2, y2;

  CMatrix2D *matrix = getCTMMatrix();

  matrix->multiplyPoint(0.0   , 0.0   , &x1, &y1);
  matrix->multiplyPoint(x_step, y_step, &x2, &y2);

  x_step = x2 - x1;
  y_step = y2 - y1;

  //------

  x1 = 0.0;

  if (x1 < xmin) {
    while (x1 < xmin)
      x1 += x_step;

    x1 -= x_step;
  }
  else {
    while (x1 > xmin)
      x1 -= x_step;
  }

  xmin = x1;

  y1 = 0.0;

  if (y1 < ymin) {
    while (y1 < ymin)
      y1 += y_step;

    y1 -= y_step;
  }
  else {
    while (y1 > ymin)
      y1 -= y_step;
  }

  ymin = y1;

  /****************/

  PSViewGStateTokenMgr *mgr = getPSView()->getGStateTokenMgr();

  PSViewOperandStack *opStack = getPSView()->getOperandStack();

  for (double x = xmin; x < xmax; x += x_step) {
    for (double y = ymin; y < ymax; y += y_step) {
      PSViewGStateToken *gstate_token = mgr->save();

      gstate_token->clip();

      gstate_token->newPath();

      CMatrix2D tmatrix;

      tmatrix.setTranslation(x, y);

      gstate_token->preMultiplyCTMMatrix(&tmatrix);

      PSViewGStatePattern *pattern = gstate_token->getPattern();

      const double *bbox = pattern->getBBox();

      gstate_token->moveTo(bbox[0], bbox[1]);
      gstate_token->lineTo(bbox[2], bbox[1]);
      gstate_token->lineTo(bbox[2], bbox[3]);
      gstate_token->lineTo(bbox[0], bbox[3]);

      gstate_token->closePath();

      gstate_token->clip();

      gstate_token->newPath();

      opStack->push(pattern->getDictionary());

      pattern->execute();

      mgr->restore();
    }
  }
}

void
PSViewGState::
eofill()
{
  if (getPSView()->getDebug()) {
    CStrUtil::printf("PSViewGState::eofill\n");

    path_->print();
  }

  PSViewPathRenderer renderer(graphics_);

  path_->process(renderer);

  brush_.setFillRule(FILL_TYPE_EVEN_ODD);

  setGraphics();

  graphics_->pathFill();

  newPath();
}

void
PSViewGState::
clip()
{
  if (getPSView()->getDebug()) {
    CStrUtil::printf("PSViewGState::clip\n");

    path_->print();
  }

  *clippath_ = *path_;

  PSViewPathRenderer renderer(graphics_);

  path_->process(renderer);

  brush_.setFillRule(FILL_TYPE_WINDING);

  setGraphics();

  graphics_->pathClip();
}

void
PSViewGState::
eoclip()
{
  if (getPSView()->getDebug()) {
    CStrUtil::printf("PSViewGState::eoclip\n");

    path_->print();
  }

  *clippath_ = *path_;

  PSViewPathRenderer renderer(graphics_);

  path_->process(renderer);

  brush_.setFillRule(FILL_TYPE_EVEN_ODD);

  setGraphics();

  graphics_->pathClip();
}

void
PSViewGState::
clipPath()
{
  *path_ = *clippath_;

  if (getPSView()->getDebug()) {
    CStrUtil::printf("PSViewGState::clipPath\n");

    path_->print();
  }
}

void
PSViewGState::
flattenPath()
{
  if (getPSView()->getDebug()) {
    CStrUtil::printf("PSViewGState::flattenPath\n");

    path_->print();
  }

  path_->flatten();

  if (getPSView()->getDebug()) {
    CStrUtil::printf("PSViewGState::flattenPath\n");

    path_->print();
  }
}

void
PSViewGState::
reversePath()
{
  if (getPSView()->getDebug()) {
    CStrUtil::printf("PSViewGState::reversePath\n");

    path_->print();
  }

  path_->reverse();

  if (getPSView()->getDebug()) {
    CStrUtil::printf("PSViewGState::reversePath\n");

    path_->print();
  }
}

void
PSViewGState::
pathBBox(double *llx, double *lly, double *urx, double *ury)
{
  double x1, y1, x2, y2;

  path_->getBounds(&x1, &y1, &x2, &y2);

  multiplyByInverseCTMMatrix(x1, y1, llx, lly);
  multiplyByInverseCTMMatrix(x2, y2, urx, ury);

  newPath();
}

void
PSViewGState::
pathForAll(PSViewToken *move_to_token, PSViewToken *line_to_token,
           PSViewToken *curve_to_token, PSViewToken *close_path_token)
{
  if (getPSView()->getDebug()) {
    CStrUtil::printf("PSViewGState::pathForAll\n");

    path_->print();
  }

  PSViewTokenPathVisitor visitor(getPSView(), move_to_token, line_to_token,
                                 curve_to_token, close_path_token);

  path_->process(visitor);
}

void
PSViewGState::
showPage()
{
  // if (graphics_ != NULL)
  //   graphics_->showPage();
}

void
PSViewGState::
erasePage()
{
  if (graphics_ != NULL)
    graphics_->clear(CRGBA(1,1,1));
}

void
PSViewGState::
setCacheDevice(double wx, double wy, double llx, double lly, double urx, double ury)
{
  PSViewGStateFont *font = getFont();

  PSViewGStateFont3Data *font_data = font->getFont3Data();

  font_data->cache_device_wx  = wx;
  font_data->cache_device_wy  = wy;
  font_data->cache_device_llx = llx;
  font_data->cache_device_lly = lly;
  font_data->cache_device_urx = urx;
  font_data->cache_device_ury = ury;
}

void
PSViewGState::
setCharWidth(double wx, double wy)
{
  PSViewGStateFont *font = getFont();

  PSViewGStateFont3Data *font_data = font->getFont3Data();

  font_data->cache_device_wx = wx;
  font_data->cache_device_wy = wy;
}

void
PSViewGState::
getCharWidth(double *wx, double *wy)
{
  double cx, cy;

  getCharWidth1(&cx, &cy);

  double x1, y1;

  multiplyDistByCTMMatrix(cx, cy, &x1, &y1);

  *wx = x1;
  *wy = y1;
}

void
PSViewGState::
getCharWidth1(double *wx, double *wy)
{
  PSViewGStateFont *font = getFont();

  PSViewGStateFont3Data *font_data = font->getFont3Data();

  *wx = font_data->cache_device_wx;
  *wy = font_data->cache_device_wy;
}

void
PSViewGState::
defineFont(PSViewToken *key, PSViewDictionaryToken *dictionary)
{
  PSViewToken *token = dictionary->getValue("FontType");

  if (token == NULL || ! token->isInteger()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);

    return;
  }

  PSVinteger type = token->getIntegerValue();

  if (type != PSVIEW_FONT_TYPE_DEFAULT &&
      type != PSVIEW_FONT_TYPE_0       &&
      type != PSVIEW_FONT_TYPE_1       &&
      type != PSVIEW_FONT_TYPE_3) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_LIMIT_CHECK);

    return;
  }

  if      (type == PSVIEW_FONT_TYPE_0)
    defineType0Font(key, dictionary);
  else if (type == PSVIEW_FONT_TYPE_1)
    CStrUtil::eprintf("PSView: Type 1 Fonts not Implemented\n");
  else if (type == PSVIEW_FONT_TYPE_3)
    defineType3Font(key, dictionary);
}

void
PSViewGState::
defineType0Font(PSViewToken *key, PSViewDictionaryToken *dictionary)
{
  PSViewToken *token = dictionary->getValue("FontMatrix");

  if (token == NULL || ! token->isMatrix()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
    return;
  }

  //------

  token = dictionary->getValue("FMapType");

  if (token == NULL || ! token->isInteger()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
    return;
  }

  //------

  token = dictionary->getValue("Encoding");

  if (token == NULL || ! token->isArray()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
    return;
  }

  PSViewArrayToken *array_token = (PSViewArrayToken *) token;

  PSVinteger encoding_size = array_token->getNumValues();

  for (int i = 1; i <= encoding_size; i++) {
    PSViewToken *sub_token = array_token->getValue(i);

    if (! sub_token->isInteger()) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
      return;
    }
  }

  //------

  token = dictionary->getValue("FDepVector");

  if (token == NULL || ! token->isArray()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
    return;
  }

  array_token = (PSViewArrayToken *) token;

  PSVinteger num_fonts = array_token->getNumValues();

  for (int i = 1; i <= num_fonts; i++) {
    PSViewToken *sub_token = array_token->getValue(i);

    if (! sub_token->isDictionary()) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
      return;
    }
  }

  //------

  PSViewToken *key1 = new PSViewNameToken(getPSView(), "FID");

  PSViewToken *value1 = new PSViewFontIdToken(getPSView(), mgr_->getNextFontId());

  dictionary->addValue(key1, value1);

  //------

  token = getPSView()->getDictionaryMgr()->lookup("FontDirectory");

  if (token == NULL || ! token->isDictionary())
    return;

  PSViewDictionaryToken *dictionary_token = (PSViewDictionaryToken *) token;

  dictionary_token->addValue(key, dictionary);

  //------

  getPSView()->getOperandStack()->push(value1);

  return;
}

void
PSViewGState::
defineType3Font(PSViewToken *key, PSViewDictionaryToken *dictionary)
{
  PSViewToken *token = dictionary->getValue("FontMatrix");

  if (token == NULL || ! token->isMatrix()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
    return;
  }

  //------

  token = dictionary->getValue("Encoding");

  if (token == NULL || ! token->isArray()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
    return;
  }

  PSViewArrayToken *array_token = (PSViewArrayToken *) token;

  PSVinteger num_tokens = array_token->getNumValues();

  if (num_tokens != 256) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
    return;
  }

  for (int i = 1; i <= 256; i++) {
    PSViewToken *sub_token = array_token->getValue(i);

    if (! sub_token->isName()) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
      return;
    }
  }

  //------

  token = dictionary->getValue("FontBBox");

  if (token == NULL || ! token->isArray()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
    return;
  }

  array_token = (PSViewArrayToken *) token;

  num_tokens = array_token->getNumValues();

  if (num_tokens != 4) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
    return;
  }

  for (int i = 1; i <= 4; i++) {
    PSViewToken *sub_token = array_token->getValue(i);

    if (! sub_token->isNumber()) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
      return;
    }
  }

  //------

  token = dictionary->getValue("BuildChar");

  if (token != NULL && ! token->isProcedure()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
    return;
  }

  //------

  token = dictionary->getValue("BuildGlyph");

  if (token != NULL && ! token->isProcedure()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
    return;
  }

  //------

  PSViewToken *key1 = new PSViewNameToken(getPSView(), "FID");

  PSViewToken *value1 = new PSViewFontIdToken(getPSView(), mgr_->getNextFontId());

  dictionary->addValue(key1, value1);

  //------

  token = getPSView()->getDictionaryMgr()->lookup("FontDirectory");

  if (token == NULL || ! token->isDictionary()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
    return;
  }

  PSViewDictionaryToken *dictionary_token = (PSViewDictionaryToken *) token;

  dictionary_token->addValue(key, dictionary);

  //------

  getPSView()->getOperandStack()->push(value1);

  return;
}

void
PSViewGState::
undefineFont(PSViewToken *key)
{
  PSViewToken *token = getPSView()->getDictionaryMgr()->lookup("FontDirectory");

  if (token == NULL || ! token->isDictionary())
    return;

  PSViewDictionaryToken *dictionary = (PSViewDictionaryToken *) token;

  dictionary->addValue(key, NULL);
}

PSViewDictionaryToken *
PSViewGState::
findFont(PSViewToken *key)
{
  PSViewToken *token = getPSView()->getDictionaryMgr()->lookup("FontDirectory");

  if (token == NULL || ! token->isDictionary())
    return mgr_->getDefaultFont();

  PSViewDictionaryToken *dictionary = (PSViewDictionaryToken *) token;

  token = dictionary->getValue(key);

  PSViewDictionaryToken *font;

  if (token != NULL && token->isDictionary())
    font = (PSViewDictionaryToken *) token;
  else
    font = readFont(key);

  return font;
}

PSViewDictionaryToken *
PSViewGState::
readFont(PSViewToken *key)
{
  if (! key->isName())
    return mgr_->getDefaultFont();

  PSViewToken *key1 = NULL;

  PSViewNameToken *name_token = (PSViewNameToken *) key;

  const PSViewName &name = name_token->getValue();

  string name1 = name.getString();

  bool file_found = false;

  string font_dir;

  if (! CEnvInst.get("PSVIEW_FONT_DIR", font_dir))
    font_dir = COSUser::getUserHome() + "/data/PSView/fonts";

  char font_filename[256];

  if (font_dir[font_dir.size() - 1] == '/')
    sprintf(font_filename, "%s%s.font", font_dir.c_str(), name1.c_str());
  else
    sprintf(font_filename, "%s/%s.font", font_dir.c_str(), name1.c_str());

  struct stat file_stat;

  if (stat(font_filename, &file_stat) != -1)
    file_found = true;

  if (! file_found) {
    if (font_dir[font_dir.size() - 1] == '/')
      sprintf(font_filename, "%s%s.font", font_dir.c_str(), name1.c_str());
    else
      sprintf(font_filename, "%s/%s.font", font_dir.c_str(), name1.c_str());

    struct stat file_stat;

    if (stat(font_filename, &file_stat) != -1)
      file_found = true;
  }

  if (! file_found) {
    sprintf(font_filename, "%s.font", name1.c_str());

    struct stat file_stat;

    if (stat(font_filename, &file_stat) != -1)
      file_found = true;
  }

  if (! file_found) {
    if (CStrUtil::casefind(name1, "Bold") != string::npos) {
      if (CStrUtil::casefind(name1, "Italic" ) != string::npos ||
          CStrUtil::casefind(name1, "Oblique") != string::npos)
        name1 = "Helvetica-BoldOblique";
      else
        name1 = "Helvetica-Bold";
    }
    else {
      if (CStrUtil::casefind(name1, "Italic" ) != string::npos ||
          CStrUtil::casefind(name1, "Oblique") != string::npos)
        name1 = "Helvetica-Oblique";
      else
        name1 = "Helvetica";
    }

    key1 = key;
    key  = new PSViewNameToken(getPSView(), name1);

    if (font_dir[font_dir.size() - 1] == '/')
      sprintf(font_filename, "%s%s.font", font_dir.c_str(), name1.c_str());
    else
      sprintf(font_filename, "%s/%s.font", font_dir.c_str(), name1.c_str());

    struct stat file_stat;

    if (stat(font_filename, &file_stat) != -1)
      file_found = true;

    if (! file_found) {
      if (font_dir[font_dir.size() - 1] == '/')
        sprintf(font_filename, "%s%s.font", font_dir.c_str(), name1.c_str());
      else
        sprintf(font_filename, "%s/%s.font", font_dir.c_str(), name1.c_str());

      struct stat file_stat;

      if (stat(font_filename, &file_stat) != -1)
        file_found = true;
    }

    if (! file_found) {
      sprintf(font_filename, "%s.font", name1.c_str());

      struct stat file_stat;

      if (stat(font_filename, &file_stat) != -1)
        file_found = true;
    }
  }

  if (! file_found)
    return mgr_->getDefaultFont();

  getPSView()->openCurrentFile(font_filename);

  getPSView()->executeCurrentFile();

  getPSView()->closeCurrentFile();

  PSViewToken *token = getPSView()->getDictionaryMgr()->lookup("FontDirectory");

  if (token == NULL || ! token->isDictionary())
    return mgr_->getDefaultFont();

  PSViewDictionaryToken *dictionary = (PSViewDictionaryToken *) token;

  token = dictionary->getValue(key);

  if (token == NULL || ! token->isDictionary())
    return mgr_->getDefaultFont();

  if (key1 != NULL)
    dictionary->addValue(key1, token);

  PSViewDictionaryToken *font = (PSViewDictionaryToken *) token;

  return font;
}

void
PSViewGState::
setFont(PSViewDictionaryToken *font_dictionary)
{
  setFont1(font_dictionary);
}

void
PSViewGState::
setFont1(PSViewDictionaryToken *font_dictionary)
{
  PSViewToken *token = font_dictionary->getValue("FontType");

  if (token == NULL || ! token->isInteger()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
    return;
  }

  PSVinteger type = token->getIntegerValue();

  if (type != PSVIEW_FONT_TYPE_DEFAULT &&
      type != PSVIEW_FONT_TYPE_0       &&
      type != PSVIEW_FONT_TYPE_1       &&
      type != PSVIEW_FONT_TYPE_3) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_LIMIT_CHECK);
    return;
  }

  //------

  token = font_dictionary->getValue("FontMatrix");

  if (token == NULL || ! token->isMatrix()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
    return;
  }

  CAutoPtr<CMatrix2D> matrix;

  matrix = token->getMatrix();

  //------

  PSViewGStateFont *font = getFont();

  if      (type == PSVIEW_FONT_TYPE_DEFAULT) {
    font->setDefaultFont(font_dictionary, matrix);
  }
  else if (type == PSVIEW_FONT_TYPE_0) {
    PSViewToken *token1 = font_dictionary->getValue("Encoding");

    if (token1 == NULL || ! token1->isArray()) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
      return;
    }

    PSViewArrayToken *array_token = (PSViewArrayToken *) token1;

    PSVinteger encoding_size = array_token->getNumValues();

    if (encoding_size == 0) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
      return;
    }

    vector<int> encoding;

    encoding.resize(encoding_size);

    for (int i = 1; i <= encoding_size; i++) {
      PSViewToken *sub_token = array_token->getValue(i);

      if (! sub_token->isInteger()) {
        getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
        return;
      }

      encoding[i - 1] = sub_token->getIntegerValue();
    }

    //------

    token1 = font_dictionary->getValue("FMapType");

    if (token1 == NULL || ! token1->isInteger()) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
      return;
    }

    PSVinteger map_type = token1->getIntegerValue();

    //------

    token1 = font_dictionary->getValue("EscChar");

    if (token1 != NULL && ! token1->isInteger()) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
      return;
    }

    PSVinteger escape_char = 255;

    if (token1 != NULL)
      escape_char = token1->getIntegerValue();

    //------

    token1 = font_dictionary->getValue("FDepVector");

    if (token1 == NULL || ! token1->isArray()) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
      return;
    }

    array_token = (PSViewArrayToken *) token1;

    PSVinteger num_fonts = array_token->getNumValues();

    if (num_fonts == 0) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
      return;
    }

    vector<PSViewDictionaryToken *> font_array;

    font_array.resize(num_fonts);

    for (int i = 1; i <= num_fonts; i++) {
      PSViewToken *sub_token = array_token->getValue(i);

      if (! sub_token->isDictionary()) {
        getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
        return;
      }

      font_array[i - 1] = (PSViewDictionaryToken *) sub_token;
    }

    //------

    font->setType1Font(font_dictionary, encoding_size, encoding,
                        map_type, escape_char, num_fonts, font_array);
  }
  else if (type == PSVIEW_FONT_TYPE_1)
    CStrUtil::eprintf("PSView: Type 1 Fonts not implemented\n");
  else if (type == PSVIEW_FONT_TYPE_3) {
    PSViewToken *encoding = font_dictionary->getValue("Encoding");

    if (encoding == NULL || ! encoding->isArray()) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
      return;
    }

    //------

    PSViewToken *build_glyph = font_dictionary->getValue("BuildGlyph");
    PSViewToken *build_char  = font_dictionary->getValue("BuildChar" );

    PSViewToken         *build;
    PSViewFontBuildType  build_type;

    if (build_glyph != NULL) {
      build      = build_glyph;
      build_type = PSVIEW_FONT_BUILD_TYPE_GLYPH;
    }
    else {
      build      = build_char;
      build_type = PSVIEW_FONT_BUILD_TYPE_CHAR;
    }

    if (build == NULL || ! build->isProcedure()) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
      return;
    }

    //------

    font->setType3Font(font_dictionary, matrix, encoding, build, build_type);
  }
}

PSViewDictionaryToken *
PSViewGState::
getCurrentFont()
{
  PSViewGStateFont *font = getFont();

  return font->getDictionary();
}

PSViewDictionaryToken *
PSViewGState::
getRootFont()
{
  PSViewGStateFont *font = getFont();

  PSViewDictionaryToken *root_font = font->getDictionary();

  if (root_font != NULL)
    return root_font;
  else
    return getCurrentFont();
}

void
PSViewGState::
selectScaleFont(PSViewToken *key, PSVreal scale)
{
  getPSView()->getOperandStack()->push(key);

  PSViewToken *findfont = getPSView()->getDictionaryMgr()->lookup("findfont");

  findfont->execute();

  PSViewToken *token = getPSView()->getOperandStack()->pop();

  PSViewDictionaryToken *font = (PSViewDictionaryToken *) token;

  PSViewDictionaryToken *font_dictionary1 = scaleFont(font, scale);

  setFont(font_dictionary1);
}

void
PSViewGState::
selectMakeFont(PSViewToken *key, CMatrix2D *matrix)
{
  getPSView()->getOperandStack()->push(key);

  PSViewToken *findfont = getPSView()->getDictionaryMgr()->lookup("findfont");

  findfont->execute();

  PSViewToken *token = getPSView()->getOperandStack()->pop();

  PSViewDictionaryToken *font = (PSViewDictionaryToken *) token;

  PSViewDictionaryToken *font_dictionary1 = makeFont(font, matrix);

  setFont(font_dictionary1);
}

PSViewDictionaryToken *
PSViewGState::
scaleFont(PSViewDictionaryToken *font_dictionary, PSVreal scale)
{
  PSViewToken *token = font_dictionary->getValue("FontType");

  if (token == NULL || ! token->isInteger()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
    return NULL;
  }

  PSVinteger type = token->getIntegerValue();

  if (type != PSVIEW_FONT_TYPE_DEFAULT &&
      type != PSVIEW_FONT_TYPE_0       &&
      type != PSVIEW_FONT_TYPE_1       &&
      type != PSVIEW_FONT_TYPE_3) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_LIMIT_CHECK);
    return NULL;
  }

  if (type == PSVIEW_FONT_TYPE_0) {
    PSViewToken *token = font_dictionary->getValue("FDepVector");

    if (token == NULL || ! token->isArray()) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
      return NULL;
    }

    PSViewArrayToken *array_token = (PSViewArrayToken *) token;

    int num_fonts = array_token->getNumValues();

    for (int i = 1; i <= num_fonts; i++) {
      PSViewToken *sub_token = array_token->getValue(i);

      if (! sub_token->isDictionary()) {
        getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
        return NULL;
      }

      PSViewDictionaryToken *dictionary = (PSViewDictionaryToken *) sub_token;

      PSViewDictionaryToken *font_dictionary1 = scaleFont(dictionary, scale);

      array_token->setValue(i, font_dictionary1);
    }

    return font_dictionary;
  }

  PSViewToken *token_m1 = font_dictionary->getValue("FontMatrix");

  if (token_m1 == NULL) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
    return NULL;
  }

  CAutoPtr<CMatrix2D> matrix;

  matrix = token_m1->getMatrix();

  CMatrix2D matrix1;

  matrix1.setScale(scale, scale);

  CMatrix2D matrix2 = matrix1 * (*matrix);

  PSViewToken *token_m2 = new PSViewArrayToken(getPSView(), &matrix2);

  //------

  PSVinteger size = font_dictionary->getMaxValues();

  PSViewDictionaryToken *font_dictionary1 =
    new PSViewDictionaryToken(getPSView()->getDictionaryMgr(), size);

  size = font_dictionary->getNumValues();

  for (int i = 1; i < size; i++) {
    PSViewKeyValue *keyval = font_dictionary->getKeyValue(i);

    font_dictionary1->addValue(keyval->getKey(), keyval->getValue());
  }

  //------

  PSViewToken *key = new PSViewNameToken(getPSView(), "FontMatrix");

  font_dictionary1->addValue(key, token_m2);

  return font_dictionary1;
}

PSViewDictionaryToken *
PSViewGState::
makeFont(PSViewDictionaryToken *font_dictionary, CMatrix2D *matrix)
{
  PSViewToken *token_m1 = font_dictionary->getValue("FontMatrix");

  if (token_m1 == NULL) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_FONT);
    return NULL;
  }

  CAutoPtr<CMatrix2D> matrix1;

  matrix1 = token_m1->getMatrix();

  CMatrix2D matrix2 = (*matrix) * (*matrix1);

  PSViewToken *token_m2 = new PSViewArrayToken(getPSView(), &matrix2);

  //------

  PSVinteger size = font_dictionary->getMaxValues();

  PSViewDictionaryToken *font_dictionary1 =
    new PSViewDictionaryToken(getPSView()->getDictionaryMgr(), size);

  size = font_dictionary->getNumValues();

  for (int i = 1; i < size; i++) {
    PSViewKeyValue *keyval = font_dictionary->getKeyValue(i);

    font_dictionary1->addValue(keyval->getKey(), keyval->getValue());
  }

  //------

  PSViewToken *key = new PSViewNameToken(getPSView(), "FontMatrix");

  font_dictionary1->addValue(key, token_m2);

  return font_dictionary1;
}

//------

PSViewDictionaryToken *
PSViewGState::
makePattern(PSViewDictionaryToken *pattern_dictionary, CMatrix2D *matrix)
{
  PSViewToken *token = pattern_dictionary->getValue("PatternType");

  if (token == NULL || ! token->isInteger()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return NULL;
  }

  PSVinteger pattern_type = token->getIntegerValue();

  if (pattern_type != 1) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return NULL;
  }

  //------

  token = pattern_dictionary->getValue("PaintType");

  if (token == NULL || ! token->isInteger()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return NULL;
  }

  PSVinteger paint_type = token->getIntegerValue();

  if (paint_type != 1 && paint_type != 2) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return NULL;
  }

  //------

  token = pattern_dictionary->getValue("TilingType");

  if (token == NULL || ! token->isInteger()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return NULL;
  }

  PSVinteger tiling_type = token->getIntegerValue();

  if (tiling_type != 1 && tiling_type != 2 && tiling_type != 3) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return NULL;
  }

  //------

  token = pattern_dictionary->getValue("BBox");

  if (token == NULL || ! token->isArray()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return NULL;
  }

  PSViewArrayToken *array_token = (PSViewArrayToken *) token;

  PSVinteger num_tokens = array_token->getNumValues();

  if (num_tokens != 4) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return NULL;
  }

  for (int i = 1; i <= 4; i++) {
    PSViewToken *sub_token = array_token->getValue(i);

    if (! sub_token->isNumber()) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
      return NULL;
    }
  }

  //------

  token = pattern_dictionary->getValue("XStep");

  if (token == NULL || ! token->isNumber()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return NULL;
  }

  PSVreal x_step = token->getRealValue();

  if (x_step == 0.0) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return NULL;
  }

  //------

  token = pattern_dictionary->getValue("YStep");

  if (token == NULL || ! token->isNumber()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return NULL;
  }

  PSVreal y_step = token->getRealValue();

  if (y_step == 0.0) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return NULL;
  }

  //------

  PSViewToken *paint_proc = pattern_dictionary->getValue("PaintProc");

  if (paint_proc == NULL || ! paint_proc->isProcedure()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return NULL;
  }

  //------

  PSVinteger size = pattern_dictionary->getMaxValues();

  PSViewDictionaryToken *pattern1 =
    new PSViewDictionaryToken(getPSView()->getDictionaryMgr(), size);

  size = pattern_dictionary->getNumValues();

  for (int i = 1; i <= size; i++) {
    PSViewKeyValue *keyval = pattern_dictionary->getKeyValue(i);

    pattern1->addValue(keyval->getKey(), keyval->getValue());
  }

  //------

  PSViewGStateToken *gstate_token =
    new PSViewGStateToken(*getPSView()->getGStateTokenMgr()->getCurrent());

  PSViewGState *gstate = gstate_token->getGState();

  CMatrix2D matrix1 = gstate->ctm_matrix_ * (*matrix);

  gstate->ctm_matrix_ = matrix1;

  gstate->newPath();

  gstate->initClip();

  //------

  PSViewToken *sub_token = new PSViewNameToken(getPSView(), "Implementation");

  pattern1->addValue(sub_token, gstate_token);

  return pattern1;
}

void
PSViewGState::
setPattern(PSViewDictionaryToken *pattern_dictionary)
{
  PSViewToken *token = pattern_dictionary->getValue("PatternType");

  if (token == NULL || ! token->isInteger()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  int pattern_type = token->getIntegerValue();

  if (pattern_type != 1) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return;
  }

  //------

  token = pattern_dictionary->getValue("PaintType");

  if (token == NULL || ! token->isInteger()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  int paint_type = token->getIntegerValue();

  if (paint_type != 1 && paint_type != 2) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return;
  }

  //------

  token = pattern_dictionary->getValue("TilingType");

  if (token == NULL || ! token->isInteger()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  int tiling_type = token->getIntegerValue();

  if (tiling_type != 1 && tiling_type != 2 && tiling_type != 3) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return;
  }

  //------

  token = pattern_dictionary->getValue("BBox");

  if (token == NULL || ! token->isArray()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSViewArrayToken *array_token = (PSViewArrayToken *) token;

  int num_tokens = array_token->getNumValues();

  if (num_tokens != 4) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return;
  }

  double bbox[4];

  for (int i = 1; i <= 4; i++) {
    PSViewToken *sub_token = array_token->getValue(i);

    if (! sub_token->isNumber()) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
      return;
    }

    bbox[i - 1] = sub_token->getRealValue();
  }

  //------

  token = pattern_dictionary->getValue("XStep");

  if (token == NULL || ! token->isNumber()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  double x_step = token->getRealValue();

  //------

  token = pattern_dictionary->getValue("YStep");

  if (token == NULL || ! token->isNumber()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  double y_step = token->getRealValue();

  //------

  PSViewToken *paint_proc = pattern_dictionary->getValue("PaintProc");

  if (paint_proc == NULL || ! paint_proc->isProcedure()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  //------

  token = pattern_dictionary->getValue("Implementation");

  if (token == NULL || ! token->isGState()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSViewGStateToken *gstate_token = (PSViewGStateToken *) token;

  PSViewGState *gstate = gstate_token->getGState();

  //------

  pattern_color_ = true;

  pattern_ = new PSViewGStatePattern(pattern_dictionary, pattern_type, paint_type, tiling_type,
                                     bbox, x_step, y_step, paint_proc, gstate);

  if (paint_type == 2) {
    gstate->color_space_   = color_space_;
    gstate->pattern_color_ = pattern_color_;
    gstate->pen_           = pen_;
  }
}

void
PSViewGState::
execForm(PSViewDictionaryToken *form_dictionary)
{
  PSViewToken *token = form_dictionary->getValue("FormType");

  if (token == NULL || ! token->isInteger()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSVinteger form_type = token->getIntegerValue();

  if (form_type != 1) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return;
  }

  //------

  token = form_dictionary->getValue("BBox");

  if (token == NULL || ! token->isArray()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSViewArrayToken *array_token = (PSViewArrayToken *) token;

  PSVinteger num_tokens = array_token->getNumValues();

  if (num_tokens != 4) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return;
  }

  double bbox[4];

  for (int i = 1; i <= 4; i++) {
    PSViewToken *sub_token = array_token->getValue(i);

    if (! sub_token->isNumber()) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
      return;
    }

    bbox[i - 1] = sub_token->getRealValue();
  }

  //------

  token = form_dictionary->getValue("Matrix");

  if (token == NULL || ! token->isMatrix()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  CMatrix2D *matrix = token->getMatrix();

  //------

  PSViewToken *paint_proc = form_dictionary->getValue("PaintProc");

  if (paint_proc == NULL || ! paint_proc->isProcedure()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  //------

  token = new PSViewNameToken(getPSView(), "Implementation");

  PSViewToken *sub_token = new PSViewNullToken(getPSView());

  form_dictionary->addValue(token, sub_token);

  //------

  getPSView()->getGStateTokenMgr()->save();

  postMultiplyCTMMatrix(matrix);

  newPath();

  moveTo(bbox[0], bbox[1]);
  lineTo(bbox[2], bbox[1]);
  lineTo(bbox[2], bbox[3]);
  lineTo(bbox[0], bbox[3]);

  closePath();

  clip();

  newPath();

  getPSView()->getOperandStack()->push(form_dictionary);

  paint_proc->execute();

  getPSView()->getGStateTokenMgr()->restore();
}

void
PSViewGState::
setColorSpace(const PSViewName &color_space)
{
  color_space_ = &color_space;
}

void
PSViewGState::
setPatternColor(bool flag)
{
  pattern_color_ = flag;
}

const PSViewName &
PSViewGState::
getColorSpace()
{
  return *color_space_;
}

bool
PSViewGState::
getPatternColor()
{
  return pattern_color_;
}

void
PSViewGState::
setCMYKColor(const CCMYK &cmyk)
{
  cmyk_ = cmyk;

  cmyk_valid_ = true;
  hsb_valid_  = false;

  CRGBA rgba = CRGBA(cmyk_.toRGB());

  setRGBColor1(rgba);
}

void
PSViewGState::
getCMYKColor(CCMYK &cmyk)
{
  if (! cmyk_valid_)
    cmyk_ = rgbToCMYK(pen_.getColor());

  cmyk = cmyk_;
}

void
PSViewGState::
setHSBColor(const CHSB &hsb)
{
  hsb_ = hsb;

  hsb_valid_  = true;
  cmyk_valid_ = false;

  CRGBA rgba = CRGBA(hsb_.toRGB());

  setRGBColor1(rgba);
}

void
PSViewGState::
getHSBColor(CHSB &hsb)
{
  if (! hsb_valid_)
    hsb_ = CHSB::fromRGB(pen_.getColor().getRGB());

  hsb = hsb_;
}

CCMYK
PSViewGState::
rgbToCMYK(const CRGBA &rgba)
{
  CCMYK cmyk = CCMYK::fromRGB(rgba.getRGB());

  //------

  double black = cmyk.getBlack();

  PSViewToken *token1 = new PSViewRealToken(getPSView(), black);

  getPSView()->getOperandStack()->push(token1);

  PSViewToken *proc = getBlackGeneration();

  proc->execute();

  PSViewToken *token2 = getPSView()->getOperandStack()->pop();

  if (token2 != NULL) {
    black = token2->getRealValue();

    black = std::min(std::max(black, 0.0), 1.0);

    cmyk.setBlack(black);
  }

  delete token1;

  //------

  double delta = cmyk.getBlack();

  token1 = new PSViewRealToken(getPSView(), delta);

  getPSView()->getOperandStack()->push(token1);

  proc = getUnderColorRemoval();

  proc->execute();

  token2 = getPSView()->getOperandStack()->pop();

  if (token2 != NULL) {
    delta = token2->getRealValue();

    cmyk.lighten(delta);
  }

  delete token1;

  //------

  return cmyk;
}

void
PSViewGState::
setRGBColor(const CRGBA &rgba)
{
  cmyk_valid_ = false;
  hsb_valid_  = false;

  setRGBColor1(rgba);
}

void
PSViewGState::
setRGBColor1(const CRGBA &rgba)
{
  pen_  .setColor(rgba);
  brush_.setColor(rgba);
}

void
PSViewGState::
getRGBColor(CRGBA &rgba)
{
  rgba = pen_.getColor();
}

void
PSViewGState::
setGray(double gray)
{
  setRGBColor1(CRGBA(gray));
}

double
PSViewGState::
getGray()
{
  return pen_.getColor().getGray();
}

void
PSViewGState::
setFlat(double flat)
{
  if (flat < 0.2) flat = 0.2;
  if (flat > 100) flat = 100;

  pen_.setFlatness(flat);
}

double
PSViewGState::
getFlat()
{
  return pen_.getFlatness();
}

void
PSViewGState::
setLineWidth(double line_width)
{
  pen_.setWidth(line_width);
}

double
PSViewGState::
getLineWidth()
{
  return pen_.getWidth();
}

void
PSViewGState::
setLineCap(CLineCapType line_cap)
{
  pen_.setLineCap(line_cap);
}

CLineCapType
PSViewGState::
getLineCap()
{
  return pen_.getLineCap();
}

void
PSViewGState::
setLineJoin(CLineJoinType line_join)
{
  pen_.setLineJoin(line_join);
}

CLineJoinType
PSViewGState::
getLineJoin()
{
  return pen_.getLineJoin();
}

void
PSViewGState::
setMitreLimit(double mitre_limit)
{
  pen_.setMitreLimit(mitre_limit);
}

double
PSViewGState::
getMitreLimit()
{
  return pen_.getMitreLimit();
}

void
PSViewGState::
setStrokeAdjust(bool flag)
{
  stroke_adjust_ = flag;
}

bool
PSViewGState::
getStrokeAdjust()
{
  return stroke_adjust_;
}

void
PSViewGState::
setDashPattern(double *dash_array, int num_dashes, double dash_offset)
{
  CLineDash dash;

  dash.setDashes(dash_array, num_dashes, dash_offset);

  pen_.setLineDash(dash);
}

void
PSViewGState::
getDashPattern(const double **dash_array, int *num_dashes, double *dash_offset)
{
  const CLineDash &dash = pen_.getLineDash();

  *dash_array  = dash.getLengths();
  *num_dashes  = dash.getNumLengths();
  *dash_offset = dash.getOffset();
}

void
PSViewGState::
setBlackGeneration(PSViewToken *token)
{
  black_generation_ = token;
}

PSViewToken *
PSViewGState::
getBlackGeneration()
{
  if (black_generation_ == NULL) {
    black_generation_ = new PSViewArrayToken(getPSView(), (uint) 0);

    black_generation_->setExecutable();
  }

  return black_generation_;
}

void
PSViewGState::
setUnderColorRemoval(PSViewToken *token)
{
  undercolor_removal_ = token;
}

PSViewToken *
PSViewGState::
getUnderColorRemoval()
{
  if (undercolor_removal_ == NULL) {
    undercolor_removal_ = new PSViewArrayToken(getPSView(), (uint) 0);

    undercolor_removal_->setExecutable();
  }

  return undercolor_removal_;
}

void
PSViewGState::
setCTMMatrix(CMatrix2D *matrix)
{
  ctm_matrix_ = *matrix;
}

CMatrix2D *
PSViewGState::
getCTMMatrix()
{
  return &ctm_matrix_;
}

CMatrix2D *
PSViewGState::
getInverseCTMMatrix()
{
  static CMatrix2D imatrix;

  bool flag = ctm_matrix_.invert(imatrix);

  if (flag)
    return &imatrix;
  else
    return NULL;
}

void
PSViewGState::
preMultiplyCTMMatrix(CMatrix2D *matrix)
{
  CMatrix2D matrix1 = (*matrix) * ctm_matrix_;

  setCTMMatrix(&matrix1);
}

void
PSViewGState::
postMultiplyCTMMatrix(CMatrix2D *matrix)
{
  CMatrix2D matrix1 = ctm_matrix_ * (*matrix);

  setCTMMatrix(&matrix1);
}

void
PSViewGState::
multiplyDistByCTMMatrix(double x1, double y1, double *x2, double *y2)
{
  CMatrix2D *matrix = getCTMMatrix();

  double tx1, ty1, tx2, ty2;

  matrix->multiplyPoint(0 , 0 , &tx1, &ty1);
  matrix->multiplyPoint(x1, y1, &tx2, &ty2);

  *x2 = tx2 - tx1;
  *y2 = ty2 - ty1;
}

void
PSViewGState::
multiplyByCTMMatrix(double x1, double y1, double *x2, double *y2)
{
  CMatrix2D *matrix = getCTMMatrix();

  matrix->multiplyPoint(x1, y1, x2, y2);
}

void
PSViewGState::
multiplyByInverseCTMMatrix(double x1, double y1, double *x2, double *y2)
{
  CMatrix2D *imatrix = getInverseCTMMatrix();

  if (imatrix != NULL)
    imatrix->multiplyPoint(x1, y1, x2, y2);
  else {
    *x2 = x1;
    *y2 = y1;
  }
}
