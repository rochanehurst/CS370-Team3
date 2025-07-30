#include "warning.h"
#include "ui_warning.h"

#include <QPropertyAnimation>

using warning_level = MainWindow::WarningLevel;

Warning::Warning(MainWindow::WarningLevel level, QWidget *parent)
    : QFrame(parent)
    , ui_(new Ui::Warning)
{
    ui_->setupUi(this);

    connect(this, &Warning::clicked, this, &Warning::toggleDetails);

    ui_->warning_details->setVisible(false);
    ui_->warning_details->setMaximumHeight(0);

    setWarningLevel(level);

}

Warning::~Warning()
{
    delete ui_;
}



void Warning::setWarningLevel(MainWindow::WarningLevel level) {
    if (level == warning_level::ERROR) {
        QIcon icon(":/icons/icons/warning_level_error.png");
        ui_->warning_icon->setPixmap(icon.pixmap(24,24));
    }
    if (level == warning_level::WARNING) {
        QIcon icon(":/icons/icons/warning_level_warning.png");
        ui_->warning_icon->setPixmap(icon.pixmap(24,24));
    }
    if (level == warning_level::NOTICE) {
        QIcon icon(":/icons/icons/warning_level_notice.png");
        ui_->warning_icon->setPixmap(icon.pixmap(24,24));
    }
}



void Warning::setWarningText(){

}



void Warning::setOffenders(){

}



void Warning::setDescription(){

}



void Warning::toggleDetails() {
    bool expanding = !ui_->warning_details->isVisible();
    ui_->warning_details->setVisible(true); // make it visible if expanding

    int startHeight = expanding ? 0 : ui_->warning_details->sizeHint().height();
    int endHeight   = expanding ? ui_->warning_details->sizeHint().height() : 0;

    auto *anim = new QPropertyAnimation(ui_->warning_details, "maximumHeight");
    anim->setDuration(300);
    anim->setStartValue(startHeight);
    anim->setEndValue(endHeight);
    anim->setEasingCurve(QEasingCurve::OutCubic);

    connect(anim, &QPropertyAnimation::finished, [=]() {
        if (!expanding)
            ui_->warning_details->setVisible(false);
        anim->deleteLater();
    });

    anim->start();
}


