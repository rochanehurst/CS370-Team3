#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "createclass.h"

#include <QMainWindow>
#include <QDialog>
#include <QVector>
#include <QVBoxLayout>
#include <QQuickWidget>


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

private slots:
    void createClassButtonHandler();
    void createClassFrame(const ClassInfo& class_info);

    void editSave();
    void removeFromSave();
    void clearSchedule();
    void searchClass();

    void debugPopulateList();

private:
    void setupConnections();
    void setupClassListLayout();
    void setupMenu();

    void debugAddClasstoList(ClassInfo* tester);
    ClassInfo* debugCreateClass(QString name,
                          QString days,
                          QString start,
                          QString end,
                          QString building);
    //added for Map
    void initMap();
    QQuickWidget* mapWidget = nullptr;

private:
    Ui::main_window *ui_;
    QVector<ClassInfo> class_infos_;
    QVBoxLayout* class_list_layout_;
};

#endif // MAINWINDOW_H
