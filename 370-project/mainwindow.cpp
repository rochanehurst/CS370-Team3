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
    connect(ui->classCreatorButton, &QAbstractButton::clicked, this, &MainWindow::createClassButtonHandler);

    // Initalize layout in scroll area
    classListLayout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!classListLayout){
        classListLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
        ui->scrollAreaWidgetContents->setLayout(classListLayout);
    }
    classListLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
}

MainWindow::~MainWindow()
{
    // Call save data
    // Will save data before closing
    // One problem: will not save if program crashes
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
    // Should be almost identical to createClassButtonHandler()
    // Form should be filled out with class_data_info
    ClassInfo edit_data = class_data->getData();
}

void MainWindow::deleteClassFrame(ClassInfoFrame* class_data)
{
    // TODO
    // Remove from save file
    classListLayout->removeWidget(class_data);
}

void MainWindow::createClassButtonHandler()
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
