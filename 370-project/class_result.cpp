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
            // Convert ClassData -> ClassInfo if needed
            mainWindow->createClassFrame(own_data.data);
            return;
        }
        p = p->parentWidget(); // keep climbing
    }

    qDebug() << "MainWindow not found in parent chain!";
}


