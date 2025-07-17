#ifndef CREATECLASS_H
#define CREATECLASS_H

#include <QDialog>
#include <QStringListModel>
#include <QCheckBox>

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
    ~Dialog();

    ClassInfo getData() const;

private slots:

    void on_confirmButtonBox_accepted();

    void on_confirmButtonBox_rejected();

    void fourDayChecker(QObject *sender, bool disable);

    void onMWFStateChanged(int arg1);

    void onTRStateChanged(int arg1);

    void createClass();

    QString dayStringCreate();

    void on_timeStart_userTimeChanged(const QTime &time);

    void on_timeStop_userTimeChanged(const QTime &time);

    void on_onlineCheckBox_stateChanged(int arg1);

private:
    Ui::Dialog *ui;
    QStringListModel *model;
    QVector<QCheckBox*> checkboxes;
    ClassInfo classInfo;
};

#endif // CREATECLASS_H
