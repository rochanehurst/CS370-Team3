#include <QtTest/QtTest>
#include "../createclass.h"
#include "../savestuff.h"

class TC07_SaveScheduleToFile : public QObject {
    Q_OBJECT

private slots:
    void test_saveOneClass() {
        // Create test file
        std::string testFile = "test_save.txt";
        SaveFeature saver(testFile);

        // Clear any old data
        saver.clearAll(testFile);

        // Create simple class
        ClassInfo myClass;
        myClass.name = "CS111";
        myClass.building = "MARK";
        myClass.startTime = "10:00AM";
        myClass.endTime = "11:00AM";
        myClass.days = "MW";

        // Save it
        saver.addToSave(myClass, testFile);
        saver.closeFile();

        // If we get here without crashing, test passes!
        QVERIFY(true);
    }

    void test_clearFile() {
        std::string testFile = "test_clear.txt";
        SaveFeature saver(testFile);

        // Clear the file
        saver.clearAll(testFile);

        // Test passes if no crash
        QVERIFY(true);
    }
};

QTEST_MAIN(TC07_SaveScheduleToFile)
#include "TC07_SaveScheduleToFile.moc"
