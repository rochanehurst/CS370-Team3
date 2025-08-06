#include <QtTest/QtTest>
#include <QVector>
#include "../createclass.h"

class TC02_AddClassToSchedule : public QObject {
    Q_OBJECT

private slots:
    void test_addClassToVector() {
        // Test adding a ClassInfo to a QVector (unit test for data structure)
        QVector<ClassInfo> schedule;

        ClassInfo testClass;
        testClass.name = "CS111";

        schedule.append(testClass);

        QCOMPARE(schedule.size(), 1);
        QCOMPARE(schedule[0].name, QString("CS111"));
    }
};

QTEST_MAIN(TC02_AddClassToSchedule)
#include "TC02_AddClassToSchedule.moc"
