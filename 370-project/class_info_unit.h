#ifndef CLASS_INFO_UNIT_H
#define CLASS_INFO_UNIT_H

#include "ui_class_info_unit.h"
#include "createclass.h"
#include <QFrame>

class ClassInfoFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ClassInfoFrame(QWidget* parent = nullptr);

    void createFrame(const ClassInfo class_info);

    ClassInfo getData();

private slots:
    void setClassName(const QString &name);

    void setLocation(const QString &location);

    void setTime(const QString &start, const QString &stop);

    void setDays(const QString &days);

    void setOnline(const bool &online);

    void setIcon();

    void editFrame();

    void deleteFrame();

private:
    Ui::class_info_frame ui;

    ClassInfo data;
};
#endif // CLASS_INFO_UNIT_H
