#include <QtTest/QtTest>
#include "../savestuff.h"
#include "../createclass.h"

class TC10_SaveFeatureFileCheck : public QObject {
    Q_OBJECT

private slots:
    void test_checkIfOpen() {
        // Test the checkIfOpen function
        std::string testFile = "test_file_check.txt";
        SaveFeature saver(testFile);

        // File should be open after construction
        QVERIFY(saver.checkIfOpen(testFile));

        // Close file
        saver.closeFile();

        // File should be closed
        QVERIFY(!saver.checkIfOpen(testFile));
    }

    void test_clearAllFunction() {
        // Test that clearAll function executes
        std::string testFile = "test_clear_function.txt";
        SaveFeature saver(testFile);

        // Function should execute without error
        saver.clearAll(testFile);
        QVERIFY(true);
    }
};

QTEST_MAIN(TC10_SaveFeatureFileCheck)
#include "TC10_SaveFeatureFileCheck.moc"
