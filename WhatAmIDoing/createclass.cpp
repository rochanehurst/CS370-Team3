#include "createclass.h"
#include "ui_createclass.h"

#include <QStringListModel>
#include <QMessageBox>
#include <QDebug>



Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    model = new QStringListModel(this);
    model->setStringList(days);
    ui->dayList->setModel(model);
    setWindowTitle("Create Class");

    // Allows pop up warnings without closing window
    disconnect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    disconnect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::update()
{
    QStringList temp = days;
    days = {};

    // Organize days by custom order
    // BONUS: Also Prevents Duplicates (Which was not intended by I will take it)
    for (const QString &i : dayOrder){
        if (temp.contains(i)){
            days.append(i);
        }
    }
    model->setStringList(days);
}

void Dialog::on_addDay_clicked()
{
    auto day = ui->dayComboBox->currentText();
    if (!dayOrder.contains(day)) { return; }    // Limits selection to days
    if (days.contains(day)) { return; }        // Prevents entry duplicates

    days.append(day);
    update();
}

void Dialog::on_removeDay_clicked()
{
    QModelIndex index = ui->dayList->currentIndex();

    if (!index.isValid()) { return; }

    QString day = index.data().toString();
    int position = days.indexOf(day);

    if (position < 0 || position > 4) { return; } // Should never be more than 5 items

    days.removeAt(position);
    update();
}


void Dialog::on_buttonBox_accepted()
{
    if ((days.size()) < 1){
        QMessageBox::warning(this, "Error", "Select at least one day");
        return;
    }
    accept();
}


void Dialog::on_buttonBox_rejected()
{
    auto accepted = QMessageBox::question(
        this,
        "Cancel?",
        "Are you sure you want to cancel?\nAll data will be lost",
        QMessageBox::Yes | QMessageBox::No
        );

    if (accepted == QMessageBox::Yes){
        reject();
    }
}

