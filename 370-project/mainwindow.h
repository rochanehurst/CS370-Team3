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

class main_window;
}
QT_END_NAMESPACE

enum class IsOnline{
    Yes,
    No
};

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

    void debugPopulateList();

    //void deleteClassFrame(ClassInfoFrame* class_data);

    //void editClassFrame(ClassInfoFrame* class_data);

private:
    void setupConnections();
    void setupClassListLayout();

    void debugAddClassToList(ClassInfo* tester);
    ClassInfo* debugCreateClass(QString name,
                          QString days,
                          QString start,
                          QString end,
                          IsOnline online,
                          QString building = "Online Class");

private:
    Ui::main_window *ui_;
    QVector<ClassInfo> class_infos_;  // stores the data model

    //QVector<ClassInfoFrame*> class_data_containers;  // stores the actual QFrame widgets

    QVBoxLayout* class_list_layout_;  // layout where the frames get added
};

#endif // MAINWINDOW_H
