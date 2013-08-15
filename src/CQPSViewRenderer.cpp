#include "CQPSViewRenderer.h"
#include <CQUtil.h>

CQPSViewRenderer::
CQPSViewRenderer() :
 painter_   (NULL),
 path_      (NULL),
 fillType_  (FILL_TYPE_WINDING),
 flatness_  (0),
 antiAlias_ (false)
{
  painter_ = new QPainter;

  viewMatrix_.setIdentity();

  setSize(CPSView::PAGE_WIDTH, CPSView::PAGE_HEIGHT);
}

CQPSViewRenderer *
CQPSViewRenderer::
dup()
{
  return new CQPSViewRenderer;
}

void
CQPSViewRenderer::
beginDraw()
{
  painter_->begin(&qimage_);

  if (antiAlias_)
    painter_->setRenderHints(QPainter::Antialiasing);

  painter_->fillRect(qimage_.rect(), QBrush(QColor(255,255,255)));

  painter_->setWorldMatrix(CQUtil::toQMatrix(range_.getMatrix()*viewMatrix_));

  transform_  = painter_->worldTransform();
  itransform_ = transform_.inverted();
}

void
CQPSViewRenderer::
endDraw()
{
  painter_->end();
}

void
CQPSViewRenderer::
setSize(int width, int height)
{
  if (width != qimage_.width() || height != qimage_.height())
    qimage_ = QImage(QSize(width, height), QImage::Format_ARGB32);

  range_.setPixelRange(0, 0, width - 1, height - 1);

  range_.setWindowRange(0, 0, width - 1, height - 1);
}

void
CQPSViewRenderer::
clear(const CRGBA &bg)
{
  painter_->fillRect(qimage_.rect(), QBrush(CQUtil::rgbaToColor(bg)));
}

void
CQPSViewRenderer::
setPen(const CPen &pen)
{
  pen_.setColor(CQUtil::rgbaToColor(pen.getColor()));

  double w = pen.getWidth();

  if (w <= 0.0) w = 0.01;

  pen_.setWidthF(w);

  const CLineDash &dash = pen.getLineDash();

  int num = dash.getNumLengths();

  if (num > 0) {
    pen_.setStyle(Qt::CustomDashLine);

    pen_.setDashOffset(dash.getOffset());

    QVector<qreal> dashes;

    for (int i = 0; i < num; ++i)
      dashes << dash.getLength(i)/pen_.widthF();

    if (num & 1)
      dashes << dash.getLength(0)/pen_.widthF();

    pen_.setDashPattern(dashes);
  }
  else
    pen_.setStyle(Qt::SolidLine);

  switch (pen.getLineCap()) {
    case LINE_CAP_TYPE_BUTT:
      pen_.setCapStyle(Qt::FlatCap);
      break;
    case LINE_CAP_TYPE_ROUND:
      pen_.setCapStyle(Qt::RoundCap);
      break;
    case LINE_CAP_TYPE_SQUARE:
      pen_.setCapStyle(Qt::SquareCap);
      break;
    default:
      break;
  }

  switch (pen.getLineJoin()) {
    case LINE_JOIN_TYPE_MITRE:
      pen_.setJoinStyle(Qt::MiterJoin);
      break;
    case LINE_JOIN_TYPE_ROUND:
      pen_.setJoinStyle(Qt::RoundJoin);
      break;
    case LINE_JOIN_TYPE_BEVEL:
      pen_.setJoinStyle(Qt::BevelJoin);
      break;
    default:
      break;
  }

  pen_.setMiterLimit(pen.getMitreLimit());

  flatness_ = pen.getFlatness();

  painter_->setPen(pen_);
}

void
CQPSViewRenderer::
setBrush(const CBrush &brush)
{
  brush_.setColor(CQUtil::rgbaToColor(brush.getColor()));
  brush_.setStyle(Qt::SolidPattern);

  fillType_ = brush.getFillRule();

  painter_->setBrush(brush_);
}

