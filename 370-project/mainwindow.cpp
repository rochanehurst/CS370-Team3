#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "class_info_unit.h"
#include "search.h"

#include <QMenu>
#include <QFile>
#include <QLineEdit>
#include <QTextStream>
#include <QStringList>
#include <QIcon>
#include <QSize>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::main_window)
{
    ui_->setupUi(this);
    setupConnections();

    // Initalize layout in scroll area
    setupClassListLayout();
    setupMenu();
    setupClassSearch();
}

MainWindow::~MainWindow() {
    // TODO: Persist data to file before exit
    delete ui_;
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

void MainWindow::setupClassSearch(){
    QString path = QCoreApplication::applicationDirPath() + "/../../../data/csusm_classes.csv";
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
        new_class.subject    = fields[0].trimmed().simplified();
        new_class.name       = fields[1].trimmed().simplified();
        new_class.class_code = fields[2].trimmed().simplified();
        new_class.instructors = extractInstructors(fields[3].trimmed());
        new_class.buildings   = building_check;

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
        search_classes_.append(new_class);
    }
}


QString MainWindow::extractDays(const QString &days) {
    static const QMap<QString, QChar> dayMap = {
        {"Mo", 'M'}, {"Tu", 'T'}, {"We", 'W'},
        {"Th", 'R'}, {"Fr", 'F'}, {"Sa", 'S'}, {"Su", 'U'}
    };

    QString new_days;
    for (int i = 0; i + 1 < days.length(); i += 2) {  // ensure 2 chars exist
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
    bool valid = false;

    for (QString &inst : differentBuildings) {
        inst = inst.trimmed();
        for (const QString &building : std::as_const(valid_buildings_)) {
            if (inst.contains(building)) {
                if (inst == "Synchronous Virtual Instr"){
                    inst = "ONLINE";
                }
                valid = true;
            }
        }
    }

    if (valid){
        return differentBuildings;
    }

    return {};
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
    Dialog class_creator;
    class_creator.setModal(true);
    if (class_creator.exec() == QDialog::Accepted) {
        ClassInfo classData = class_creator.getData();       // Retrieve data from the dialog
        class_infos_.append(classData);                     // Store in MainWindow’s QVector
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

void MainWindow::searchClass(){
    search *search_class = new search(search_classes_, this);
    search_class->setModal(false);
    search_class->show();
}

// Below functions are for debug only
// ***MARKED FOR REMOVAL***
void MainWindow::debugAddClasstoList(ClassInfo* tester) {
    ClassInfoFrame* debug_data = new ClassInfoFrame();
    debug_data->createFrame(*tester);
    class_list_layout_->addWidget(debug_data);
}

ClassInfo* MainWindow::debugCreateClass(QString name,
                                  QString days,
                                  QString start,
                                  QString end,
                                  QString building) {
    ClassInfo* tester = new ClassInfo;
    tester->name = name;
    tester->days = days;
    tester->startTime = start;
    tester->endTime = end;
    tester->building = building;
    return tester;
}

void MainWindow::debugPopulateList() {

    debugAddClasstoList(debugCreateClass("testClass1", "M",
                                         "9:30 AM", "1:30 AM",
                                         "Social and Behavioral Sciences Building"));

    debugAddClasstoList(debugCreateClass("testClass2", "T",
                                         "1:30 PM", "4:45 PM",
                                         "University Hall"));

    debugAddClasstoList(debugCreateClass("testClass3", "MWF",
                                         "7:30 AM", "8:20 AM",
                                         "ONLINE CLASS"));

    debugAddClasstoList(debugCreateClass("testClass4", "TR",
                                         "5:00 AM", "6:50 AM",
                                         "Science Hall I"));

    debugAddClasstoList(debugCreateClass("testClass5", "F",
                                         "10:00 AM", "1:20 PM",
                                         "Arts Building"));
}


