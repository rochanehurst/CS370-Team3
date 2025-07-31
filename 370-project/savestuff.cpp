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
#include <QStringList>
using namespace std;

SaveFeature::SaveFeature(const string& filename) {
    SaveFile.open(filename);

    // if (!SaveFile.is_open()) {
    //     //QMessageBox::critical(this, "Error", "Save file has failed to open.");
    //     return;
    // } else {
    //     //QMessageBox::critical(this, "Non-error", "File has opened successfully.");
    //     return;
    // }
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
    if (!SaveFile.is_open()) {
        return;
    }

    SaveFile.close();
    SaveFile.open(filename, ios::app);

    /* if (filesystem::is_empty(filename)) { //if file is not empty, add new line before data
        SaveFile << endl;
    } */

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
        string hold;
        //size = 0;
        //string unparsed[15];
        SaveFile.ignore();
        while (!SaveFile.eof()) {
            getline(SaveFile, hold, '\n'); // takes entire line until newline char
            //SaveFile.ignore(); // ignore \n
            unparsed.append(QString::fromStdString(hold)); // adds hold to unparsed stringlist as a qstring
                //unparsed[size] = hold; // place line into array
            //size++; // increase size
        }
        //SaveFile.ignore(); //should ignore last newline char
        //SaveFile.ignore();
        // void collect_all(istream& input, College c[], int& size) {
        //     while (!input.eof()) {
        //         collect(input, c[size]);
        //         size++;
        //     }
        // }

        // getline the entire string until '\n'
        // then run thru a loop to add it to an array of strings (or maybe in the format of qt's strings)
        // use string length to get how long string should go on for? as var for loop

        /* void collect(istream& input, College& c) {
            getline(input, c.name, ',');
            input >> c.cost;
            input.ignore();
            input >> c.acceptance;
            input.ignore();
        } */
    }
    // take all data and put it into a string
    // split everything up, put into an array?
        // alpha to bool for online
    // feed it into variables

    // issues:
        // what to do about multiple lines?
            // array of strings, and each one gets parsed?
            // and then the data gets put into an array of classdata

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
    SaveFile.open(filename, ios::out | ios::trunc);
}
