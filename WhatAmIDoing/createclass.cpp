#include "createclass.h"
#include "ui_createclass.h"

#include <QStringListModel>
#include <QMessageBox>
#include <QDebug>
#include <QCheckBox>

int mwf = 0;
int tuth = 0;

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("Create Class");

    // Allows pop up warnings without closing window
    disconnect(ui->confirmButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    disconnect(ui->confirmButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(ui->monday, &QCheckBox::checkStateChanged, this, &Dialog::on_MWF_stateChanged);
    connect(ui->wednesday, &QCheckBox::checkStateChanged, this, &Dialog::on_MWF_stateChanged);
    connect(ui->friday, &QCheckBox::checkStateChanged, this, &Dialog::on_MWF_stateChanged);

    connect(ui->tuesday, &QCheckBox::checkStateChanged, this, &Dialog::on_TR_stateChanged);
    connect(ui->thursday, &QCheckBox::checkStateChanged, this, &Dialog::on_TR_stateChanged);

    // Create an array of checkboxes
    const auto& children = ui->days->children();
    for (QObject* child : children) {
        if (QCheckBox* checkbox = qobject_cast<QCheckBox*>(child)) {
            checkboxes.push_back(checkbox);
        }
    }
}

Dialog::~Dialog()
{
    delete ui;
}

QString Dialog::dayStringCreate(){
    QString dayString;
    for (QCheckBox* const day : checkboxes){
        if (day->isChecked()){
            // High chance there is a better way to do this, I'm just too dumb
            if (day->objectName() == "monday") { dayString.append("M"); }
            if (day->objectName() == "tuesday") { dayString.append("T"); }
            if (day->objectName() == "wednesday") { dayString.append("W"); }
            if (day->objectName() == "thursday") { dayString.append("R"); }
            if (day->objectName() == "friday") { dayString.append("F"); }
        }
    }
    return dayString;
}

void Dialog::createClass()
{
    ClassInfo* newClass = new ClassInfo;
    newClass->school = ui->schoolComboBox->currentText();
    newClass->name = ui->className->text();
    newClass->building = ui->buildingName->text();
    newClass->startTime = ui->timeStart->text();
    newClass->endTime = ui->timeStop->text();
    newClass->days = dayStringCreate();
    newClass->online = ui->onlineCheckBox->isChecked();

    classes.push_back(newClass);
}

void Dialog::on_confirmButtonBox_accepted()
{
    if ((mwf + tuth) > 0){
        createClass();
        accept();
        return;
    }
    QMessageBox::critical(this, "Error", "No day selected");
}


void Dialog::on_confirmButtonBox_rejected()
{
    auto accepted = QMessageBox::question(
        this,
        "Cancel?",
        "Are you sure you want to cancel?\nAll data will be lost",
        QMessageBox::Yes | QMessageBox::No
        );
    if (accepted == QMessageBox::Yes){
        reject();
    }
}

<<<<<<< Updated upstream:WhatAmIDoing/createclass.cpp
=======

// prevents all days from being checked
void Dialog::fourDayChecker(QObject *sender, bool disable){
    QCheckBox* checkbox = qobject_cast<QCheckBox*>(sender);
    if (checkbox->objectName() == "tuesday"){ ui->thursday->setDisabled(disable); }
    else { ui->tuesday->setDisabled(disable); }
}


void Dialog::on_MWF_stateChanged(int arg1)
{
    bool disabled = false;

    if (arg1 == Qt::Checked) { mwf++;}
    else if (arg1 == Qt::Unchecked) {mwf--;}

    if (mwf > 0) { disabled = true; }
    if (mwf == 3) { disabled = false; } // MTWF or MWRF are valid

    ui->tuesday->setDisabled(disabled);
    ui->tuesday->setChecked(false);

    ui->thursday->setDisabled(disabled);
    ui->thursday->setChecked(false);
}



void Dialog::on_TR_stateChanged(int arg1)
{
    bool disabled = false;

    if (arg1 == Qt::Checked) { tuth++;}
    else if (arg1 == Qt::Unchecked) { tuth--; }

    if (tuth > 0) { disabled = true; }
    else { disabled = false; }

    if (mwf == 3) {     // MTWF or MWRF are valid
        fourDayChecker(sender(), disabled);
        disabled = false;
    }

    ui->monday->setDisabled(disabled);
    ui->wednesday->setDisabled(disabled);
    ui->friday->setDisabled(disabled);
}

// prevents time start from being higher than time stop
void Dialog::on_timeStart_userTimeChanged(const QTime &time)
{
    QTime otherTime = ui->timeStop->time();
    if (time.hour() >= otherTime.hour()){
        otherTime = time.addSecs(3600);  // Take time stop and add an hour
        ui->timeStop->setTime(otherTime);
    }
}

// prevents time stop from being lower than time start
void Dialog::on_timeStop_userTimeChanged(const QTime &time)
{
    QTime otherTime = ui->timeStart->time();
    if (time.hour() <= otherTime.hour()){
        otherTime = time.addSecs(-3600);    // Take time start and subtract an hour
        ui->timeStart->setTime(otherTime);
    }
}

>>>>>>> Stashed changes:370-project/370-Project/createclass.cpp
