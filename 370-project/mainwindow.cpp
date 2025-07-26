#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "class_info_unit.h"
#include "search.h"

#include <QMenu>
#include <QIcon>
#include <QSize>
#include <QWidget>
#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::main_window)
{
    ui_->setupUi(this);
    setupConnections();

    // Initalize layout in scroll area
    setupClassListLayout();
    setupMenu();
    initMap();
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
    search search_class;
    search_class.setModal(true);
    search_class.exec();
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
                                         "Social and Behavioral Sciences Building (SBSB)"));

    debugAddClasstoList(debugCreateClass("testClass2", "T",
                                         "1:30 PM", "4:45 PM",
                                         "University Hall (UNIV)"));

    debugAddClasstoList(debugCreateClass("testClass3", "MWF",
                                         "7:30 AM", "8:20 AM",
                                         "ONLINE CLASS"));

    debugAddClasstoList(debugCreateClass("testClass4", "TR",
                                         "5:00 AM", "6:50 AM",
                                         "Science Hall I (SCI 1)"));

    debugAddClasstoList(debugCreateClass("testClass5", "F",
                                         "10:00 AM", "1:20 PM",
                                         "Arts Building (ARTS)"));
}

//added by Raymond Las Pinas
//function that uses the placeholder in mainwindow.ui and initializes a map
void MainWindow::initMap() {
    // get placeholder widget where the map'll be inserted
    QWidget* mapPlaceholder = ui_->map_placeholder;
    if (!mapPlaceholder) { //for debugging
        qWarning("Map placeholder widget not found!");
        return;
    }

    //get the parent widget containing placeholder
    QWidget* parentWidget = mapPlaceholder->parentWidget();
    if (!parentWidget) { //for debugging
        qWarning("Map placeholder has no parent!");
        return;
    }

    //get layout of parent widget
    QLayout* layout = parentWidget->layout();
    if (!layout) { //for debugging
        qWarning("Parent widget has no layout!");
        return;
    }

    // casting layout into a QGridLayout
    QGridLayout* gridLayout = qobject_cast<QGridLayout*>(layout);
    if (!gridLayout) { //for debugging
        qWarning("Parent layout is not a QGridLayout!");
        return;
    }

    // find the placeholder's position in the grid
    int row, column, rowSpan, columnSpan;
    bool found = false;
    for (int i = 0; i < gridLayout->count(); ++i) {
        QLayoutItem* item = gridLayout->itemAt(i);
        if (item->widget() == mapPlaceholder) {
            gridLayout->getItemPosition(i, &row, &column, &rowSpan, &columnSpan);
            found = true;
            break;
        }
    }
    if (!found) { //for debugging
        qWarning("Map placeholder not found in grid layout!");
        return;
    }

    // create the map widget
    mapWidget = new QQuickWidget(this);
    mapWidget->setResizeMode(QQuickWidget::SizeRootObjectToView); //resize map
    mapWidget->setSource(QUrl(QStringLiteral("../../map.qml"))); //load map file

    // remove and hide placeholder widget
    gridLayout->removeWidget(mapPlaceholder);
    mapPlaceholder->hide();
    mapPlaceholder->setParent(nullptr); //disconnect from layout

    // add map widget to exact position as the placeholder
    gridLayout->addWidget(mapWidget, row, column, rowSpan, columnSpan);

    //show map
    mapWidget->show();

}


