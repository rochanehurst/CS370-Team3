#include "savestuff.h"
#include "createclass.h"
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
using namespace std;

SaveFeature::SaveFeature(const string& filename) {
    SaveFile.open(filename);
    if (!SaveFile.is_open()) {
        //QMessageBox::critical(this, "Error", "Save file has failed to open.");
        return;
    } else {
        //QMessageBox::critical(this, "Non-error", "File has opened successfully.");
        return;
    }
}

void SaveFeature::addToSave(const ClassInfo& data, const string& filename) {
    if (!SaveFile.is_open()) {
        //QMessageBox::critical(this, "Error", "Save file has failed to open.");
        return;
    }
    if (!filesystem::is_empty(filename)) { //if file is not empty, add new line before data
        SaveFile << "/n";
    }
    SaveFile << data.school.toStdString() << ",";
    SaveFile << data.name.toStdString() << ",";
    SaveFile << data.building.toStdString() << ",";
    SaveFile << data.startTime.toStdString() << ",";
    SaveFile << data.endTime.toStdString() << ",";
    SaveFile << data.days.toStdString() << ",";
    SaveFile << boolalpha << data.online;
}

//load function goes here
void SaveFeature::loadSaveData(const string& filename, const ClassInfo& data) {
    // take all data and put it into a string
    // split everything up, put into an array?
        // alpha to bool for online
    // feed it into variables

    // issues:
        // what to do about multiple lines?
            // array of strings, and each one gets parsed?
            // and then the data gets put into an array of classdata
}
