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

    bool checkIfOpen(const string& filename);

    void closeFile();

    void addToSave(const ClassInfo& data, const string& filename);

    void loadSaveData(const string& filename, QStringList& unparsed);

    void parseSavaData(const string& filename, QString line, ClassInfo& data);

    void clearAll(const string& filename);
};

#endif // SAVESTUFF_H
