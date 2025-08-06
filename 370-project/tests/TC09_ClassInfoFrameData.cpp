#include <QtTest/QtTest>
#include "../class_info_unit.h"
#include "../createclass.h"

class TC09_ClassInfoFrameData : public QObject {
    Q_OBJECT

private slots:
    void test_getFrameData() {
        // Test that ClassInfoFrame stores and returns data correctly
        ClassInfoFrame frame;

        ClassInfo testData;
        testData.name = "Physics 101";
        testData.building = "SCI1 200";
        testData.startTime = "1:00PM";
        testData.endTime = "2:15PM";
        testData.days = "TR";

        // Create frame with test data
        frame.createFrame(testData);

        // Get data back and verify
        ClassInfo retrievedData = frame.getFrameData();
        QCOMPARE(retrievedData.name, QString("Physics 101"));
        QCOMPARE(retrievedData.building, QString("SCI1 200"));
        QCOMPARE(retrievedData.days, QString("TR"));
    }
};

QTEST_MAIN(TC09_ClassInfoFrameData)
#include "TC09_ClassInfoFrameData.moc"
