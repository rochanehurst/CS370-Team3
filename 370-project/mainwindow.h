#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QObject>
#include <QVector>
#include <QFrame>
#include <QVBoxLayout>

#include "createclass.h"
#include "class_info_unit.h"

QT_BEGIN_NAMESPACE
namespace Ui {

class main_window;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void createClassButtonHandler();

    void createClassFrame(const ClassInfo& class_info);

    void deleteClassFrame(ClassInfoFrame* class_data);

    void editClassFrame(ClassInfoFrame* class_data);

private:
    Ui::main_window *ui;

    QVector<ClassInfo> class_info;

    QVector<ClassInfoFrame*> class_data_containers;

    QVBoxLayout* class_list_layout;
};

#endif // MAINWINDOW_H
