#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("HAL Tool");
    w.setWindowIcon(QIcon("://HAL.ico"));
    w.setFixedSize(380,234);
    w.show();
    return a.exec();
}