void
CQPSViewRenderer::
setViewMatrix(const CMatrix2D &m)
{
  viewMatrix_ = m;

  painter_->setWorldMatrix(CQUtil::toQMatrix(range_.getMatrix()*viewMatrix_));

  transform_  = painter_->worldTransform();
  itransform_ = transform_.inverted();
}

void
CQPSViewRenderer::
pathInit()
{
  delete path_;

  path_ = new QPainterPath;
}

void
CQPSViewRenderer::
pathMoveTo(const CPoint2D &p)
{
  path_->moveTo(p.x, p.y);
}

void
CQPSViewRenderer::
pathLineTo(const CPoint2D &p)
{
  path_->lineTo(p.x, p.y);
}

void
CQPSViewRenderer::
pathArc(const CPoint2D &c, double xr, double yr, double angle1, double angle2)
{
  double a1 = CMathGen::RadToDeg(angle1);
  double a2 = CMathGen::RadToDeg(angle2);

  QRectF rect(QPointF(c.x - xr, c.y - yr), QPointF(c.x + xr, c.y + yr));

  a1 = -a1;
  a2 = -a2;

  path_->arcTo(rect, a1, a2 - a1);
}

void
CQPSViewRenderer::
pathBezier2To(const CPoint2D &p1, const CPoint2D &p2)
{
  path_->quadTo(CQUtil::toQPoint(p1), CQUtil::toQPoint(p2));
}

void
CQPSViewRenderer::
pathBezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3)
{
  path_->cubicTo(CQUtil::toQPoint(p1), CQUtil::toQPoint(p2), CQUtil::toQPoint(p3));
}

void
CQPSViewRenderer::
pathClose()
{
  path_->closeSubpath();
}

void
CQPSViewRenderer::
pathStroke()
{
  painter_->strokePath(*path_, pen_);
}

void
CQPSViewRenderer::
pathFill()
{
  if (fillType_ == FILL_TYPE_EVEN_ODD)
    path_->setFillRule(Qt::OddEvenFill);
  else
    path_->setFillRule(Qt::WindingFill);

  painter_->fillPath(*path_, brush_);
}

void
CQPSViewRenderer::
pathClip()
{
  if (fillType_ == FILL_TYPE_EVEN_ODD)
    path_->setFillRule(Qt::OddEvenFill);
  else
    path_->setFillRule(Qt::WindingFill);

  painter_->setClipPath(*path_);
}

#if 0
void
CQPSViewRenderer::
pathClipPath(PathBuilder &builder)
{
  QPainterPath path = painter_->clipPath();

  int num = path.elementCount();

  if (num > 0) {
    for (int i = 0; i < num; ++i) {
      const QPainterPath::Element &e = path.elementAt(i);

      QPointF p = e;

      if      (e.isMoveTo()) {
        builder.moveTo(CQUtil::fromQPoint(p));
      }
      else if (e.isLineTo()) {
        builder.lineTo(CQUtil::fromQPoint(p));
      }
      else if (e.isCurveTo()) {
        const QPainterPath::Element &e1 = path.elementAt(i + 1);
        const QPainterPath::Element &e2 = path.elementAt(i + 2);

        QPointF p1 = e1;
        QPointF p2 = e2;

        builder.curveTo(CQUtil::fromQPoint(p), CQUtil::fromQPoint(p1), CQUtil::fromQPoint(p2));

        i += 2;
      }
    }
  }
  else {
    double xmin = 0.0, ymin = 0.0, xmax = CPSView::PAGE_WIDTH - 1, ymax = CPSView::PAGE_HEIGHT - 1;

    builder.moveTo(CPoint2D(xmin, ymin));
    builder.lineTo(CPoint2D(xmax, ymin));
    builder.lineTo(CPoint2D(xmax, ymax));
    builder.lineTo(CPoint2D(xmin, ymax));
  }
}
#endif
