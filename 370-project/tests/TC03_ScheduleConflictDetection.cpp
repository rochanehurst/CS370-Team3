#include <QtTest/QtTest>
#include "../createclass.h"
#include "../schedulelogic.h"

class TC03_ScheduleConflictDetection : public QObject {
    Q_OBJECT

private slots:
    void test_noConflictDifferentDays() {
        ScheduleLogic logic;

        // Monday class
        ClassInfo mondayClass;
        mondayClass.name = "Math";
        mondayClass.days = "M";
        mondayClass.startTime = "10:00AM";
        mondayClass.endTime = "11:00AM";

        // Tuesday class (different day)
        ClassInfo tuesdayClass;
        tuesdayClass.name = "English";
        tuesdayClass.days = "T";
        tuesdayClass.startTime = "10:00AM";  // Same time is OK
        tuesdayClass.endTime = "11:00AM";

        // Should NOT conflict (different days)
        QVERIFY(!logic.classesConflict(mondayClass, tuesdayClass));
    }

    void test_conflictSameTime() {
        ScheduleLogic logic;

        // Monday 10am class
        ClassInfo class1;
        class1.name = "Math";
        class1.days = "M";
        class1.startTime = "10:00AM";
        class1.endTime = "11:00AM";

        // Another Monday 10am class
        ClassInfo class2;
        class2.name = "Science";
        class2.days = "M";  // Same day!
        class2.startTime = "10:00AM";  // Same time!
        class2.endTime = "11:00AM";

        // Should conflict
        QVERIFY(logic.classesConflict(class1, class2));
    }
};

QTEST_MAIN(TC03_ScheduleConflictDetection)
#include "TC03_ScheduleConflictDetection.moc"
