#ifndef SEARCH_WINDOW_H
#define SEARCH_WINDOW_H

#include "mainwindow.h"
#include <qdatetime.h>

namespace Ui {
class search_window;
}

class search_window : public QDialog
{
    Q_OBJECT

public:
    explicit search_window(QVector<ClassData> classes_, QWidget *parent = nullptr);
    ~search_window();

private slots:
    void updateSearch();

    void on_reset_time_clicked();

private:
    void declareCheckboxes();
    void setupConnections();
    void loadCSV(const QString& filePath);
    void applyFilters();
    void createWidgets(const QVector<ClassData> &data);

private:
    Ui::search_window *ui_;
    QMap<QCheckBox*, std::function<void(int)>> dayHandlers;
    QVector<ClassData> search_classes_;
    QVector<ClassData> classes_with_subject_;
    QVector<ClassData> classes_with_building_;
    QVector<ClassData> filtered_classes_;
    std::array<std::pair<QCheckBox*, QChar>, 6> day_boxes;
    QTime default_start = QTime::fromString("6:00AM", "h:mmA");
    QTime default_end = QTime::fromString("10:00PM", "h:mmA");
};

#endif // SEARCH_WINDOW_H
