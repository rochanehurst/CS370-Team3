#include "schedulelogic.h"
#include <QTime>
#include <QDebug>

ScheduleLogic::ScheduleLogic() {}

bool ScheduleLogic::classesConflict(const ClassInfo& a, const ClassInfo& b) const {
    for (QChar day : a.days) {
        if (b.days.contains(day)) {
            QTime startA = QTime::fromString(a.startTime.trimmed(), "h:mma");
            QTime endA   = QTime::fromString(a.endTime.trimmed(), "h:mma");

            QTime startB = QTime::fromString(b.startTime.trimmed(), "h:mma");
            QTime endB   = QTime::fromString(b.endTime.trimmed(), "h:mma");

            if (!startA.isValid() || !endA.isValid() || !startB.isValid() || !endB.isValid()) {
                qDebug() << "Invalid time format in class data:";
                return false;
            }

            if (startA < endB && startB < endA)
                return true;
        }
    }
    return false;
}

bool ScheduleLogic::scheduleIsValid(const QVector<ClassInfo>& schedule) const {
    for (int i = 0; i < schedule.size(); ++i) {
        for (int j = i + 1; j < schedule.size(); ++j) {
            if (classesConflict(schedule[i], schedule[j]))
                return false;
        }
    }
    return true;
}

void ScheduleLogic::loadBuildingData(const QString& filePath) {
    qDebug() << "Pretending to load building data from:" << filePath;
}

QMap<QChar, QVector<ClassInfo>> ScheduleLogic::groupByDay(const QVector<ClassInfo>& schedule) const {
    QMap<QChar, QVector<ClassInfo>> grouped;

    for (const ClassInfo& c : schedule) {
        if (c.building.trimmed().toUpper() == "ONLINE") continue;
        for (QChar day : c.days) {
            grouped[day].append(c);
        }
    }

    return grouped;
}

bool ScheduleLogic::isValidTimeFormat(const QString& time) const {
    return QTime::fromString(time.trimmed(), "h:mma").isValid();
}
