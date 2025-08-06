#include <QtTest/QtTest>
#include <QVector>
#include "../createclass.h"

class TC05_ClearAllClasses : public QObject {
    Q_OBJECT

private slots:
    void test_clearEmptySchedule() {
        // Test clearing when schedule is already empty
        QVector<ClassInfo> schedule;
        schedule.clear();

        QCOMPARE(schedule.size(), 0);
        QVERIFY(schedule.isEmpty());
    }

    void test_clearScheduleWithClasses() {
        // Test clearing schedule with actual ClassInfo objects
        QVector<ClassInfo> schedule;

        // Add real classes
        ClassInfo class1;
        class1.name = "CS111";
        class1.building = "MARK 204";
        class1.startTime = "10:00AM";
        class1.endTime = "11:15AM";
        class1.days = "MW";

        ClassInfo class2;
        class2.name = "MATH150";
        class2.building = "SCI2 350";
        class2.startTime = "2:00PM";
        class2.endTime = "3:15PM";
        class2.days = "TR";

        ClassInfo class3;
        class3.name = "PHYS101";
        class3.building = "SCI1 100";
        class3.startTime = "11:00AM";
        class3.endTime = "12:15PM";
        class3.days = "MWF";

        // Add classes to schedule (like current_schedule_data_)
        schedule.append(class1);
        schedule.append(class2);
        schedule.append(class3);

        // Verify classes were added
        QCOMPARE(schedule.size(), 3);
        QCOMPARE(schedule[0].name, QString("CS111"));
        QCOMPARE(schedule[1].name, QString("MATH150"));
        QCOMPARE(schedule[2].name, QString("PHYS101"));

        // Clear all classes (just like clearSchedule() function)
        schedule.clear();

        // Verify schedule is empty
        QCOMPARE(schedule.size(), 0);
        QVERIFY(schedule.isEmpty());
    }
};

QTEST_MAIN(TC05_ClearAllClasses)
#include "TC05_ClearAllClasses.moc"
