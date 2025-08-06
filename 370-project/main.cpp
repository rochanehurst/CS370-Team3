#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QQmlApplicationEngine engine;
    engine.addImportPath(QString::fromUtf8(qgetenv("QML2_IMPORT_PATH")));
    engine.addImportPath("C:/Qt/6.9.1/mingw_64/qml");

    MainWindow w;
    w.show();
    return a.exec();
}
