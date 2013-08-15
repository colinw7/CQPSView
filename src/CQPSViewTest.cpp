#include <CQPSView.h>
#include <CPSView.h>

#include <QApplication>
#include <QVBoxLayout>

class CQPSViewTest : public QWidget {
 private:
  CQPSView *psview_;

 public:
  CQPSViewTest();
 ~CQPSViewTest();

  void loadFile(const QString &fileName);
};

int
main(int argc, char **argv)
{
  QApplication app(argc, argv);

  CQPSViewTest test;

  test.show();

  if (argc > 1)
    test.loadFile(argv[1]);

  return app.exec();
}

CQPSViewTest::
CQPSViewTest()
{
  resize(CPSView::PAGE_WIDTH, CPSView::PAGE_HEIGHT);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  psview_ = new CQPSView;

  layout->addWidget(psview_);
}

CQPSViewTest::
~CQPSViewTest()
{
}

void
CQPSViewTest::
loadFile(const QString &fileName)
{
  psview_->loadFile(fileName);
}
