#include <QApplication>
#include "XMLLoader.h"

int main(int argc, char *argv[]) {

  QApplication app(argc, argv);

  XMLLoader window;

  window.resize(300, 300);
  window.setWindowTitle("XML Handler");
  window.show();

  return app.exec();
}
