#include "WowSay.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WowSay w;
    w.setWindowFlag(Qt::FramelessWindowHint, true);
    w.show();
    return a.exec();
}
