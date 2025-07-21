// createclass.cpp
#include "createclass.h"
#include "ui_createclass.h"

#include <QStringListModel>
#include <QMessageBox>
#include <QDebug>
#include <QCheckBox>

int mwf = 0;
int tuth = 0;

// create form constructor
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("Create Class");
    setupConnections();
    setupCheckboxes();
    resetCounters();
}

// edit form constructor
Dialog::Dialog(const ClassInfo &info, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("Edit Class");

    QIcon icon(":/icons/icons/sign-up-icon-signup-square-box-on-transparent-background-free-png.png");
    setWindowIcon(icon);

    setupConnections();
    setupCheckboxes();
    resetCounters();
    editClassFrame(info);
}

Dialog::~Dialog()
{
    delete ui;
}

// Allows pop up warnings without closing window
void Dialog::setupConnections(){
    disconnect(ui->confirm_button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    disconnect(ui->confirm_button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(ui->confirm_button_box, &QDialogButtonBox::accepted, this, &Dialog::handleConfirmAccepted);
    connect(ui->confirm_button_box, &QDialogButtonBox::rejected, this, &Dialog::handleConfirmRejected);
    connect(ui->time_start, &QTimeEdit::userTimeChanged, this, &Dialog::startTimeChangeHandler);
    connect(ui->time_stop, &QTimeEdit::userTimeChanged, this, &Dialog::endTimeChangeHandler);
    connect(ui->online_check_box, &QCheckBox::checkStateChanged, this, &Dialog::onlineStateChangeHandler);

    connect(ui->monday, &QCheckBox::checkStateChanged, this, &Dialog::onMWFStateChanged);
    connect(ui->wednesday, &QCheckBox::checkStateChanged, this, &Dialog::onMWFStateChanged);
    connect(ui->friday, &QCheckBox::checkStateChanged, this, &Dialog::onMWFStateChanged);
    connect(ui->tuesday, &QCheckBox::checkStateChanged, this, &Dialog::onTRStateChanged);
    connect(ui->thursday, &QCheckBox::checkStateChanged, this, &Dialog::onTRStateChanged);
}

void Dialog::setupCheckboxes(){
    // Create an array of checkboxes
    const auto& children = ui->days->children();
    for (QObject* child : children) {
        if (QCheckBox* checkbox = qobject_cast<QCheckBox*>(child)) {
            checkboxes.push_back(checkbox);
        }
    }
}

void Dialog::resetCounters(){
    mwf = 0;
    tuth = 0;
}

void Dialog::editClassFrame(ClassInfo classFormInfo){
    ui->class_name->setText(classFormInfo.name);
    ui->building_name->setText(classFormInfo.building);

    Qt::CheckState online_state = Qt::Unchecked;
    if (classFormInfo.online) { online_state = Qt::Checked; }
    ui->online_check_box->setCheckState(online_state);

    QString format = "h:mm AP";
    ui->time_start->setTime(QTime::fromString(classFormInfo.startTime, format));
    ui->time_stop->setTime(QTime::fromString(classFormInfo.endTime, format));

    bool tuesday = false, thursday = false;

    for (const QChar &day : classFormInfo.days){
        // High chance there is a better way to do this, I'm just too dumb
        if (day == "M") { ui->monday->setCheckState(Qt::Checked);}
        if (day == "T") { tuesday = true; }
        if (day == "W") { ui->wednesday->setCheckState(Qt::Checked); }
        if (day == "R") { thursday = true; }
        if (day == "F") { ui->friday->setCheckState(Qt::Checked); }
    }
    if (tuesday) { ui->tuesday->setCheckState(Qt::Checked); }
    if (thursday) { ui->thursday->setCheckState(Qt::Checked); }

}


QString Dialog::dayStringCreate(){
    QString dayString;
    for (const QCheckBox* day : checkboxes){
        if (day->isChecked()){
            if (day->objectName() == "thursday") { dayString.append("R"); }
            else
            {
                QChar extractAbbr = day->objectName()[0].toUpper();
                dayString.append(extractAbbr);
            }
        }
    }
    return dayString;
}

ClassInfo Dialog::getData() const {
    return class_form_info;
}

void Dialog::createClassForm()
{
    class_form_info.school = ui->school_combo_box->currentText();
    class_form_info.name = ui->class_name->text();

    if (ui->online_check_box->isChecked()){ class_form_info.building = "Online Class"; }

    else { class_form_info.building = ui->building_name->text(); }

    class_form_info.startTime = ui->time_start->text();
    class_form_info.endTime = ui->time_stop->text();
    class_form_info.days = dayStringCreate();
    class_form_info.online = ui->online_check_box->isChecked();

    resetCounters();
}

void Dialog::handleConfirmAccepted()
{
    if (ui->class_name->text().isEmpty()){
        QMessageBox::critical(this, "Error", "Enter class name");
        return;
    }
    if (ui->building_name->text().isEmpty() && !ui->online_check_box->isChecked()){
        QMessageBox::critical(this, "Error", "Enter building name");
        return;
    }
    if ((mwf + tuth) <= 0){
        QMessageBox::critical(this, "Error", "Select a day");
        return;
    }
    createClassForm();
    accept();
}


void Dialog::handleConfirmRejected()
{
    auto rejected = QMessageBox::question(this,
        "Cancel?",
        "Are you sure you want to cancel?\nAll data will be lost",
        QMessageBox::Yes | QMessageBox::No );

    if (rejected == QMessageBox::Yes){ reject(); }
}

// prevents all days from being checked
void Dialog::fourDayChecker(QObject *sender, bool disabled){
    QCheckBox* checkbox = qobject_cast<QCheckBox*>(sender);

    if (checkbox->objectName() == "tuesday"){ ui->thursday->setDisabled(disabled); }

    else { ui->tuesday->setDisabled(disabled); }
}

void Dialog::onMWFStateChanged(int state)
{
    bool disabled = false;

    if (state == Qt::Checked) { mwf++;}
    else if (state == Qt::Unchecked) {mwf--;}

    if (mwf > 0) { disabled = true; }
    if (mwf == 3) { disabled = false; } // MTWF or MWRF are valid

    ui->tuesday->setDisabled(disabled);
    ui->tuesday->setChecked(false);

    ui->thursday->setDisabled(disabled);
    ui->thursday->setChecked(false);
}

void Dialog::onTRStateChanged(int state)
{
    bool disabled = false;

    if (state == Qt::Checked) { tuth++;}
    else if (state == Qt::Unchecked) { tuth--; }

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
void Dialog::startTimeChangeHandler(const QTime &time)
{
    QTime other_time = ui->time_stop->time();
    if (time.hour() >= other_time.hour()){
        other_time = time.addSecs(3600);  // Take time stop and add an hour
        ui->time_stop->setTime(other_time);
    }
}

// prevents time stop from being lower than time start
void Dialog::endTimeChangeHandler(const QTime &time)
{
    QTime other_time = ui->time_start->time();
    if (time.hour() <= other_time.hour()){
        other_time = time.addSecs(-3600);    // Take time start and subtract an hour
        ui->time_start->setTime(other_time);
    }
}

void Dialog::onlineStateChangeHandler(int state)
{
    // 0 = UNCHECKED | 2 = CHECKED
    switch(state){
        case 0:
            ui->building_name->setDisabled(false);
            break;
        case 2:
            ui->building_name->setDisabled(true);
            break;
        default:
            ui->building_name->setDisabled(false);
            break;
    }

}

