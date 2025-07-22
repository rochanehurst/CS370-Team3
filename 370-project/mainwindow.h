#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "createclass.h"


#include <QMainWindow>
#include <QDialog>
#include <QVector>
#include <QVBoxLayout>


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

private slots:
    void createClassButtonHandler();
    void createClassFrame(const ClassInfo& class_info);

    void editSave();
    void removeFromSave();
    void clearSchedule();

    void debugPopulateList();

private:
    void setupConnections();
    void setupClassListLayout();

    void debugAddClasstoList(ClassInfo* tester);
    ClassInfo* debugCreateClass(QString name,
                          QString days,
                          QString start,
                          QString end,
                          IsOnline online,
                          QString building = "Online Class");


private:
    Ui::main_window *ui_;
    QVector<ClassInfo> class_infos_;
    QVBoxLayout* class_list_layout_;
};

#endif // MAINWINDOW_H
