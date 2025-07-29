#ifndef SEARCH_H
#define SEARCH_H

#include <QDialog>
#include <QCheckBox>
#include <QTimeEdit>
#include "mainwindow.h"

namespace Ui {
class search;
}

class search : public QDialog
{
    Q_OBJECT

public:
    explicit search(QVector<ClassData> classes_, QWidget *parent = nullptr);
    ~search();

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
    Ui::search *ui_;
    QMap<QCheckBox*, std::function<void(int)>> dayHandlers;
    QVBoxLayout* class_list_layout_;
    QVector<ClassData> search_classes_;
    QVector<ClassData> classes_with_subject_;
    QVector<ClassData> classes_with_building_;
    QVector<ClassData> filtered_classes_;
    std::array<std::pair<QCheckBox*, QChar>, 6> day_boxes;
};

#endif // SEARCH_H
