#ifndef SCHEDULELOGIC_H
#define SCHEDULELOGIC_H

#include "class_info_unit.h"
#include <QVector>
#include <QMap>
#include <QString>

class ScheduleLogic {
public:
    ScheduleLogic();

    bool classesConflict(const ClassInfo& a, const ClassInfo& b) const;
    bool scheduleIsValid(const QVector<ClassInfo>& schedule) const;
    void loadBuildingData(const QString& filePath);

    QMap<QChar, QVector<ClassInfo>> groupByDay(const QVector<ClassInfo>& schedule) const;

private:
    bool isValidTimeFormat(const QString& time) const;

    friend class TC08_ScheduleLogicValidation; // friend declaration to allow test access
};



#endif // SCHEDULELOGIC_H
