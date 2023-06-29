#include "contorlor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    contorlor w;
    w.setWindowTitle("我的上位机");
    w.show();
    return a.exec();
}
