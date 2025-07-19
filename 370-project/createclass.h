// createclass.h
#ifndef CREATECLASS_H
#define CREATECLASS_H

#include <QDialog>
#include <QStringListModel>
#include <QCheckBox>
#include <QDebug>

using namespace std;

struct ClassInfo {
    QString school;
    QString name;
    QString building;
    QString startTime;
    QString endTime;
    QString days;
    bool online;

    void print() const{
        qDebug() << "\nSchool: " << school
                 << "\nName: " << name
                 << "\nBuilding: " << building
                 << "\nStarts at: " << startTime
                 << "\nEnds at:" << endTime
                 << "\nOn days: " << days
                 << "\nIs Online?: " << online;
    }
};

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);

    explicit Dialog(const ClassInfo &info, QWidget *parent = nullptr);

    ~Dialog();

    ClassInfo getData() const;

private slots:
    void createClass();

    void editClass(ClassInfo info);

    void handleConfirmAccepted();

    void handleConfirmRejected();

    void fourDayChecker(QObject *sender, bool disable);

    void onMWFStateChanged(int arg1);

    void onTRStateChanged(int arg1);

    void setupConnections();

    void setupCheckboxes();

    void resetCounters();

    void startTimeChangeHandler(const QTime &time);

    void endTimeChangeHandler(const QTime &time);

    void onlineStateChangeHandler(int arg1);

    QString dayStringCreate();

private:
    Ui::Dialog *ui;

    QStringListModel *model;

    QVector<QCheckBox*> checkboxes;

    ClassInfo classInfo;

};

#endif // CREATECLASS_H
