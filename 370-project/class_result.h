#ifndef CLASS_RESULT_H
#define CLASS_RESULT_H

#include <QFrame>
#include "mainwindow.h"

namespace Ui {
class class_result;
}

class class_result : public QFrame
{
    Q_OBJECT

public:
    explicit class_result(const ClassData& data, QWidget *parent = nullptr);
    ~class_result();

private:
    void setupData(const ClassData& data);

private:
    Ui::class_result *ui_;
};

#endif // CLASS_RESULT_H
