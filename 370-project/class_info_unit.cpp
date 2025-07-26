//class_info_unit.cpp
#include "class_info_unit.h"

#include <QMenu>

ClassInfoFrame::ClassInfoFrame(QWidget* parent)
    : QFrame(parent)
{
    ui_.setupUi(this);

    // Create tool button options
    QMenu* menu = new QMenu(this);
    menu->addAction("Edit", this, &ClassInfoFrame::editFrameData);
    menu->addAction("Delete", this, &ClassInfoFrame::deleteFrameData);
    ui_.tool_button->setMenu(menu);
}

void ClassInfoFrame::createFrame(const ClassInfo& class_info) {
    frame_data_ = class_info;
    ui_.class_name_label->setToolTip(class_info.name);

    setIcon();
    setClassName(class_info.name);
    setLocation(class_info.building);
    setTime(class_info.startTime, class_info.endTime);
    setDays(class_info.days);
}

void ClassInfoFrame::editFrameData() {
    Dialog editor(getFrameData());
    editor.setModal(true);
    if (editor.exec() == QDialog::Accepted) {
        createFrame(editor.getData());
    }
}

void ClassInfoFrame::deleteFrameData() {
    deleteLater();
}

ClassInfo ClassInfoFrame::getFrameData() const {
    return frame_data_;
}

void ClassInfoFrame::setIcon() {
    QIcon icon(":/icons/icons/Windows_Settings_app_icon.png");
    ui_.tool_button->setIcon(icon);
    ui_.tool_button->setIconSize(QSize(24,24));
}

void ClassInfoFrame::setClassName(const QString &name) {
    constexpr int name_length = 24;

    QString name_shortened = name;
    if (name.length() > name_length) {
        name_shortened = name_shortened.left(name_length - 3) + "...";
    }
    ui_.class_name_label->setText(name_shortened);
}

void ClassInfoFrame::setLocation(const QString& location) {
    ui_.location_label->setText(location);
}

void ClassInfoFrame::setTime(const QString& start, const QString& stop) {
    QString time_range = start + " - " + stop;
    ui_.time_label->setText(time_range);
}

void ClassInfoFrame::setDays(const QString& days) {
    ui_.days_label->setText(days);
}
<<<<<<< HEAD
=======

>>>>>>> 7dabfc36730817542fab985d1e88bd8c3b446888
