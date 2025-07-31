#ifndef SAVESTUFF_H
#define SAVESTUFF_H

#include <QMainWindow>
#include <QDialog>
#include <QObject>
#include <QVector>
#include <QFrame>
#include <QVBoxLayout>

#include <QMessageBox>
#include <QStringList>
#include <fstream>
#include <string>
using namespace std;

#include "mainwindow.h"
#include "createclass.h"
//#include "class_info_unit.h"

class SaveFeature {
private:
    fstream SaveFile;

public:
    SaveFeature(const string& filename);

    void closeFile();

    //void addToSave(const ClassInfo& data, const string& filename);
    void addToSave(const ClassInfo& data, const string& filename);

    void deleteFromSave();

    void loadSaveData(const string& filename, QStringList& unparsed);

    //void parseSavaData(const string& filename, QString line, QStringList& parsed, int size);
    void parseSavaData(const string& filename, QString line, ClassInfo& data);

    void clearAll(const string& filename);

    // 3rd void for convert to widget

    // file -> qstringlist of the unparsed lines -> feed one line into parse funct -> return as
    // second qstringlist -> feed into data card
};

/* QT_BEGIN_NAMESPACE
namespace Ui {

class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void createClassButtonHandler();

    void createClassFrame(const ClassInfo& class_info);

    void deleteClassFrame(ClassInfoFrame* class_data);

    void editClassFrame(ClassInfoFrame* class_data);

private:
    Ui::MainWindow *ui;

    QVector<ClassInfo> class_info;  // stores the data model

    QVector<ClassInfoFrame*> class_data_containers;  // stores the actual QFrame widgets

    QVBoxLayout* classListLayout;  // layout where the frames get added
}; */

#endif // SAVESTUFF_H
