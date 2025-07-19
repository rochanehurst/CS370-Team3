#include "class_info_unit.h"
#include "ui_class_info_unit.h"
#include "createclass.h"

#include <QMenu>

ClassInfoFrame::ClassInfoFrame(QWidget* parent): QFrame(parent)
{
    ui.setupUi(this);

    // Create tool button options
    QMenu* menu = new QMenu(this);
    menu->addAction("Edit", this, SLOT(editFrame()));
    menu->addAction("Delete", this, SLOT(deleteFrame()));
    ui.toolButton->setMenu(menu);
}

void ClassInfoFrame::createFrame(const ClassInfo info){
    data = info;
    setClassName(info.name);
    setLocation(info.building);
    setTime(info.startTime, info.endTime);
    setDays(info.days);
    setOnline(info.online);

    setIcon();
}

ClassInfo ClassInfoFrame::getData(){
    return data;
}

void ClassInfoFrame::editFrame(){
    Dialog editor(getData());
    editor.setModal(true);
    if (editor.exec() == QDialog::Accepted) { createFrame(editor.getData()); }
}

void ClassInfoFrame::deleteFrame(){
    this->deleteLater();
}

void ClassInfoFrame::setIcon(){
    QIcon icon(":/icons/icons/Windows_Settings_app_icon.png");
    ui.toolButton->setIcon(icon);
    ui.toolButton->setIconSize(QSize(24,24));
}


void ClassInfoFrame::setClassName(const QString &name){
    QString name_shortened = name;
    if (name.length() > 16){ name_shortened = name_shortened.left(13) + "..."; }
    ui.class_name_label->setText(name_shortened);
}

void ClassInfoFrame::setLocation(const QString &location){
    ui.location_label->setText(location);
}

void ClassInfoFrame::setTime(const QString &start, const QString &stop){
    QString time_range = start + " - " + stop;
    ui.time_label->setText(time_range);
}

void ClassInfoFrame::setDays(const QString &days){
    ui.days_label->setText(days);
}

void ClassInfoFrame::setOnline(const bool &online){
    if (online){ ui.online_label->setText("Online"); }
    else{ ui.online_label->setText("In Person"); }
}

