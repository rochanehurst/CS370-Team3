#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createclass.h"

#include <QFrame>
#include <QApplication>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // initalize layout in scroll area
    classListLayout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!classListLayout){
        classListLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
        ui->scrollAreaWidgetContents->setLayout(classListLayout);
    }
    classListLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::createClassFrame(const ClassInfo& class_info)
{
    // TODO
    // ADD TO SAVE FILE
    ClassInfoFrame* class_data = new ClassInfoFrame();
    class_data->createFrame(class_info);
    classListLayout->addWidget(class_data);
}

void MainWindow::editClassFrame(ClassInfoFrame* class_data){
    // TODO
    // Should be almost identical to on_classCreatorButton_clicked()
    // Form should be filled out with class_data_info
    ClassInfo edit_data = class_data->getData();

    // Use edit_data to fill out create class form
}

void MainWindow::deleteClassFrame(ClassInfoFrame* class_data)
{
    // TODO
    // Connect with toolbox menu
    // Remove from save file
    classListLayout->removeWidget(class_data);
}

void MainWindow::on_classCreatorButton_clicked()
{
    Dialog classCreator;
    classCreator.setModal(true);
    if (classCreator.exec() == QDialog::Accepted) {
        ClassInfo classData = classCreator.getData();   // retrieve data from the dialog
        class_info.append(classData);                      // store in MainWindow’s QVector
        createClassFrame(classData);
        // TODO:
        // Append classes to end of text file
    }
}
