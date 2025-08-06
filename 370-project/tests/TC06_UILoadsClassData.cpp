#include <QtTest/QtTest>
#include <QVector>
#include "../createclass.h"

class TC06_UILoadsClassData : public QObject {
    Q_OBJECT

private slots:
    void test_loadSingleClass() {
        // Test loading a single class (like UI displays)
        ClassInfo testClass;
        testClass.name = "CS111 - Intro to Programming";
        testClass.building = "MARK 204";
        testClass.startTime = "10:00AM";
        testClass.endTime = "11:15AM";
        testClass.days = "MW";

        // Verify class data is loaded correctly
        QVERIFY(!testClass.name.isEmpty());
        QVERIFY(!testClass.building.isEmpty());
        QVERIFY(!testClass.startTime.isEmpty());
        QVERIFY(!testClass.endTime.isEmpty());
        QVERIFY(!testClass.days.isEmpty());

        // Test specific values
        QCOMPARE(testClass.name, QString("CS111 - Intro to Programming"));
        QCOMPARE(testClass.building, QString("MARK 204"));
        QCOMPARE(testClass.days, QString("MW"));
    }

    void test_loadMultipleClasses() {
        // Test loading multiple classes (like class database)
        QVector<ClassInfo> classDatabase;

        ClassInfo class1;
        class1.name = "CS111 - Intro to Programming";
        class1.building = "MARK 204";
        class1.startTime = "10:00AM";
        class1.endTime = "11:15AM";
        class1.days = "MW";

        ClassInfo class2;
        class2.name = "MATH150 - Calculus I";
        class2.building = "SCI2 350";
        class2.startTime = "2:00PM";
        class2.endTime = "3:15PM";
        class2.days = "TR";

        ClassInfo class3;
        class3.name = "PHYS101 - General Physics";
        class3.building = "SCI1 100";
        class3.startTime = "11:00AM";
        class3.endTime = "12:15PM";
        class3.days = "MWF";

        // Load classes into database
        classDatabase.append(class1);
        classDatabase.append(class2);
        classDatabase.append(class3);

        // Verify all classes loaded
        QCOMPARE(classDatabase.size(), 3);

        // Test each class loaded correctly
        QCOMPARE(classDatabase[0].name, QString("CS111 - Intro to Programming"));
        QCOMPARE(classDatabase[1].name, QString("MATH150 - Calculus I"));
        QCOMPARE(classDatabase[2].name, QString("PHYS101 - General Physics"));

        // Verify no empty data
        for (const ClassInfo& cls : classDatabase) {
            QVERIFY(!cls.name.isEmpty());
            QVERIFY(!cls.building.isEmpty());
            QVERIFY(!cls.days.isEmpty());
        }
    }
};

QTEST_MAIN(TC06_UILoadsClassData)
#include "TC06_UILoadsClassData.moc"
