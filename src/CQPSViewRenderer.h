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

  CQPSViewRenderer *dup() override;

  void beginDraw();
  void endDraw();

  void setSize(int width, int height);

  void clear(const CRGBA &bg) override;

  void setViewMatrix(const CMatrix2D &m);

  void setPen(const CPen &pen) override;
  void setBrush(const CBrush &brush) override;

  void pathInit() override;
  void pathMoveTo(const CPoint2D &p) override;
  void pathLineTo(const CPoint2D &p) override;
  void pathArc(const CPoint2D &p, double r1, double r2, double a1, double a2) override;
  void pathBezier2To(const CPoint2D &p1, const CPoint2D &p2) override;
  void pathBezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) override;
  void pathClose() override;
  void pathStroke() override;
  void pathFill() override;
  void pathClip() override;

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
