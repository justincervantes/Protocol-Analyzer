#include "application.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Application w;
    w.setWindowTitle("Protocol Analyzer");
    w.show();
    return a.exec();
}
