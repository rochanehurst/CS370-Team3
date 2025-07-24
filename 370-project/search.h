#ifndef SEARCH_H
#define SEARCH_H

#include <QDialog>
#include <QCheckBox>

namespace Ui {
class search;
}

class search : public QDialog
{
    Q_OBJECT

public:
    explicit search(QWidget *parent = nullptr);
    ~search();

private slots:
    void updateSearch();

private:
    void declareCheckboxes();
    void setupConnections();
    void loadCSV(const QString& filePath);

private:
    Ui::search *ui_;
    QMap<QCheckBox*, std::function<void(int)>> dayHandlers;
};

#endif // SEARCH_H
