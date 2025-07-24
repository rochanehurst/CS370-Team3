#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QObject>
#include <QVector>
#include <QFrame>
#include <QVBoxLayout>
//#include <iostream>
#include <fstream>
using namespace std;

#include "createclass.h"
#include "class_info_unit.h"
#include "savestuff.h"


QT_BEGIN_NAMESPACE
namespace Ui {

class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //extern ofstream SaveFile;

private slots:
    void createClassButtonHandler();

    void createClassFrame(const ClassInfo& class_info);

    void deleteClassFrame(ClassInfoFrame* class_data);

    void editClassFrame(ClassInfoFrame* class_data);

private:
    Ui::MainWindow *ui;

    QVector<ClassInfo> class_info;  // stores the data model

    QVector<ClassInfoFrame*> class_data_containers;  // stores the actual QFrame widgets

    QVBoxLayout* classListLayout;  // layout where the frames get added
};

#endif // MAINWINDOW_H
