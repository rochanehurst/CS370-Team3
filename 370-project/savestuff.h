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

#include "createclass.h"
//#include "class_info_unit.h"

class SaveFeature {
private:
    fstream SaveFile;

public:
    SaveFeature(const string& filename);

    void backupOpen(const string& filename);
    bool checkIfOpen(const string& filename);

    void closeFile();

    //void addToSave(const ClassInfo& data, const string& filename);
    void addToSave(const ClassInfo& data, const string& filename);

    void deleteFromSave();

    void loadSaveData(const string& filename, QStringList& unparsed);

    //void parseSavaData(const string& filename, QString line, QStringList& parsed, int size);
    void parseSavaData(const string& filename, QString line, ClassInfo& data);

    void clearAll(const string& filename);

    void editSave(const string& filename);

    // 3rd void for convert to widget

    // file -> qstringlist of the unparsed lines -> feed one line into parse funct -> return as
    // second qstringlist -> feed into data card
};


#endif // SAVESTUFF_H
