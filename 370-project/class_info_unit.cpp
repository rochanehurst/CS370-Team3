#include "class_info_unit.h"
#include "ui_class_info_unit.h"
#include "createclass.h"

#include <QMenu>

ClassInfoFrame::ClassInfoFrame(QWidget* parent): QFrame(parent)
{
    ui.setupUi(this);

    // Create tool button options
    QMenu* menu = new QMenu(this);
    menu->addAction("Edit", this, SLOT(editFrameData()));
    menu->addAction("Delete", this, SLOT(deleteFrameData()));
    ui.tool_button->setMenu(menu);
}

void ClassInfoFrame::createFrame(const ClassInfo class_info){
    frame_data = class_info;
    ui.class_name_label->setToolTip(class_info.name);

    setClassName(class_info.name);
    setLocation(class_info.building);
    setTime(class_info.startTime, class_info.endTime);
    setDays(class_info.days);
    setOnline(class_info.online);

    setIcon();
}


void ClassInfoFrame::editFrameData(){
    Dialog editor(getFrameData());
    editor.setModal(true);
    if (editor.exec() == QDialog::Accepted) { createFrame(editor.getData()); }
}


ClassInfo ClassInfoFrame::getFrameData(){
    return frame_data;
}


void ClassInfoFrame::deleteFrameData(){
    this->deleteLater();
}

void ClassInfoFrame::setIcon(){
    QIcon icon(":/icons/icons/Windows_Settings_app_icon.png");
    ui.tool_button->setIcon(icon);
    ui.tool_button->setIconSize(QSize(24,24));
}


void ClassInfoFrame::setClassName(const QString &name){
    int name_length = 24;
    QString name_shortened = name;
    if (name.length() > name_length)
    {
        name_shortened = name_shortened.left(name_length - 3) + "...";
    }
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

