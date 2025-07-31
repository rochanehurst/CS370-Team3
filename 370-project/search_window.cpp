#include "search_window.h"
#include "ui_search_window.h"
#include "class_result.h"

#include <QFile>

search_window::search_window(QVector<ClassData> classes_, QWidget *parent)
    : QDialog(parent)
    , ui_(new Ui::search_window)
{
    ui_->setupUi(this);

    ui_->no_classes->show();

    search_classes_ = classes_;

    QString path = QCoreApplication::applicationDirPath() + "/../../data/subjects.csv";
    loadCSV(path);
    declareCheckboxes();
    setupConnections();
    class_list_layout_ = qobject_cast<QVBoxLayout*>(ui_->scrollAreaWidgetContents->layout());
    day_boxes = {{
        {ui_->monday, 'M'},
        {ui_->tuesday, 'T'},
        {ui_->wednesday, 'W'},
        {ui_->thursday, 'R'},
        {ui_->friday, 'F'},
        {ui_->saturday, 'S'}
    }};
}

search_window::~search_window()
{
    delete ui_;
}

void search_window::declareCheckboxes() {
    dayHandlers = {
        { ui_->monday,    [this](int state) { updateSearch(); } },
        { ui_->wednesday, [this](int state) { updateSearch(); } },
        { ui_->friday,    [this](int state) { updateSearch(); } },
        { ui_->tuesday,   [this](int state) { updateSearch(); } },
        { ui_->thursday,  [this](int state) { updateSearch(); } },
        { ui_->saturday,  [this](int state) { updateSearch(); } }
    };
}

void search_window::setupConnections() {
    connect(ui_->subject_combo_box, &QComboBox::currentIndexChanged, this, &search_window::updateSearch);
    connect(ui_->building_combo_box, &QComboBox::currentIndexChanged, this, &search_window::updateSearch);
    connect(ui_->input_class, &QLineEdit::textEdited, this, &search_window::updateSearch);
    connect(ui_->time_startafter, &QTimeEdit::userTimeChanged, this, &search_window::updateSearch);
    connect(ui_->time_endbefore, &QTimeEdit::userTimeChanged, this, &search_window::updateSearch);

    for (auto box = dayHandlers.begin(); box != dayHandlers.end(); ++box) {
        connect(box.key(), &QCheckBox::checkStateChanged, this, box.value());
    }
}

void search_window::loadCSV(const QString& filename) {
    ui_->subject_combo_box->addItem("NO SUBJECT SELECTED");
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file: " << filename;
        return;
    }

    QTextStream in(&file);
    while(!in.atEnd()) {
        QString subject = in.readLine();
        ui_->subject_combo_box->addItem(subject);
    }

    file.close();
}

void search_window::applyFilters() {
    QVector<ClassData> new_classes;

    const QString subject = ui_->subject_combo_box->currentText();
    const QString building = ui_->building_combo_box->currentText();
    const QString name = ui_->input_class->text();

    // Build checked days
    QString checked_days;
    for (auto &[box, dayChar] : day_boxes) {
        if (box->isChecked()) checked_days.append(dayChar);
    }

    QTime start = ui_->time_startafter->time();
    QTime end   = ui_->time_endbefore->time();

    for (const ClassData &class_data : std::as_const(search_classes_)) {
        // Filter by subject
        if (subject != "NO SUBJECT SELECTED" && class_data.subject != subject)
            continue;

        // Filter by building
        if (building != "NO BUILDING SELECTED") {
            bool match = false;
            for (const QString &loc : class_data.buildings) {
                QRegularExpression regex("\\b" + QRegularExpression::escape(building) + "\\b");
                if (regex.match(loc).hasMatch()) {
                    match = true;
                    break;
                }
            }
            if (!match) continue;
        }

        // Filter by days
        if (!checked_days.isEmpty()) {
            QString all_days;
            for (const QString &day : class_data.days)
                all_days.append(day);
            bool all_found = true;
            for (QChar d : checked_days)
                if (!all_days.contains(d)) { all_found = false; break; }
            if (!all_found) continue;
        }

        // Filter by time range
        bool in_time_range = true;
        for (int i = 0; i < class_data.start.size(); i++) {
            QTime start_compare = QTime::fromString(class_data.start[i], "h:mma");
            QTime end_compare   = QTime::fromString(class_data.end[i], "h:mma");
            if (start_compare < start || end_compare > end) {
                in_time_range = false;
                break;
            }
        }
        if (!in_time_range) continue;

        if (!class_data.name.contains(name)) continue;

        // Passed all filters
        new_classes.append(class_data);
    }

    filtered_classes_ = new_classes;
}

void search_window::createWidgets(const QVector<ClassData> &data) {
    // Clear existing widgets
    QLayoutItem *child;
    while ((child = ui_->scroll_list->takeAt(0)) != nullptr) {
        if (child->widget() && child->widget() != ui_->no_classes) {
            delete child->widget();
        }
        delete child;
    }

    if (data.isEmpty()) {
        ui_->no_classes->show();
        return;
    }
    ui_->no_classes->hide();

    for (const ClassData &item : data) {
        ui_->scroll_list->addWidget(new class_result(item));
    }
    ui_->scroll_list->addStretch();
}


void search_window::updateSearch() {
    filtered_classes_.clear();
    applyFilters();
    createWidgets(filtered_classes_);
}



void search_window::on_reset_time_clicked()
{
    QTime start_time = QTime::fromString("6:00 AM", "h:mm AP");
    QTime end_time = QTime::fromString("10:00 PM", "h:mm AP");
    ui_->time_startafter->setTime(start_time);
    ui_->time_endbefore->setTime(end_time);

    filtered_classes_.clear();
    createWidgets(filtered_classes_);
}

