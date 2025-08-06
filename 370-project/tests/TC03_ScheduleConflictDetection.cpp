#include <QtTest/QtTest>
#include "../createclass.h"
#include "../schedulelogic.h"

class TC03_ScheduleConflictDetection : public QObject {
    Q_OBJECT

private slots:
    void test_conflictDetection() {
        // Unit test for classesConflict() function
        ScheduleLogic logic;

        ClassInfo class1;
        class1.days = "M";
        class1.startTime = "10:00AM";
        class1.endTime = "11:00AM";

        ClassInfo class2;
        class2.days = "M";
        class2.startTime = "10:30AM";
        class2.endTime = "11:30AM";

        // Test the conflict detection function
        QVERIFY(logic.classesConflict(class1, class2));
    }
};

QTEST_MAIN(TC03_ScheduleConflictDetection)
#include "TC03_ScheduleConflictDetection.moc"
