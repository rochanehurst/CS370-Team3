//class_info_unit.cpp
// #include "savestuff.h"
#include "class_info_unit.h"

#include <QMenu>

//string filename = "cluster_savedata.txt";
// SaveFeature sv("cluster_savedata.txt");

ClassInfoFrame::ClassInfoFrame(ApiMap* apiMap, QWidget* parent)
    : QFrame(parent), apiMap_(apiMap)
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

     //adding to map when adding a class
    if(class_info.building != "ONLINE CLASS"){
        QGeoCoordinate buildingCoord = apiMap_->coordinateForBuilding(class_info.building);
        apiMap_->showMarkerAt(buildingCoord);
        apiMap_->addInfoToMap(buildingCoord, frame_data_.name, frame_data_.building);
    }
}

void ClassInfoFrame::editFrameData() {
    // string olddata = sv.makeString(this->getFrameData());
    Dialog editor(getFrameData());
    editor.setModal(true);
    if (editor.exec() == QDialog::Accepted) {
        if(frame_data_.building != "ONLINE CLASS"){
            QGeoCoordinate buildCoord = apiMap_->coordinateForBuilding(frame_data_.building);
            apiMap_->removeMarkerAt(buildCoord);
            apiMap_->removeInfoMap(buildCoord, frame_data_.name, frame_data_.building);
        }
        createFrame(editor.getData());
        /*
        string newdata = sv.makeString(editor.getData());
        sv.editSave("cluster_savedata.txt", olddata, newdata);
        */
    }
}

void ClassInfoFrame::deleteFrameData() {
    //deleting from map when removing a class
    if(frame_data_.building != "ONLINE CLASS"){
        QGeoCoordinate buildCoord = apiMap_->coordinateForBuilding(frame_data_.building);
        apiMap_->removeMarkerAt(buildCoord);
        apiMap_->removeInfoMap(buildCoord, frame_data_.name, frame_data_.building);
    }
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



