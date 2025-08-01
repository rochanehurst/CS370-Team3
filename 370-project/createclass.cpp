// createclass.cpp
#include "createclass.h"
#include "ui_createclass.h"

#include <QMessageBox>

constexpr auto TUESDAY_OBJ_NAME = "tuesday";
constexpr auto THURSDAY_OBJ_NAME = "thursday";

int mwf = 0;
int tuth = 0;
int sa = 0;


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
    declareCheckboxes();
    setupConnections();
    setupCheckboxes();
    resetCounters();
    editClassFrame(info);
}

Dialog::~Dialog() {
    delete ui_;
}

void Dialog::declareCheckboxes(){
    dayHandlers = {
        { ui_->monday,    [this](int state){ onMWFStateChanged(state); } },
        { ui_->wednesday, [this](int state){ onMWFStateChanged(state); } },
        { ui_->friday,    [this](int state){ onMWFStateChanged(state); } },
        { ui_->tuesday,   [this](int state){ onTRStateChanged(state); } },
        { ui_->thursday,  [this](int state){ onTRStateChanged(state); } },
        { ui_->saturday,  [this](int state){ onSaStateChanged(state); } }
    };
}

// Allows pop up warnings without closing window
void Dialog::setupConnections() {
    disconnect(ui_->confirm_button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    disconnect(ui_->confirm_button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(ui_->confirm_button_box, &QDialogButtonBox::accepted, this, &Dialog::handleConfirmAccepted);
    connect(ui_->confirm_button_box, &QDialogButtonBox::rejected, this, &Dialog::handleConfirmRejected);
    connect(ui_->time_start, &QTimeEdit::userTimeChanged, this, &Dialog::startTimeChangeHandler);
    connect(ui_->time_stop, &QTimeEdit::userTimeChanged, this, &Dialog::endTimeChangeHandler);

    // MWF
    for (QCheckBox* box : {ui_->monday, ui_->wednesday, ui_->friday}) {
        connect(box, &QCheckBox::checkStateChanged, this, &Dialog::onMWFStateChanged);
    }

    // TR
    for (QCheckBox* box : {ui_->tuesday, ui_->thursday}) {
        connect(box, &QCheckBox::checkStateChanged, this, &Dialog::onTRStateChanged);
    }

    // Saturday
    connect(ui_->saturday, &QCheckBox::checkStateChanged, this, &Dialog::onSaStateChanged);
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
    sa = 0;
}

void Dialog::handleConfirmAccepted() {
    if (ui_->building_name->currentText() == "--SELECT BUILDING--"){
        QMessageBox::critical(this, "Error", "Select a building");
        return;
    }
    if (ui_->class_name->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "Enter class name");
        return;
    }
    if ((mwf + tuth + sa) <= 0) {
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
        mwf++; sa++;
    }
    else if (state == Qt::Unchecked) {
        mwf--; sa--;
    }

    bool sa_disabled = (sa > 0);
    bool disabled = (mwf > 0 && mwf != 3);
    // MTWF or MWRF are valid
    ui_->tuesday->setDisabled(disabled);
    ui_->tuesday->setChecked(false);

    ui_->thursday->setDisabled(disabled);
    ui_->thursday->setChecked(false);

    if (sender()->objectName() == "friday") {
        ui_->saturday->setDisabled(false);
    }
    else {
    ui_->saturday->setDisabled(sa_disabled);
    }
}

void Dialog::onTRStateChanged(int state) {
    if (state == Qt::Checked) {
        tuth++; sa++;
    }
    else if (state == Qt::Unchecked) {
        tuth--; sa--;
    }

    bool sa_disabled = (sa > 0);
    bool disabled = (tuth > 0);

    if (mwf == 3) {
        fourDayChecker(sender(), disabled);
        disabled = false;
    }   // MTWF or MWRF are valid
    ui_->monday->setDisabled(disabled);
    ui_->wednesday->setDisabled(disabled);
    ui_->friday->setDisabled(disabled);
    ui_->saturday->setDisabled(sa_disabled);
}

void Dialog::onSaStateChanged(int state){
    bool disabled = (state ? Qt::Checked : Qt::Unchecked);
    if (disabled) sa++;
    else sa--;
    for (int i = 0; i < checkboxes_.size(); ++i) {
        QCheckBox* day = checkboxes_.at(i);
        if (day->objectName() == "saturday" || day->objectName() == "friday") continue;
        day->setDisabled(disabled);
    }
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

void Dialog::createClassForm() {
    class_form_info_.name = ui_->class_name->text();
    class_form_info_.building = ui_->building_name->currentText();
    class_form_info_.startTime = ui_->time_start->text();
    class_form_info_.endTime = ui_->time_stop->text();
    class_form_info_.days = dayStringCreate();

    resetCounters();
}

void Dialog::editClassFrame(const ClassInfo& classFormInfo){
    QMap<QChar, QCheckBox*> dayMap {
        { 'M', ui_->monday },
        { 'T', ui_->tuesday },
        { 'W', ui_->wednesday },
        { 'R', ui_->thursday },
        { 'F', ui_->friday },
        { 'S', ui_->saturday }
    };

    ui_->class_name->setText(classFormInfo.name);

    int building_index = ui_->building_name->findText(classFormInfo.building);
    if (building_index < 0) {
        ui_->building_name->addItem(classFormInfo.building);
        building_index = ui_->building_name->findText(classFormInfo.building);
    }
    ui_->building_name->setCurrentIndex(building_index);

    QString format = "h:mmA";
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
    if (tuesday)     ui_->tuesday->setCheckState(Qt::Checked);
    if (thursday)    ui_->thursday->setCheckState(Qt::Checked);

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

