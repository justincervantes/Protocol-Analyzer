#include "application.h"
#include "session.h"
#include <QApplication>

sessionVariables globals;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Application w;
    w.setWindowTitle("Protocol Analyzer");
    globals.app = &w;
    w.show();
    return a.exec();
}
