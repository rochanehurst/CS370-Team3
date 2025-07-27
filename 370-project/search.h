#ifndef SEARCH_H
#define SEARCH_H

#include <QDialog>
#include <QCheckBox>

//QT_BEGIN_NAMESPACE
namespace Ui {
class Search;
}
//QT_END_NAMESPACE

class Search : public QDialog
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);
    //Search(QWidget *parent = nullptr);
    ~Search();

private slots:
    void updateSearch();

private:
    void declareCheckboxes();
    void setupConnections();
    void loadCSV(const QString& filePath);

private:
    Ui::Search *ui_;
    QMap<QCheckBox*, std::function<void(int)>> dayHandlers;
};

#endif // SEARCH_H
