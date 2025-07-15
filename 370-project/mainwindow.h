#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QObject>
#include <QVector>

#include "createclass.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class class_Base;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_frame_customContextMenuRequested(const QPoint &pos);

    void on_class_edit_del_clicked();

    void on_classVectorTestButton_clicked();

    void on_classCreatorButton_clicked();

private:
    Ui::MainWindow *ui;
    QVector<ClassInfo> classes;
};

class class_Base : public QObject
{
    Q_OBJECT // Essential for Qt's meta-object system

public:
    // Constructor declaration
    explicit class_Base(QObject *parent = nullptr);
    ~class_Base();

    // Public member functions
    void roundEdges();

private slots:


private:
    // Private member variables
    QString m_name;
    Ui::class_Base *ui;
};
#endif // MAINWINDOW_H
