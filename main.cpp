#include "db.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  MainWindow mw;
  if (!db::init_success) {
    db::close();  // should not be necessary but to be sure no connection remains open
    return 1;
  }
  mw.show();
  return app.exec();
}
