#include "class_info_unit.h"
#include "ui_class_info_unit.h"

#include <QMenu>

// There is a quirk with the tool button
// When you click it, the text inside the button disappears
// It functions normally, you can select and read the menus just fine
// The "..." disappears

ClassInfoFrame::ClassInfoFrame(QWidget* parent): QFrame(parent)
{
    ui.setupUi(this);

    // Create tool button options
    QMenu* menu = new QMenu(this);
    menu->addAction("Edit", this, SLOT(option1()));
    menu->addAction("Delete", this, SLOT(option2()));

    ui.toolButton->setMenu(menu);
}

ClassInfo ClassInfoFrame::getData(){
    return data;
}


void ClassInfoFrame::createFrame(const ClassInfo info){
    data = info;
    setClassName(info.name);
    setLocation(info.building);
    setTime(info.startTime, info.endTime);
    setDays(info.days);
    setOnline(info.online);
}

void ClassInfoFrame::setClassName(const QString &name){
    ui.class_name_label->setText(name);
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

