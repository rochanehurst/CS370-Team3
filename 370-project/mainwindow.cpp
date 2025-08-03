#include "mainwindow.h"
#include "search_window.h"
#include "ui_mainwindow.h"
#include "warning.h"
#include "savestuff.h"
#include "class_info_unit.h"

#include <QMenu>
#include <QFile>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QAction>
#include <QProcess>
#include <QProgressDialog>

string filename = "cluster_savedata.txt";
SaveFeature s(filename);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::main_window)
{
    ui_->setupUi(this);

#ifndef QT_DEBUG
    ui_->menuBar->removeAction(ui_->menuDebug->menuAction());
#endif

    setup();
    if (!filesystem::is_empty(filename)) {
        QStringList unparsed;
        ClassInfo data;

        s.loadSaveData(filename, unparsed);
        if (unparsed.isEmpty() != true) {
            for (qsizetype i = 0; i < unparsed.size(); i++) {
                QString oneUnparsed = unparsed.at(i);
                s.parseSavaData(filename, oneUnparsed, data);
                createClassFrame(data, true);
            }
        }
    }
}



MainWindow::~MainWindow() {
    s.clearAll(filename);
    for (int i = 0; i < class_list_layout_->count(); ++i) {
        QWidget* widget = class_list_layout_->itemAt(i)->widget();
        if (widget) {
            ClassInfoFrame* class_info = qobject_cast<ClassInfoFrame*>(widget);
            if (class_info) {
                s.addToSave(class_info->getFrameData(), filename);
            }
        }
    }
    s.closeFile();
    delete ui_;
}



void MainWindow::setup(){
    setupWidths();
    setupMenu();
    setupConnections();
    setupClassListLayout();
    setupClassSearch();
}



void MainWindow::setupWidths(){
    int warn_width = ui_->warning_container->maximumWidth();
    int schedule_width = ui_->class_container->maximumWidth();
    int map_width = ui_->map_placeholder->maximumWidth();

    ui_->warning_container->parentWidget()->setMaximumWidth(warn_width);
    ui_->schedule_title->parentWidget()->setMaximumWidth(schedule_width);
    ui_->map_placeholder->parentWidget()->setMaximumWidth(map_width);
}



void MainWindow::setupMenu(){
    // Create button options
    QMenu* menu = new QMenu(this);
    menu->addAction("Manually add class", this, &MainWindow::createClassButtonHandler);
    menu->addAction("Search for class", this, &MainWindow::searchClass);
    ui_->class_creator_button->setMenu(menu);
}



void MainWindow::setupConnections() {
    connect(ui_->class_creator_button, &QAbstractButton::clicked, this, &MainWindow::createClassButtonHandler);
    connect(ui_->clear_schedule_button, &QAbstractButton::clicked, this, &MainWindow::clearSchedule);
    connect(ui_->update_class_list, &QAbstractButton::clicked, this, &MainWindow::updateClassList);
    connect(ui_->actionPopulate_Schedule, &QAction::triggered, this, &MainWindow::debugPopulateList);
    connect(ui_->actionAdd_Random_Class, &QAction::triggered, this, &MainWindow::addRandomClass);
    connect(ui_->actionError, &QAction::triggered, this, &MainWindow::createError);
    connect(ui_->actionWarning, &QAction::triggered, this, &MainWindow::createWarning);
    connect(ui_->actionNotice, &QAction::triggered, this, &MainWindow::createNotice);
}



void MainWindow::setupClassListLayout() {
    class_list_layout_ = qobject_cast<QVBoxLayout*>(ui_->class_scroll_area->layout());
    if (!class_list_layout_) {
        class_list_layout_ = new QVBoxLayout(ui_->class_scroll_area);
        ui_->class_scroll_area->setLayout(class_list_layout_);
    }
    class_list_layout_->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    warning_list_layout_ = qobject_cast<QVBoxLayout*>(ui_->warning_scroll_area->layout());
    if (!warning_list_layout_) {
        warning_list_layout_ = new QVBoxLayout(ui_->warning_scroll_area);
        ui_->warning_scroll_area->setLayout(warning_list_layout_);
    }
    warning_list_layout_->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

}



