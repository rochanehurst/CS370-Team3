#include "search_window.h"
#include "ui_search_window.h"
#include "class_result.h"

#include <QFile>
#include <QTimer>

search_window::search_window(QVector<ClassData> classes_, QWidget *parent)
    : QDialog(parent)
    , ui_(new Ui::search_window)
{
    ui_->setupUi(this);
    ui_->progressBar->hide();
    search_classes_ = classes_;

    filtered_classes_ = search_classes_;

    QString path = QCoreApplication::applicationDirPath() + "/../../class_finder/subjects.csv";
    loadCSV(path);
    declareCheckboxes();
    setupConnections();
    day_boxes = {{
        {ui_->monday, 'M'},
        {ui_->tuesday, 'T'},
        {ui_->wednesday, 'W'},
        {ui_->thursday, 'R'},
        {ui_->friday, 'F'},
        {ui_->saturday, 'S'}
    }};
    // Show all classes initially
    QTimer::singleShot(0, this, [this]() {
        updateSearch();
    });
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

    const QString subject   = ui_->subject_combo_box->currentText();
    const QString building  = ui_->building_combo_box->currentText();
    const QString name      = ui_->input_class->text();

    // Build checked days
    QString checked_days;
    for (auto &[box, dayChar] : day_boxes) {
        if (box->isChecked()) checked_days.append(dayChar);
    }

    QTime start = ui_->time_startafter->time();
    QTime end   = ui_->time_endbefore->time();

    // *** CHECK FOR NO FILTERS ***
    bool no_filters = (subject == "NO SUBJECT SELECTED" &&
                       building == "NO BUILDING SELECTED" &&
                       name.isEmpty() &&
                       checked_days.isEmpty() &&
                       start == default_start &&
                       end == default_end);

    if (no_filters) {
        // Show all classes
        new_classes = search_classes_;
    } else {
        // Apply filters normally
        for (const ClassData &class_data : std::as_const(search_classes_)) {
            // Subject filter
            if (subject != "NO SUBJECT SELECTED" && class_data.subject != subject)
                continue;

            // Building filter
            if (building != "NO BUILDING SELECTED") {
                bool match = false;
                for (const QString &loc : class_data.buildings) {
                    if (loc.contains(building, Qt::CaseInsensitive)) {
                        match = true;
                        break;
                    }
                }
                if (!match) continue;
            }

            // Days filter
            if (!checked_days.isEmpty()) {
                QString all_days;
                for (const QString &day : class_data.days) all_days.append(day);
                bool all_found = true;
                for (QChar d : checked_days)
                    if (!all_days.contains(d)) { all_found = false; break; }
                if (!all_found) continue;
            }

            bool matches_time = false;
            for (int i = 0; i < class_data.start.size(); i++) {
                QTime start_compare = QTime::fromString(class_data.start[i], "h:mmA");
                QTime end_compare   = QTime::fromString(class_data.end[i], "h:mmA");

                if (start_compare.isValid() && end_compare.isValid()) {
                    if (start_compare >= start && end_compare <= end) {
                        matches_time = true;
                        break; // at least one valid time range matches
                    }
                } else {
                    // If invalid times (like placeholder 12:00AM-12:00AM), allow it by default
                    matches_time = true;
                    break;
                }
            }
            if (!matches_time) continue;


            // Name filter
            if (!name.isEmpty() &&
                !class_data.name.contains(name, Qt::CaseInsensitive))
                continue;

            // Passed all filters
            new_classes.append(class_data);
        }
    }

    filtered_classes_ = new_classes;
}



void search_window::createWidgets(const QVector<ClassData> &data) {
    // Show "No Classes Found" if empty
    if (data.isEmpty()) {
        return;
    }
    // Add widgets with progress updates
    for (int i = 0; i < data.size(); ++i) {
        ui_->scroll_list->addWidget(new class_result(data[i]));

        // Update progress bar
        if ((i % 5) == 0 || i == data.size() - 1) {
            ui_->progressBar->setValue(i + 1);
            QApplication::processEvents();
        }
    }

    ui_->scroll_list->addStretch();
}



void search_window::updateSearch() {
    filtered_classes_.clear();
    applyFilters();

    // Show progress bar
    ui_->progressBar->setRange(0, filtered_classes_.size());
    ui_->progressBar->setValue(0);
    ui_->progressBar->show();

    // Show Loading... and hide the entire scroll area
    ui_->no_classes->setText("Loading...");
    ui_->no_classes->show();
    ui_->results->hide();   // <--- hide the scroll area completely

    QApplication::processEvents(); // let "Loading..." paint

    // Disable updates and clear previous widgets
    ui_->scroll_list->parentWidget()->setUpdatesEnabled(false);

    QLayoutItem *child;
    while ((child = ui_->scroll_list->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }

    // Build new results
    createWidgets(filtered_classes_);

    // Re-enable updates
    ui_->scroll_list->parentWidget()->setUpdatesEnabled(true);

    // Swap back to the correct view
    if (filtered_classes_.isEmpty()) {
        ui_->no_classes->setText("No Classes Found");
        ui_->no_classes->show();
        ui_->results->hide();
    } else {
        ui_->no_classes->hide();
        ui_->results->show();
    }

    ui_->progressBar->hide();
}



void search_window::on_reset_time_clicked()
{
    QTime start_time = default_start;
    QTime end_time = default_end;
    ui_->time_startafter->setTime(start_time);
    ui_->time_endbefore->setTime(end_time);

    filtered_classes_.clear();
    createWidgets(filtered_classes_);
}

