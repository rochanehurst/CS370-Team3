#include "savestuff.h"

#include <QMessageBox>
#include <fstream>
#include <string>
#include <QString>
#include <filesystem>
#include <QStringList>
using namespace std;

SaveFeature::SaveFeature(const string& filename) {
    SaveFile.open(filename, ios::in | ios::out);

    if (!SaveFile.is_open()) {
        // File didn't exist, so create it first
        SaveFile.clear(); // reset flags
        SaveFile.open(filename, ios::out);
        SaveFile.close();

        // Reopen for read/write
        SaveFile.open(filename, ios::in | ios::out);
    }
}



bool SaveFeature::checkIfOpen(const string& filename) {
    bool open = SaveFile.is_open();
    return open;
}



void SaveFeature::closeFile() {
    SaveFile.close();
}



// Does not currently handle class names with comma's in them
void SaveFeature::addToSave(const ClassInfo& data, const string& filename) {
    SaveFile.close();
    SaveFile.open(filename, ios::app);

    SaveFile << endl << data.name.toStdString() << ",";
    SaveFile << data.building.toStdString() << ",";
    SaveFile << data.startTime.toStdString() << ",";
    SaveFile << data.endTime.toStdString() << ",";
    SaveFile << data.days.toStdString();

}

//load function goes here
void SaveFeature::loadSaveData(const string& filename, QStringList& unparsed) {
    if (filesystem::is_empty(filename)) {
        return;
    } else {
        SaveFile.close();
        SaveFile.open(filename, ios::in);
        string hold;

        SaveFile.ignore(); // ignores the first \n
        while (!SaveFile.eof()) {
            getline(SaveFile, hold, '\n');
            unparsed.append(QString::fromStdString(hold)); // adds hold to unparsed stringlist as a qstring
        }
        SaveFile.close();
    }
}

void SaveFeature::parseSavaData(const string& filename, QString line, ClassInfo& data) {
    QStringList parsed;

    if (filesystem::is_empty(filename) || line == '\n') {
        return;
    } else {
        parsed = line.split(',');
        int parsedsize = parsed.size();
        if (parsedsize != 1) {
            data.name = parsed.at(0);
            data.building = parsed.at(1);
            data.startTime = parsed.at(2);
            data.endTime = parsed.at(3);
            data.days = parsed.at(4);
        }
    }
}



void SaveFeature::clearAll(const string& filename) {
    ofstream(filename, ios::trunc).close();
}
