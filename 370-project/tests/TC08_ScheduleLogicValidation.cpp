#include <QtTest/QtTest>
#include "../schedulelogic.h"
#include "../createclass.h"

class TC08_ScheduleLogicValidation : public QObject {
    Q_OBJECT

private slots:
    void test_isValidTimeFormat() {
        // Test the time format validation function
        ScheduleLogic logic;

        // Valid time formats
        QVERIFY(logic.isValidTimeFormat("10:00AM"));
        QVERIFY(logic.isValidTimeFormat("2:30PM"));
        QVERIFY(logic.isValidTimeFormat("12:00PM"));

        // Invalid formats
        QVERIFY(!logic.isValidTimeFormat("25:00AM"));  // Invalid hour
        QVERIFY(!logic.isValidTimeFormat("10AM"));     // Missing minutes
        QVERIFY(!logic.isValidTimeFormat(""));         // Empty string
    }
};

QTEST_MAIN(TC08_ScheduleLogicValidation)
#include "TC08_ScheduleLogicValidation.moc"
