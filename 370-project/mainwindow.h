#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "createclass.h"
#include "schedulelogic.h"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QDebug>
#include <QQuickWidget> //for map

using namespace std;

struct ClassData {
    QString subject;
    QString name;
    QString class_code;
    QStringList instructors;
    QStringList buildings;
    QStringList days;
    QStringList start;
    QStringList end;
    ClassInfo data;

    void print() const {
        qDebug() << "\nSubject: " << subject
                 << "\nClass Name: " << name
                 << "\nClass Code: " << class_code
                 << "\nInstructors: " << instructors
                 << "\nBuilding: " << buildings
                 << "\nDays: " << days
                 << "\nStart: " << start
                 << "\nEnd: " << end;
    }
};

QT_BEGIN_NAMESPACE
namespace Ui {

class main_window;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum class WarningLevel {
        NOTICE,
        WARNING,
        ERROR
    };
    Q_ENUM(WarningLevel)

public:
    MainWindow(QWidget *parent = nullptr);
    void createClassFrame(ClassInfo& class_info, bool loaded = false);
    ~MainWindow();

public:
    QVector<ClassInfo> getCurrentScheduleData() const { return current_schedule_data_; }


private slots:
    void onClassRemoved(ClassInfoFrame* w);
    void createClassButtonHandler();
    void clearSchedule(bool test = false);
    void searchClass();
    void debugPopulateList();
    void addRandomClass();
    void updateClassList();

private:
    void setupConnections();
    void setupClassListLayout();
    void setupMenu();
    void setupClassSearch();
    void setupWidths();
    void setup();
    void addClass(QWidget* class_to_add, const ClassInfo& info, bool loaded = false);
    void resetVectors();

    void loadCSV(const QString& filePath);
    QString extractDays(const QString& days);
    QStringList extractInstructors(const QString& instructors);
    QStringList extractBuildings(const QString& buildings);

    void initMap(); //initialize map

private:
    Ui::main_window *ui_;

    ScheduleLogic schedule_logic_;
    QVector<ClassInfo> current_schedule_data_;

    QVector<ClassInfo> class_infos_;
    QVBoxLayout* class_list_layout_;
    QVBoxLayout* warning_list_layout_;
    QVector<ClassData> search_classes_;
    QStringList valid_buildings_ = {"Academic Hall", "Arts Building", "Extended Learning Building",
                                    "Kellogg Library", "Markstein Hall", "Science Hall",
                                    "Science Hall II", "Soc & Behavioral Sci Bldg",
                                    "University Hall","Viasat Engineering Pav", "Synchronous Virtual Instr"};
    QQuickWidget* mapWidget = nullptr;
    ApiMap* apiMap_ = nullptr;  // added for apimap
};

#endif // MAINWINDOW_H
