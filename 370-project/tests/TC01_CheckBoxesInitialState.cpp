// tests/TC01_CheckBoxesInitialState.cpp
#include <QtTest/QtTest>
#include <QCheckBox>
#include "../createclass.h"

class TC01_CheckBoxesInitialState : public QObject {
    Q_OBJECT

private slots:
    void test_checkboxesStartUnchecked();
};

void TC01_CheckBoxesInitialState::test_checkboxesStartUnchecked() {
    Dialog dialog;

    QCheckBox* monday = dialog.findChild<QCheckBox*>("monday");
    QCheckBox* wednesday = dialog.findChild<QCheckBox*>("wednesday");
    QCheckBox* friday = dialog.findChild<QCheckBox*>("friday");

    QVERIFY(monday);
    QVERIFY(wednesday);
    QVERIFY(friday);

    QVERIFY(!monday->isChecked());
    QVERIFY(!wednesday->isChecked());
    QVERIFY(!friday->isChecked());
}

QTEST_MAIN(TC01_CheckBoxesInitialState)
#include "TC01_CheckBoxesInitialState.moc"


