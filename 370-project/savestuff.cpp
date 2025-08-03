#include "savestuff.h"
#include "createclass.h"
#include <stdio.h>
#include <cstdio>
#include <vector>
//#include "ui_createclass.h"
//#include "class_info_unit.h"

#include <QMessageBox>
#include <fstream>
#include <iostream>
#include <string>
#include <QFrame>
#include <QString>
#include <QApplication>
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

void SaveFeature::backupOpen(const string& filename) {
    SaveFile.open(filename);
}

bool SaveFeature::checkIfOpen(const string& filename) {
    bool open = SaveFile.is_open();
    return open;
}

void SaveFeature::closeFile() {
    SaveFile.close();
}

void SaveFeature::addToSave(const ClassInfo& data, const string& filename) {
    /* if (!SaveFile.is_open()) {
        return;
    } */

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

void SaveFeature::editSave(const string& filename, string olddata, string newdata) {
    vector<string> file_lines;
    string line;
    cout << olddata << endl << newdata << endl;

    if (SaveFile.is_open()) {
        while(getline(SaveFile, line)){
            if (line == olddata) {
                line = newdata;
                cout << "equals" << endl;
            }
            file_lines.push_back(line);
            cout << "pushback" << endl;
        }
    } else {
        perror("File unopened");
    }

    this->clearAll(filename);
    SaveFile.open(filename, ios::out|ios::app);
    int count = file_lines.size();
    cout << count << endl << file_lines.at(0) << endl << file_lines.at(1);

    if(!SaveFile.is_open()) {
        perror("file unopened");
    }
    for (int i = 0; i < count; i++) {
        SaveFile << endl << file_lines.at(i);
        cout << "dat: " << file_lines.at(i) << endl;
    }
    /* string temp_name = "tempfile.txt";
    ofstream tempfile;
    tempfile.open(temp_name);
    if (tempfile.is_open()) {
        perror("file not opened");
    }
    string line;
    const char* og_filename = filename.c_str();
    const char* new_filename = temp_name.c_str();

    while (getline(SaveFile, line)) {
        if (line == olddata) {
            line = newdata;
        }
        tempfile << line << endl;
    }

    SaveFile.close();
    tempfile.close();

    int del_status = remove(filename.c_str());
    int rename_status = rename(og_filename, new_filename);
    if (del_status != 0) {
        perror("File not deleted.");
    }
    if (rename_status != 0) {
        //printf(og_filename, new_filename);
        perror(new_filename);
        perror("File rename failed.");
    } */
}

string SaveFeature::makeString(ClassInfo data) {
    string result;
    //cout << data.name.toStdString();
    result = data.name.toStdString() + ',' + data.building.toStdString() + ',' + data.startTime.toStdString() + ',' + data.endTime.toStdString() + ',' + data.days.toStdString() + ',';
    return result;
}

void SaveFeature::clearAll(const string& filename) {
    ofstream(filename, ios::trunc).close();
}
