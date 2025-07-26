#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "createclass.h"

#include <QMainWindow>
#include <QDialog>
#include <QVector>
#include <QVBoxLayout>
<<<<<<< HEAD
//#include <iostream>
#include <fstream>
using namespace std;

#include "createclass.h"
#include "class_info_unit.h"
#include "savestuff.h"

=======
#include <QQuickWidget>

>>>>>>> 7dabfc36730817542fab985d1e88bd8c3b446888

QT_BEGIN_NAMESPACE
namespace Ui {

class main_window;
}
QT_END_NAMESPACE

<<<<<<< HEAD
=======
class QQuickWidget; // added for Map
>>>>>>> 7dabfc36730817542fab985d1e88bd8c3b446888

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
<<<<<<< HEAD

    //void deleteClassFrame(ClassInfoFrame* class_data);

    //void editClassFrame(ClassInfoFrame* class_data);
=======
>>>>>>> 7dabfc36730817542fab985d1e88bd8c3b446888

private:
    void setupConnections();
    void setupClassListLayout();
    void setupMenu();

<<<<<<< HEAD
    void debugAddClassToList(ClassInfo* tester);
=======
    void debugAddClasstoList(ClassInfo* tester);
>>>>>>> 7dabfc36730817542fab985d1e88bd8c3b446888
    ClassInfo* debugCreateClass(QString name,
                          QString days,
                          QString start,
                          QString end,
<<<<<<< HEAD
                          IsOnline online,
                          QString building = "Online Class");

private:
    Ui::main_window *ui_;
    QVector<ClassInfo> class_infos_;  // stores the data model

    //QVector<ClassInfoFrame*> class_data_containers;  // stores the actual QFrame widgets

    QVBoxLayout* class_list_layout_;  // layout where the frames get added
=======
                          QString building);
    //added for Map
    void initMap();
    QQuickWidget* mapWidget = nullptr;

private:
    Ui::main_window *ui_;
    QVector<ClassInfo> class_infos_;
    QVBoxLayout* class_list_layout_;
>>>>>>> 7dabfc36730817542fab985d1e88bd8c3b446888
};

#endif // MAINWINDOW_H
