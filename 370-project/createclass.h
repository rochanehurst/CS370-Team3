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
    void createClassForm();

    void editClassFrame(ClassInfo class_form_info);

    void handleConfirmAccepted();

    void handleConfirmRejected();

    void fourDayChecker(QObject *sender, bool disabled);

    void onMWFStateChanged(int state);

    void onTRStateChanged(int state);

    void setupConnections();

    void setupCheckboxes();

    void resetCounters();

    void startTimeChangeHandler(const QTime &time);

    void endTimeChangeHandler(const QTime &time);

    void onlineStateChangeHandler(int state);

    QString dayStringCreate();

private:
    Ui::Dialog *ui;

    QStringListModel *model;

    QVector<QCheckBox*> checkboxes;

    ClassInfo class_form_info;

};

#endif // CREATECLASS_H
