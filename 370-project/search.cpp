#include "search.h"
#include "ui_search.h"

#include <QFile>
#include <QLineEdit>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

search::search(QWidget *parent)
    : QDialog(parent)
    , ui_(new Ui::search)
{
    ui_->setupUi(this);

    QString path = QCoreApplication::applicationDirPath() + "/../../../data/subjects.csv";
    loadCSV(path);
    declareCheckboxes();
    setupConnections();
}

search::~search()
{
    delete ui_;
}

void search::declareCheckboxes(){
    dayHandlers = {
        { ui_->monday,    [this](int state){ updateSearch(); } },
        { ui_->wednesday, [this](int state){ updateSearch(); } },
        { ui_->friday,    [this](int state){ updateSearch(); } },
        { ui_->tuesday,   [this](int state){ updateSearch(); } },
        { ui_->thursday,  [this](int state){ updateSearch(); } },
        { ui_->saturday,  [this](int state){ updateSearch(); } }
    };
}

void search::setupConnections(){
    connect(ui_->subject_combo_box, &QComboBox::currentIndexChanged, this, &search::updateSearch);
    connect(ui_->building_combo_box, &QComboBox::currentIndexChanged, this, &search::updateSearch);
    connect(ui_->input_class, &QLineEdit::textEdited, this, &search::updateSearch);
    connect(ui_->time_startafter, &QTimeEdit::timeChanged, this, &search::updateSearch);
    connect(ui_->time_endbefore, &QTimeEdit::timeChanged, this, &search::updateSearch);

    for (auto box = dayHandlers.begin(); box != dayHandlers.end(); ++box){
        connect(box.key(), &QCheckBox::checkStateChanged, this, box.value());
    }
}

void search::loadCSV(const QString& filename){
    ui_->subject_combo_box->addItem("NO SUBJECT SPECIFIED");
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Failed to open file: " << filename;
        return;
    }

    QTextStream in(&file);
    while(!in.atEnd()){
        QString subject = in.readLine();
        ui_->subject_combo_box->addItem(subject);
    }

    file.close();
}

void search::updateSearch(){
    qDebug() << sender() << " Updating Search Results";
}


