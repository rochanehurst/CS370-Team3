#include "createclass.h"
#include "ui_createclass.h"

#include <QStringListModel>
#include <QMessageBox>




Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    model = new QStringListModel(this);
    model->setStringList(items);
    ui->listView->setModel(model);
    setWindowTitle("Create Class");

    disconnect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    disconnect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::update()
{
    QStringList temp = items;
    items = {};

    // organize days by custom order
    // BONUS: Also Prevents Duplicates (Which was not intended by I will take it)
    for (QString i : dayOrder){
        if (temp.contains(i)){
            items.append(i);
        }
    }
    model->setStringList(items);
}

void Dialog::on_addDay_clicked()
{
    items.append(ui->comboBox->currentText());
    update();
}

void Dialog::on_removeDay_clicked()
{
    try {
        QModelIndex index = ui->listView->currentIndex();
        QString day = index.data().toString();
        int position = items.indexOf(day);
        items.removeAt(position);
        update();
    } catch (...) {
        return;
    }
}


void Dialog::on_buttonBox_accepted()
{
    if ((items.size()) < 1){
        QMessageBox::warning(this, "Error", "No Day Selected");
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

