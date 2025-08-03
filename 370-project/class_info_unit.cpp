//class_info_unit.cpp
#include "class_info_unit.h"
#include "savestuff.h"

#include <QMenu>

//string filename = "cluster_savedata.txt";
SaveFeature sv("cluster_savedata.txt");

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
    string olddata = sv.makeString(this->getFrameData());
    Dialog editor(getFrameData());
    //string olddata = sv.makeString(editor.getData());
    // create function to take from classdata and put into qstringlist
    // access editor.class_info_form_ i think is what its called?
    editor.setModal(true);
    //string olddata = sv.makeString(editor.getData());
    if (editor.exec() == QDialog::Accepted) {
        createFrame(editor.getData());
        string newdata = sv.makeString(editor.getData());
        sv.editSave("cluster_savedata.txt", olddata, newdata);
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
    ui_.class_name_label->setText(name);
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

void ClassInfoFrame::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);

    // Calculate dynamic margin/spacing
    int margin = layout()->contentsMargins().left() + layout()->contentsMargins().right();
    int spacing = layout()->spacing();

    // Calculate available width for class name label
    int availableWidth = width() - ui_.tool_button->width() - margin - spacing - 5; // buffer of 5

    // Elide text so it fits
    QFontMetrics fm(ui_.class_name_label->font());
    QString fullText = frame_data_.name;
    QString elidedText = fm.elidedText(fullText, Qt::ElideRight, availableWidth);

    // Update label text
    ui_.class_name_label->setText(elidedText);
}



