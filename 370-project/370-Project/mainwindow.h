#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
<<<<<<< Updated upstream
#include <QDialog>
=======
#include <QObject>
#include <QVector>

#include "createclass.h"
>>>>>>> Stashed changes

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

private slots:
    void on_pushButton_clicked();

    void on_classVectorTestButton_clicked();

    void on_classCreatorButton_clicked();

private:
    Ui::MainWindow *ui;
    QVector<ClassInfo> classes;
};
#endif // MAINWINDOW_H
