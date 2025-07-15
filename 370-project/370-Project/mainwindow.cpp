#include "mainwindow.h"
<<<<<<< Updated upstream
#include "./ui_mainwindow.h"
#include "createclass.h"
=======
#include "ui_mainwindow.h"
#include "createclass.h"

#include <QFrame>
#include <QApplication>
#include <QWidget>
>>>>>>> Stashed changes

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

<<<<<<< Updated upstream
void MainWindow::on_pushButton_clicked()
{
    Dialog classCreator;
    classCreator.setModal(true);
    classCreator.exec();    // creates window
=======
void MainWindow::on_frame_customContextMenuRequested(const QPoint &pos){
    // TODO
}

void MainWindow::on_class_edit_del_clicked()
{
    // TODO
>>>>>>> Stashed changes
}

class_Base::class_Base(QObject *parent){
    // TODO
}

class_Base::~class_Base(){
    // TODO
}

void MainWindow::on_classCreatorButton_clicked()
{
    Dialog classCreator;
    classCreator.setModal(true);
    if (classCreator.exec() == QDialog::Accepted) {
        ClassInfo classData = classCreator.getData();  // retrieve data from the dialog
        classes.append(classData);               // store in MainWindow’s QVector
    }
}

void MainWindow::on_classVectorTestButton_clicked()
{
    for (auto classData : classes){
        classData.print();
    }
}



