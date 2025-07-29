#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "createclass.h"

#include <QMainWindow>
#include <QDialog>
#include <QVector>
#include <QVBoxLayout>
#include <QDebug>

struct ClassData {
    QString subject;
    QString name;
    QString class_code;
    QStringList instructors;
    QStringList buildings;
    QStringList days;
    QStringList start;
    QStringList end;

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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void createClassButtonHandler();
    void createClassFrame(const ClassInfo& class_info);

    void editSave();
    void removeFromSave();
    void clearSchedule();
    void searchClass();

    void debugPopulateList();

private:
    void setupConnections();
    void setupClassListLayout();
    void setupMenu();
    void setupClassSearch();

    void loadCSV(const QString& filePath);
    QString extractDays(const QString& days);
    QStringList extractInstructors(const QString& instructors);
    QStringList extractBuildings(const QString& buildings);

    void debugAddClasstoList(ClassInfo* tester);
    ClassInfo* debugCreateClass(QString name,
                          QString days,
                          QString start,
                          QString end,
                          QString building);


private:
    Ui::main_window *ui_;
    QVector<ClassInfo> class_infos_;
    QVBoxLayout* class_list_layout_;
    QVector<ClassData> search_classes_;
    QStringList valid_buildings_ = {"Academic Hall", "Arts Building", "Extended Learning Building",
                                    "Kellogg Library", "Markstein Hall", "Science Hall",
                                    "Science Hall II", "Soc & Behavioral Sci Bldg",
                                    "University Hall","Viasat Engineering Pav", "Synchronous Virtual Instr"};
};

#endif // MAINWINDOW_H
