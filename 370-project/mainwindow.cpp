#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createclass.h"

#include <QFrame>
#include <QApplication>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::main_window)
{
    ui->setupUi(this);
    connect(ui->class_creator_button, &QAbstractButton::clicked, this, &MainWindow::createClassButtonHandler);

    // Initalize layout in scroll area
    class_list_layout = qobject_cast<QVBoxLayout*>(ui->class_scroll_area->layout());
    if (!class_list_layout){
        class_list_layout = new QVBoxLayout(ui->class_scroll_area);
        ui->class_scroll_area->setLayout(class_list_layout);
    }
    class_list_layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
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
    class_list_layout->addWidget(class_data);
}

void MainWindow::editClassFrame(ClassInfoFrame* class_data){
    // TODO
    // Should be almost identical to createClassButtonHandler()
    // Form should be filled out with class_data_info
    ClassInfo edit_data = class_data->getFrameData();
}

void MainWindow::deleteClassFrame(ClassInfoFrame* class_data)
{
    // TODO
    // Remove from save file
    class_list_layout->removeWidget(class_data);
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
