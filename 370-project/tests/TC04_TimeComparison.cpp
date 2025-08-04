#include <QtTest/QtTest>
#include <QTime>
#include "../createclass.h"

class TC04_TimeComparison : public QObject {
    Q_OBJECT

private slots:
    void test_validClassTimeRange() {
        // Test that class times are in correct order (like  program validates)
        ClassInfo testClass;
        testClass.name = "CS111";
        testClass.building = "MARK 204";
        testClass.startTime = "10:00AM";
        testClass.endTime = "11:15AM";
        testClass.days = "MW";

        // Parse times
        QTime start = QTime::fromString(testClass.startTime, "h:mmAP");
        QTime end = QTime::fromString(testClass.endTime, "h:mmAP");

        // Verify start time is before end time (like time validation)
        QVERIFY(start.isValid());
        QVERIFY(end.isValid());
        QVERIFY(start < end);
    }

    void test_invalidTimeOrder() {
        // Test invalid time range (end before start)
        QString startTime = "11:00AM";
        QString endTime = "10:00AM";  // End before start - invalid!

        QTime start = QTime::fromString(startTime, "h:mmAP");
        QTime end = QTime::fromString(endTime, "h:mmAP");

        // This should be invalid (start after end)
        QVERIFY(start.isValid());
        QVERIFY(end.isValid());
        QVERIFY(start > end);  // Invalid situation your program prevents
    }

    void test_sameStartEndTime() {
        // Test edge case: same start and end time
        QString timeString = "10:00AM";

        QTime start = QTime::fromString(timeString, "h:mmAP");
        QTime end = QTime::fromString(timeString, "h:mmAP");

        QVERIFY(start.isValid());
        QVERIFY(end.isValid());
        QVERIFY(start == end);
    }
};

QTEST_MAIN(TC04_TimeComparison)
#include "TC04_TimeComparison.moc"
