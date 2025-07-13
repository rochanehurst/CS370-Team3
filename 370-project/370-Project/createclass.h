#ifndef CREATECLASS_H
#define CREATECLASS_H

#include <QDialog>
#include <QStringListModel>
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
    void on_addDay_clicked();

    void on_removeDay_clicked();

    void update();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Dialog *ui;
    QStringListModel *model;
    QStringList days = {};
    const QStringList dayOrder = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
};

#endif // CREATECLASS_H
