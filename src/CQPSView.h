#ifndef CQ_PSVIEW_H
#define CQ_PSVIEW_H

#include <QWidget>

class CQPSViewRenderer;
class CPSView;
class QPaintEvent;

class CQPSView : public QWidget {
  Q_OBJECT

 public:
  CQPSView(QWidget *parent=0);
 ~CQPSView();

  void loadFile(const QString &fileName);

  void paintEvent(QPaintEvent *) override;

 private:
  CQPSViewRenderer *renderer_;
  CPSView          *psview_;
};

#endif
