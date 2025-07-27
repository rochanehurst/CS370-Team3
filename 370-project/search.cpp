#include "search.h"
#include "ui_search.h"

#include <QFile>
#include <QLineEdit>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

Search::Search(QWidget *parent)
    : QDialog(parent)
    , ui_(new Ui::Search)
{
    ui_->setupUi(this);

    QString path = QCoreApplication::applicationDirPath() + "/../../../data/subjects.csv";
    loadCSV(path);
    declareCheckboxes();
    setupConnections();
}

Search::~Search()
{
    delete ui_;
}

void Search::declareCheckboxes(){
    dayHandlers = {
        { ui_->monday,    [this](int state){ updateSearch(); } },
        { ui_->wednesday, [this](int state){ updateSearch(); } },
        { ui_->friday,    [this](int state){ updateSearch(); } },
        { ui_->tuesday,   [this](int state){ updateSearch(); } },
        { ui_->thursday,  [this](int state){ updateSearch(); } },
        { ui_->saturday,  [this](int state){ updateSearch(); } }
    };
}

void Search::setupConnections(){
    connect(ui_->subject_combo_box, &QComboBox::currentIndexChanged, this, &Search::updateSearch);
    connect(ui_->building_combo_box, &QComboBox::currentIndexChanged, this, &Search::updateSearch);
    connect(ui_->input_class, &QLineEdit::textEdited, this, &Search::updateSearch);
    connect(ui_->time_startafter, &QTimeEdit::timeChanged, this, &Search::updateSearch);
    connect(ui_->time_endbefore, &QTimeEdit::timeChanged, this, &Search::updateSearch);

    for (auto box = dayHandlers.begin(); box != dayHandlers.end(); ++box){
        connect(box.key(), &QCheckBox::checkStateChanged, this, box.value());
    }
}

void Search::loadCSV(const QString& filename){
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

void Search::updateSearch(){
    qDebug() << sender() << " Updating Search Results";
}


