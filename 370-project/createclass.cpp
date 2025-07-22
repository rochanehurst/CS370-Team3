// createclass.cpp
#include "createclass.h"
#include "ui_createclass.h"

#include <QStringListModel>
#include <QMessageBox>
#include <QDebug>
#include <QCheckBox>

constexpr auto TUESDAY_OBJ_NAME = "tuesday";
constexpr auto THURSDAY_OBJ_NAME = "thursday";

int mwf = 0;
int tuth = 0;

// Create form constructor
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui_(new Ui::Dialog)
{
    ui_->setupUi(this);
    setWindowTitle("Create Class");
    setupConnections();
    setupCheckboxes();
    resetCounters();
}

// Edit form constructor
Dialog::Dialog(const ClassInfo &info, QWidget *parent)
    : QDialog(parent)
    , ui_(new Ui::Dialog)
{
    ui_->setupUi(this);
    setWindowTitle("Edit Class");

    QIcon icon(":/icons/icons/sign-up-icon-signup-square-box-on-transparent-background-free-png.png");
    setWindowIcon(icon);
    setupConnections();
    setupCheckboxes();
    resetCounters();
    editClassFrame(info);
}

Dialog::~Dialog() {
    delete ui_;
}

// Allows pop up warnings without closing window
void Dialog::setupConnections() {
    disconnect(ui_->confirm_button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    disconnect(ui_->confirm_button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(ui_->confirm_button_box, &QDialogButtonBox::accepted, this, &Dialog::handleConfirmAccepted);
    connect(ui_->confirm_button_box, &QDialogButtonBox::rejected, this, &Dialog::handleConfirmRejected);
    connect(ui_->time_start, &QTimeEdit::userTimeChanged, this, &Dialog::startTimeChangeHandler);
    connect(ui_->time_stop, &QTimeEdit::userTimeChanged, this, &Dialog::endTimeChangeHandler);
    connect(ui_->online_check_box, &QCheckBox::checkStateChanged, this, &Dialog::onlineStateChangeHandler);

    connect(ui_->monday, &QCheckBox::checkStateChanged, this, &Dialog::onMWFStateChanged);
    connect(ui_->tuesday, &QCheckBox::checkStateChanged, this, &Dialog::onTRStateChanged);
    connect(ui_->wednesday, &QCheckBox::checkStateChanged, this, &Dialog::onMWFStateChanged);
    connect(ui_->thursday, &QCheckBox::checkStateChanged, this, &Dialog::onTRStateChanged);
    connect(ui_->friday, &QCheckBox::checkStateChanged, this, &Dialog::onMWFStateChanged);
}

void Dialog::setupCheckboxes() {
    // Create an array of checkboxes
    const auto& children = ui_->days->children();
    for (QObject* child : children) {
        if (QCheckBox* checkbox = qobject_cast<QCheckBox*>(child)) {
            checkboxes_.push_back(checkbox);
        }
    }
}

void Dialog::resetCounters() {
    mwf = 0;
    tuth = 0;
}

void Dialog::handleConfirmAccepted() {
    if (ui_->class_name->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "Enter class name");
        return;
    }
    if (ui_->building_name->text().isEmpty() && !ui_->online_check_box->isChecked()) {
        QMessageBox::critical(this, "Error", "Enter building name");
        return;
    }
    if ((mwf + tuth) <= 0) {
        QMessageBox::critical(this, "Error", "Select a day");
        return;
    }
    createClassForm();
    accept();
}


void Dialog::handleConfirmRejected() {
    auto rejected = QMessageBox::question(
        this,
        "Cancel?",
        "Are you sure you want to cancel?\nAll data will be lost",
        QMessageBox::Yes | QMessageBox::No );

    if (rejected == QMessageBox::Yes) {
        reject();
    }
}

void Dialog::onMWFStateChanged(int state) {
    if (state == Qt::Checked) {
        mwf++;
    }
    else if (state == Qt::Unchecked) {
        mwf--;
    }

    bool disabled = (mwf > 0 && mwf != 3);
    // MTWF or MWRF are valid
    ui_->tuesday->setDisabled(disabled);
    ui_->tuesday->setChecked(false);

    ui_->thursday->setDisabled(disabled);
    ui_->thursday->setChecked(false);
}

void Dialog::onTRStateChanged(int state) {
    if (state == Qt::Checked) {
        tuth++;
    }
    else if (state == Qt::Unchecked) {
        tuth--;
    }

    bool disabled = (tuth > 0);

    if (mwf == 3) {
        fourDayChecker(sender(), disabled);
        disabled = false;
    }   // MTWF or MWRF are valid
    ui_->monday->setDisabled(disabled);
    ui_->wednesday->setDisabled(disabled);
    ui_->friday->setDisabled(disabled);
}

// Prevents time start from being higher than time stop
void Dialog::startTimeChangeHandler(const QTime &time) {
    QTime other_time = ui_->time_stop->time();
    if (time.hour() >= other_time.hour()) {
        other_time = time.addSecs(3600);    // Take time stop and add an hour
        ui_->time_stop->setTime(other_time);
    }
}

// Prevents time stop from being lower than time start
void Dialog::endTimeChangeHandler(const QTime &time) {
    QTime other_time = ui_->time_start->time();
    if (time.hour() <= other_time.hour()) {
        other_time = time.addSecs(-3600);   // Take time start and subtract an hour
        ui_->time_start->setTime(other_time);
    }
}

void Dialog::onlineStateChangeHandler(int state) {
    switch(state) {
        case Qt::Unchecked:
            ui_->building_name->setDisabled(false);
            break;
        case Qt::Checked:
            ui_->building_name->setDisabled(true);
            break;
        default:
            ui_->building_name->setDisabled(false);
            break;
    } 
}

void Dialog::createClassForm() {
    class_form_info_.school = ui_->school_combo_box->currentText();
    class_form_info_.name = ui_->class_name->text();

    if (ui_->online_check_box->isChecked()) {
        class_form_info_.building = "Online Class";
    }
    else {
        class_form_info_.building = ui_->building_name->text();
    }
    class_form_info_.startTime = ui_->time_start->text();
    class_form_info_.endTime = ui_->time_stop->text();
    class_form_info_.days = dayStringCreate();
    class_form_info_.online = ui_->online_check_box->isChecked();

    resetCounters();
}

void Dialog::editClassFrame(const ClassInfo& classFormInfo){
    QMap<QChar, QCheckBox*> dayMap {
        { 'M', ui_->monday },
        { 'T', ui_->tuesday },
        { 'W', ui_->wednesday },
        { 'R', ui_->thursday },
        { 'F', ui_->friday }
    };

    ui_->class_name->setText(classFormInfo.name);
    ui_->building_name->setText(classFormInfo.building);

    // True: set Checked | False: set Unchecked
    ui_->online_check_box->setCheckState(
        classFormInfo.online ? Qt::Checked : Qt::Unchecked);

    QString format = "h:mm AP";
    ui_->time_start->setTime(QTime::fromString(classFormInfo.startTime, format));
    ui_->time_stop->setTime(QTime::fromString(classFormInfo.endTime, format));

    bool tuesday = false, thursday = false;
    for (const QChar &day : classFormInfo.days) {
        if (day == 'T') tuesday = true;
        if (day == 'R') thursday = true;
        if (day != 'T' && day != 'R'){
            dayMap[day]->setCheckState(Qt::Checked);
        }
    }
    if (tuesday) {
        ui_->tuesday->setCheckState(Qt::Checked);
    }
    if (thursday) {
        ui_->thursday->setCheckState(Qt::Checked);
    }
}

QString Dialog::dayStringCreate() const {
    QString dayString;
    for (int i = 0; i < checkboxes_.size(); ++i) {
        const QCheckBox* day = checkboxes_.at(i);
        if (day->isChecked()) {
            if (day->objectName() == THURSDAY_OBJ_NAME) {
                dayString.append('R');
            }
            else {
                const QString name = day->objectName();
                QChar extractAbbr = name.at(0).toUpper();
                dayString.append(extractAbbr);
            }
        }
    }
    return dayString;
}

// Prevents all days from being checked
void Dialog::fourDayChecker(QObject *sender, bool disabled) {
    QCheckBox* checkbox = qobject_cast<QCheckBox*>(sender);
    if (checkbox->objectName() == TUESDAY_OBJ_NAME) {
        ui_->thursday->setDisabled(disabled);
    }
    else {
        ui_->tuesday->setDisabled(disabled);
    }
}

ClassInfo Dialog::getData() const {
    return class_form_info_;
}

