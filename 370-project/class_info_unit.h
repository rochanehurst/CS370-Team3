//class_info_unit.h
#ifndef CLASS_INFO_UNIT_H
#define CLASS_INFO_UNIT_H

#include "ui_class_info_unit.h"
#include "createclass.h"
#include "apimap.h"

class ClassInfoFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ClassInfoFrame(ApiMap* apiMap = nullptr, QWidget* parent = nullptr);
    void createFrame(const ClassInfo& class_info);
    ClassInfo getFrameData() const;

private slots:
    void editFrameData();
    void deleteFrameData();

private:
    void setIcon();
    void setClassName(const QString &name);
    void setLocation(const QString &location);
    void setTime(const QString &start, const QString &stop);
    void setDays(const QString &days);
    void setOnline(bool online);
    void resizeEvent(QResizeEvent *event);

private:
    Ui::class_info_frame ui_;
    ClassInfo frame_data_;
    ApiMap* apiMap_ = nullptr;  // added for apimap
};

#endif // CLASS_INFO_UNIT_H
