#include "class_result.h"
#include "ui_class_result.h"

class_result::class_result(const ClassData& data, QWidget *parent)
    : QFrame(parent)
    , ui_(new Ui::class_result)
{
    ui_->setupUi(this);
    setupData(data);
    own_data = data;
}

ClassData class_result::getData(){
    return own_data;
}

void class_result::setupData(const ClassData &data) {
    ui_->subject->setText(data.subject);
    ui_->class_code->setText(data.class_code);
    ui_->class_name->setText(data.name);

    // Instructors and days (joined with commas)
    QString instructor_list = data.instructors.join(", ");
    QString day_list = data.days.join(", ");
    QString building_list = data.buildings.join(", ");

    // Build time ranges safely (if data.start and data.end are aligned)
    QStringList times;
    int count = qMin(data.start.size(), data.end.size());
    for (int i = 0; i < count; i++) {
        times.append(data.start[i] + " - " + data.end[i]);
    }
    QString time_list = times.join(", ");

    // Set UI fields
    ui_->instructors->setText(instructor_list);
    ui_->buildings->setText(building_list);
    ui_->days->setText(day_list);
    ui_->time->setText(time_list);
}



class_result::~class_result()
{
    delete ui_;
}



void class_result::on_add_button_clicked()
{
    QWidget *p = this->parentWidget();
    while (p) {
        MainWindow *mainWindow = qobject_cast<MainWindow*>(p);
        if (mainWindow) {

            QVector<QStringList> lists = {own_data.buildings, own_data.days, own_data.start};
            int maxSize = 0;
            for (const auto &list : lists) {
                maxSize = qMax(maxSize, list.size());
            }

            QString last_building = own_data.buildings.isEmpty() ? "" : own_data.buildings.last();
            QString last_day      = own_data.days.isEmpty() ? "" : own_data.days.last();
            QString last_start    = own_data.start.isEmpty() ? "" : own_data.start.last();
            QString last_end      = own_data.end.isEmpty() ? "" : own_data.end.last();

            for (int i = 0; i < maxSize; i++) {
                ClassInfo new_info = own_data.data;

                new_info.building  = (i < own_data.buildings.size()) ? own_data.buildings[i] : last_building;
                new_info.days      = (i < own_data.days.size())      ? own_data.days[i]      : last_day;
                new_info.startTime = (i < own_data.start.size())     ? own_data.start[i]     : last_start;
                new_info.endTime   = (i < own_data.end.size())       ? own_data.end[i]       : last_end;

                mainWindow->createClassFrame(new_info);
            }
            return;
        }
        p = p->parentWidget();
    }
    qDebug() << "MainWindow not found in parent chain!";
}



