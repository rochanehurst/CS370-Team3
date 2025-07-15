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
        qDebug() << "School: " << school
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

private slots:

    void on_confirmButtonBox_accepted();

    void on_confirmButtonBox_rejected();

    void fourDayChecker(QObject *sender, bool disable);

    void on_MWF_stateChanged(int arg1);

    void on_TR_stateChanged(int arg1);

    void createClass();

    QString dayStringCreate();

    void on_timeStart_userTimeChanged(const QTime &time);

    void on_timeStop_userTimeChanged(const QTime &time);

private:
    Ui::Dialog *ui;
    QStringListModel *model;
    QVector<QCheckBox*> checkboxes;
    vector<ClassInfo*> classes;
};

#endif // CREATECLASS_H
