#include "mainwindow.h"

#include <QApplication>

// LAST UPDATED:
// 7/17/2025 4:36PM
// BY COLE LEAHY

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