void MainWindow::setupClassSearch(){
    search_classes_.clear();
    QString path = QCoreApplication::applicationDirPath() + "/../../class_finder/data/csusm_classes.csv";
    loadCSV(path);
}



void MainWindow::loadCSV(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << filename;
        return;
    }
    QTextStream in(&file);

    bool firstItem = true;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (firstItem) {     // Skips topic lines
            firstItem = false;
            continue;
        }
        if (fields.size() < 6) continue; // Safety check for malformed lines

        QStringList building_check = extractBuildings(fields[4].trimmed());
        if (building_check.isEmpty()) {
            continue;
        }
        ClassData new_class;
        ClassInfo class_conversion;

        QString name = fields[1].trimmed().simplified();
        name.replace(" | ", ", ");

        new_class.subject    = fields[0].trimmed().simplified();
        new_class.name       = name;
        new_class.class_code = fields[2].trimmed().simplified();
        new_class.instructors = extractInstructors(fields[3].trimmed());
        new_class.buildings   = building_check;

        class_conversion.name = new_class.name;
        class_conversion.building = new_class.buildings.join(", ");

        // Handle multiple day/time blocks
        QStringList differentTimes = fields[5].split('|', Qt::SkipEmptyParts);
        for (QString &times : differentTimes) {
            QStringList timeRange = times.split('-', Qt::SkipEmptyParts);

            if (timeRange.size() == 2) {
                QString left = timeRange[0].trimmed();   // "MoWe 2:00PM"
                QString right = timeRange[1].trimmed();  // "4:50PM"

                QStringList leftParts = left.split(' ', Qt::SkipEmptyParts);
                if (leftParts.size() >= 2) {
                    new_class.days.append(extractDays(leftParts[0]));
                    new_class.start.append(leftParts[1].trimmed());
                    new_class.end.append(right);
                }
            }
        }
        class_conversion.startTime = new_class.start.join(", ");
        class_conversion.endTime = new_class.end.join(", ");
        class_conversion.days = new_class.days.join(", ");
        new_class.data = class_conversion;
        search_classes_.append(new_class);
    }
}



QString MainWindow::extractDays(const QString &days) {
    static const QMap<QString, QChar> dayMap = {
        {"Mo", 'M'}, {"Tu", 'T'}, {"We", 'W'},
        {"Th", 'R'}, {"Fr", 'F'}, {"Sa", 'S'}, {"Su", 'U'}
    };

    QString new_days;
    for (int i = 0; i + 1 < days.length(); i += 2) {  // Ensure 2 chars exist
        QString dayCode = days.mid(i, 2);

        if (dayMap.contains(dayCode)) {
            new_days.append(dayMap.value(dayCode));
        } else {
            qDebug() << "Unknown day code:" << dayCode;
        }
    }
    return new_days;
}



QStringList MainWindow::extractInstructors(const QString &instructors) {
    QStringList differentInstructors = instructors.split('|', Qt::SkipEmptyParts);

    for (QString &inst : differentInstructors) {
        inst = inst.trimmed();
    }
    return differentInstructors;
}



QStringList MainWindow::extractBuildings(const QString &buildings) {
    QStringList differentBuildings = buildings.split('|', Qt::SkipEmptyParts);
    bool hasValid = false;

    // First check if at least one valid building exists
    for (QString &inst : differentBuildings) {
        QString trimmed = inst.trimmed();
        for (const QString &building : std::as_const(valid_buildings_)) {
            if (trimmed.contains(building, Qt::CaseInsensitive)) {
                hasValid = true;
                break;
            }
        }
        if (hasValid) break;
    }

    // If at least one valid building is found, return all (with cleanup)
    if (hasValid) {
        for (QString &inst : differentBuildings) {
            inst = inst.trimmed();
            if (inst == "Synchronous Virtual Instr") {
                inst = "ONLINE"; // normalize virtual classes
            }
        }
        return differentBuildings;
    }

    // Otherwise, return empty (class will be skipped)
    return {};
}



void MainWindow::createClassFrame(ClassInfo& class_info, bool loaded) {
    ClassInfoFrame* class_data = new ClassInfoFrame();
    class_data->createFrame(class_info);
    addClass(class_data, class_info, loaded);
}



