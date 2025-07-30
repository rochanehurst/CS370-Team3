#ifndef WARNING_H
#define WARNING_H

#include <QFrame>
#include <QMouseEvent>
#include <QVBoxLayout>

#include "mainwindow.h"

namespace Ui {
class Warning;
}

class Warning : public QFrame {
    Q_OBJECT
public:
    explicit Warning(MainWindow::WarningLevel level, QWidget *parent = nullptr);
    ~Warning();

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override {
        emit clicked();
        QFrame::mousePressEvent(event);
    }

private slots:
    void toggleDetails();


private:
    void setWarningLevel(MainWindow::WarningLevel level);
    void setWarningText();
    void setOffenders();
    void setDescription();

private:
    Ui::Warning *ui_;
};


#endif // WARNING_H
