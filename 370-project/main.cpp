#include "mainwindow.h"

#include <QApplication>
//#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

    ofstream SaveFile("cluster_save.txt");
    //this is a test.
    //i am going to go insane.
}
