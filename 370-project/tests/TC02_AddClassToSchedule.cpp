#include <QtTest/QtTest>
#include <QVector>
#include "../createclass.h"

class TC02_AddClassToSchedule : public QObject {
    Q_OBJECT

private slots:
    void test_addOneClass() {
        // Create empty schedule
        QVector<ClassInfo> schedule;

        // Create a simple test class
        ClassInfo testClass;
        testClass.name = "CS111";
        testClass.building = "MARK 204";
        testClass.startTime = "10:00AM";
        testClass.endTime = "11:00AM";
        testClass.days = "MW";

        // Add to schedule
        schedule.append(testClass);

        // Check it was added
        QCOMPARE(schedule.size(), 1);
        QCOMPARE(schedule[0].name, QString("CS111"));
    }

    void test_scheduleStartsEmpty() {
        // New schedule should be empty
        QVector<ClassInfo> schedule;
        QVERIFY(schedule.isEmpty());
    }
};

QTEST_MAIN(TC02_AddClassToSchedule)
#include "TC02_AddClassToSchedule.moc"
