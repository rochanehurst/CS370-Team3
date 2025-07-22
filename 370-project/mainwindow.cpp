#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createclass.h"
#include "class_info_unit.h"

#include <QFrame>
#include <QApplication>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::main_window)
{
    ui_->setupUi(this);
    setupConnections();

    // Initalize layout in scroll area
    setupClassListLayout();
}

MainWindow::~MainWindow() {
    // TODO: Persist data to file before exit
    delete ui_;
}

void MainWindow::setupConnections() {
    connect(ui_->class_creator_button, &QAbstractButton::clicked, this, &MainWindow::createClassButtonHandler);
    connect(ui_->clear_schedule_button, &QAbstractButton::clicked, this, &MainWindow::clearSchedule);
    connect(ui_->debug_populate_button, &QAbstractButton::clicked, this, &MainWindow::debugPopulateList);
}

void MainWindow::setupClassListLayout() {
    class_list_layout_ = qobject_cast<QVBoxLayout*>(ui_->class_scroll_area->layout());
    if (!class_list_layout_) {
        class_list_layout_ = new QVBoxLayout(ui_->class_scroll_area);
        ui_->class_scroll_area->setLayout(class_list_layout_);
    }
    class_list_layout_->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
}

void MainWindow::createClassFrame(const ClassInfo& class_info) {
    ClassInfoFrame* class_data = new ClassInfoFrame();
    class_data->createFrame(class_info);
    class_list_layout_->addWidget(class_data);
    // TODO: Add class info to save file
}

void MainWindow::editSave() {
    // TODO: Add edited class info to save file
}

void MainWindow::removeFromSave() {
// TODO: Remove deleted class info from save file
}

void MainWindow::createClassButtonHandler() {
    Dialog classCreator;
    classCreator.setModal(true);
    if (classCreator.exec() == QDialog::Accepted) {
        ClassInfo classData = classCreator.getData();       // Retrieve data from the dialog
        class_infos_.append(classData);                       // Store in MainWindow’s QVector
        createClassFrame(classData);
        // TODO: Append class data to save file
    }
}

void MainWindow::clearSchedule() {
    if (class_list_layout_->count() == 0) return;
    QLayoutItem *child;
    while ((child = class_list_layout_->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
}


void MainWindow::debugPopulateList()
{
    for (int i = 1; i <= 5; i++){
        ClassInfo* debug_info = new ClassInfo;
        debug_info->name = "test" + QString::number(i);
        debug_info->days = "MWF";
        debug_info->startTime = "7:30 AM";
        debug_info->endTime = "8:30 AM";
        debug_info->online = true;

        ClassInfoFrame* debug_data = new ClassInfoFrame();
        debug_data->createFrame(*debug_info);
        class_list_layout_->addWidget(debug_data);

    }
}

