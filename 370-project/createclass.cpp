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

    // changes disabled checkboxes to be a nice gray instead of a bright ass neon green
    for (auto* day : checkboxes){
        day->setStyleSheet(R"(
        QCheckBox:disabled {
            color: gray; /* text color */
        }
        QCheckBox::indicator:disabled {
            background-color: gray; /* box color */
            border: 1px solid darkgray;
        }
    )");
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

ClassInfo Dialog::getData() const {
    return classInfo;
}


void Dialog::createClass()
{
    classInfo.school = ui->schoolComboBox->currentText();
    classInfo.name = ui->className->text();

    if (ui->onlineCheckBox->isChecked()){ classInfo.building = "Online Class"; }
    else { classInfo.building = ui->buildingName->text(); }

    classInfo.startTime = ui->timeStart->text();
    classInfo.endTime = ui->timeStop->text();
    classInfo.days = dayStringCreate();
    classInfo.online = ui->onlineCheckBox->isChecked();

    mwf = 0;
    tuth = 0;
}

void Dialog::on_confirmButtonBox_accepted()
{
    if (ui->className->text().isEmpty()){
        QMessageBox::critical(this, "Error", "Enter class name");
        return;
    }
    if (ui->buildingName->text().isEmpty() && !ui->onlineCheckBox->isChecked()){
        QMessageBox::critical(this, "Error", "Enter building name");
        return;
    }
    if ((mwf + tuth) <= 0){
        QMessageBox::critical(this, "Error", "Select a day");
        return;
    }
    createClass();
    accept();
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

void Dialog::on_onlineCheckBox_stateChanged(int arg1)
{
    switch(arg1){
        case 0:
            ui->buildingName->setDisabled(false);
            break;
        case 2:
            ui->buildingName->setDisabled(true);
            break;
        default:
            ui->buildingName->setDisabled(false);
            break;
    }

}

