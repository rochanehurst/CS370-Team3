#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFrame>
#include <QApplication>
#include <QWidget>

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


void MainWindow::on_class_edit_del_clicked()
{

}

