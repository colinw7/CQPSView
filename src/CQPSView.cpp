#include <CQPSView.h>
#include <CPSView.h>
#include <CQPSViewRenderer.h>

CQPSView::
CQPSView(QWidget *parent) :
 QWidget(parent)
{
  renderer_ = new CQPSViewRenderer();

  psview_ = new CPSView;

  if (getenv("PSVIEW_DEBUG") != NULL)
    psview_->setDebug(true);
}

CQPSView::
~CQPSView()
{
}

void
CQPSView::
loadFile(const QString &fileName)
{
  psview_->setRenderer(renderer_);

  renderer_->beginDraw();

  //renderer_->setDataRange(0, 0, 594, 792);

  psview_->openCurrentFile(fileName.toStdString());

  psview_->executeCurrentFile();

  psview_->closeCurrentFile();

  renderer_->endDraw();

  update();
}

void
CQPSView::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  QImage qimage = renderer_->getQImage();

  painter.drawImage(0, 0, qimage);
}
