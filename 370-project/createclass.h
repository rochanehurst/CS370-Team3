// createclass.h
#ifndef CREATECLASS_H
#define CREATECLASS_H

#include <QDialog>
#include <QCheckBox>
#include <QDebug>

struct ClassInfo {
    QString name;
    QString building;
    QString startTime;
    QString endTime;
    QString days;

    void print() const {
        qDebug() << "\nName: " << name
                 << "\nBuilding: " << building
                 << "\nStarts at: " << startTime
                 << "\nEnds at:" << endTime
                 << "\nOn days: " << days;
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
    void handleConfirmAccepted();
    void handleConfirmRejected();

    void onMWFStateChanged(int state);
    void onTRStateChanged(int state);
    void onSaStateChanged(int state);

    void startTimeChangeHandler(const QTime &time);
    void endTimeChangeHandler(const QTime &time);

private:
    void declareCheckboxes();
    void setupConnections();
    void setupCheckboxes();
    void resetCounters();

    void createClassForm();
    void editClassFrame(const ClassInfo& class_form_info);
    void fourDayChecker(QObject *sender, bool disabled);
    QString dayStringCreate() const;

private:
    Ui::Dialog *ui_ {};
    QVector<QCheckBox*> checkboxes_;
    ClassInfo class_form_info_;
    QMap<QCheckBox*, std::function<void(int)>> dayHandlers;

};

#endif // CREATECLASS_H
