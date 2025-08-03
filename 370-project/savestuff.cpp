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

    SaveFile << data.name.toStdString() << "," <<
        data.building.toStdString() << "," <<
        data.startTime.toStdString() << "," <<
        data.endTime.toStdString() << "," <<
        data.days.toStdString() << endl;
}



//load function goes here
void SaveFeature::loadSaveData(const string& filename, QStringList& unparsed) {
    if (filesystem::is_empty(filename)) {
        return;
    } else {
        SaveFile.close();
        SaveFile.open(filename, ios::in);
        string hold;

        while (std::getline(SaveFile, hold)) {
            if (!hold.empty()) {
                unparsed.append(QString::fromStdString(hold));
            }
        }
        SaveFile.close();
    }
}



void SaveFeature::parseSavaData(const string& filename, QString line, ClassInfo& data) {
    QStringList parsed;

    if (std::filesystem::is_empty(filename) || line.trimmed().isEmpty()) {
        return;
    }

    else {
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
