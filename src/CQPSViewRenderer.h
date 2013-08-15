#ifndef CQPSVIEW_RENDERER_H
#define CQPSVIEW_RENDERER_H

#include <CPSView.h>
#include <CDisplayTransform2D.h>

#include <QImage>
#include <QPainter>
#include <QBrush>
#include <QPen>

class CQPSViewRenderer : public CPSViewRenderer {
 public:
  CQPSViewRenderer();

  CQPSViewRenderer *dup();

  void beginDraw();
  void endDraw();

  void setSize(int width, int height);

  void clear(const CRGBA &bg);

  void setViewMatrix(const CMatrix2D &m);

  void setPen(const CPen &pen);
  void setBrush(const CBrush &brush);

  void pathInit();
  void pathMoveTo(const CPoint2D &p);
  void pathLineTo(const CPoint2D &p);
  void pathArc(const CPoint2D &p, double r1, double r2, double a1, double a2);
  void pathBezier2To(const CPoint2D &p1, const CPoint2D &p2);
  void pathBezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);
  void pathClose();
  void pathStroke();
  void pathFill();
  void pathClip();

  //void pathClipPath(PathBuilder &builder);

  QImage getQImage() const { return qimage_; }

 private:
  QImage           qimage_;
  QPainter        *painter_;
  QPainterPath    *path_;
  QBrush           brush_;
  QPen             pen_;
  QFont            qfont_;
  CFillType        fillType_;
  double           flatness_;
  CMatrix2D        viewMatrix_;
  bool             antiAlias_;
  CDisplayRange2D  range_;
  QTransform       transform_;
  QTransform       itransform_;
};

#endif
