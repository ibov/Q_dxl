#include "serialmonitor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SerialMonitor w;
    w.show();
    return a.exec();
}