void MainWindow::createClassButtonHandler() {
    Dialog class_creator;
    class_creator.setModal(true);
    if (class_creator.exec() == QDialog::Accepted) {
        ClassInfo classData = class_creator.getData();       // Retrieve data from the dialog
        createClassFrame(classData);
    }
}



void MainWindow::clearSchedule(bool test) {
    if (!test){
        auto clear_all = QMessageBox::question(
            this,
            "Clear All?",
            "Warning: Clearing all will permanently remove all data. Do you want to proceed?",
            QMessageBox::Yes | QMessageBox::No);
        if (class_list_layout_->count() == 0 || clear_all == QMessageBox::No) return;
    }
    QLayoutItem *child;
    while ((child = class_list_layout_->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    s.clearAll(filename);
}



void MainWindow::searchClass(){
    search_window *search_class = new search_window(search_classes_, this);
    search_class->setModal(false);
    search_class->show();
}



void MainWindow::updateClassList() {
    QString path = QCoreApplication::applicationDirPath() + "/../../class_finder/data_extractor.exe";

    // Progress dialog with determinate range
    QProgressDialog *progress = new QProgressDialog("Updating class list...", "Cancel", 0, 100, this);
    progress->setWindowModality(Qt::WindowModal);
    progress->setMinimumDuration(0);
    progress->setValue(0);

    QProcess *process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);

    connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
        QString output = process->readAllStandardOutput();

        for (const QString &line : output.split('\n', Qt::SkipEmptyParts)) {
            QString trimmed = line.trimmed();

            // Progress updates
            if (trimmed.startsWith("PROGRESS:")) {
                QString valueStr = trimmed.section(' ', 1, 1);
                bool ok;
                int percent = valueStr.toInt(&ok);
                if (ok) {
                    progress->setValue(percent);

                    // Keep the existing label text (log message)
                    QString currentText = progress->labelText();
                    if (!currentText.isEmpty() && currentText.contains("Processing:")) {
                        // Don't overwrite the log message; just update percentage visually
                    }
                }

                // Log messages update the label text
            } else if (trimmed.startsWith("LOG:")) {
                progress->setLabelText(
                    QString("Updating class list...\n%1")
                        .arg(trimmed.mid(4).trimmed())           // message without "LOG:"
                    );

            } else if (!trimmed.isEmpty()){
                qDebug() << "[data_extractor]:" << trimmed;
            }
        }
    });

    connect(process, &QProcess::finished, this, [=](int exitCode, QProcess::ExitStatus status) {
        progress->close();
        progress->deleteLater();

        if (exitCode == 0) {
            QMessageBox::information(this, "Done", "Class list updated successfully!");
            setupClassSearch();
        } else {
            QMessageBox::warning(this, "Error", "Update failed!");
        }
    });

    connect(progress, &QProgressDialog::canceled, process, &QProcess::kill);
    process->start(path);
}




void MainWindow::addClass(QWidget* class_to_add, const ClassInfo& info, bool loaded){
    class_list_layout_->addWidget(class_to_add);
}



void MainWindow::addWarning(QWidget* warning_to_add){
    ui_->warninglist->addWidget(warning_to_add);
}



// Below functions are for debug only
// ***MARKED FOR REMOVAL***
void MainWindow::addRandomClass(){
    int classValue = QRandomGenerator::global()->bounded(search_classes_.size());
    createClassFrame(search_classes_[classValue].data);
}



void MainWindow::debugPopulateList() {
    clearSchedule(true);
    int randomValue = QRandomGenerator::global()->bounded(2, 8);
    for (;randomValue > 0; randomValue--) {
        int classValue = QRandomGenerator::global()->bounded(search_classes_.size());
        createClassFrame(search_classes_[classValue].data);
    }
}



void MainWindow::createError(){
    Warning* new_warning = new Warning(WarningLevel::ERROR);
    addWarning(new_warning);
}



void MainWindow::createWarning(){
    Warning* new_warning = new Warning(WarningLevel::WARNING);
    addWarning(new_warning);
}



void MainWindow::createNotice(){
    Warning* new_warning = new Warning(WarningLevel::NOTICE);
    addWarning(new_warning);
}




