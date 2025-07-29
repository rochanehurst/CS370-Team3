#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "createclass.h"
#include "class_info_unit.h"
#include "savestuff.h"
#include "search.h"

#include <QMainWindow>
#include <QDialog>
#include <QVector>
#include <QVBoxLayout>
//#include <iostream>
#include <fstream>
#include <QQuickWidget>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {

class main_window;
}
QT_END_NAMESPACE

class QQuickWidget; // added for Map

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

    void editSave();
    void removeFromSave();
    void clearSchedule();
    void searchClass();

    void debugPopulateList();

    //void deleteClassFrame(ClassInfoFrame* class_data);

    //void editClassFrame(ClassInfoFrame* class_data);

private:
    void setupConnections();
    void setupClassListLayout();
    void setupMenu();


    void debugAddClasstoList(ClassInfo* tester);
    ClassInfo* debugCreateClass(QString name,
                          QString days,
                          QString start,
                          QString end,
                          //IsOnline online,
                          QString building);

private:
    Ui::main_window *ui_;
    QVector<ClassInfo> class_infos_;  // stores the data model

    //QVector<ClassInfoFrame*> class_data_containers;  // stores the actual QFrame widgets

    QVBoxLayout* class_list_layout_;  // layout where the frames get added
    //added for Map
    void initMap();
    QQuickWidget* mapWidget = nullptr;
};

#endif // MAINWINDOW_H
