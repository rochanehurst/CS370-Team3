#include "data.h"
#include <fstream>
#include <sstream>
#include <iostream>


static inline std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

// Load classes CSV into a map keyed by class code/id
std::map<std::string, ClassInfo> loadClassesFromCSV(const std::string& filename) {
    std::map<std::string, ClassInfo> classes;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open class CSV file: " << filename << std::endl;
        return {};
    }

    std::string line;

    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string major, className, classCode, instructor, location, daysTimes;

        // Assuming CSV columns: Major,Class Name,Class Code,Instructor,Location,Days & Times
        std::getline(ss, major, ',');
        std::getline(ss, className, ',');
        std::getline(ss, classCode, ',');
        std::getline(ss, instructor, ',');
        std::getline(ss, location, ',');
        std::getline(ss, daysTimes, ',');

        // Trim fields
        major = trim(major);
        className = trim(className);
        classCode = trim(classCode);
        location = trim(location);
        daysTimes = trim(daysTimes);

        // Parse days and times from 'daysTimes'
        size_t spacePos = daysTimes.find(' ');
        if (spacePos == std::string::npos) {
            std::cerr << "Invalid days & times format: " << daysTimes << std::endl;
            continue;
        }

        std::string daysStr = daysTimes.substr(0, spacePos);
        std::string timeStr = daysTimes.substr(spacePos + 1);

        // Parse days string into vector of day abbreviations
        std::vector<std::string> days;
        for (size_t i = 0; i < daysStr.size();) {
            if (daysStr[i] == 'M' || daysStr[i] == 'W' || daysStr[i] == 'F' || daysStr[i] == 'S') {
                days.push_back(std::string(1, daysStr[i]));
                i++;
            } else if (i + 1 < daysStr.size()) {
                std::string twoChars = daysStr.substr(i, 2);
                if (twoChars == "Tu" || twoChars == "Th" || twoChars == "Sa" || twoChars == "Su") {
                    days.push_back(twoChars);
                    i += 2;
                } else {
                    days.push_back(std::string(1, daysStr[i]));
                    i++;
                }
            } else {
                days.push_back(std::string(1, daysStr[i]));
                i++;
            }
        }

        ClassInfo cls;
        cls.id = classCode;
        cls.name = className;
        cls.building = location;
        cls.time = timeStr;
        cls.days = days;


        classes[classCode] = cls;
    }
    return classes;
}

// Load the building distance matrix CSV into nested maps
std::map<std::string, std::map<std::string, int>> loadDistanceMatrixFromCSV(const std::string& filename) {
    std::map<std::string, std::map<std::string, int>> distanceMatrix;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open building CSV file: " << filename << std::endl;
        return {};
    }

    std::string line;
    // Read header line with building names
    std::getline(file, line);
    std::stringstream headerStream(line);
    std::vector<std::string> buildings;
    std::string cell;

    // Skip first empty cell
    std::getline(headerStream, cell, ',');

    while (std::getline(headerStream, cell, ',')) {
        buildings.push_back(trim(cell));
    }

    // Read rows with building name and distances
    while (std::getline(file, line)) {
        std::stringstream lineStream(line);
        std::string rowBuilding;
        std::getline(lineStream, rowBuilding, ',');
        rowBuilding = trim(rowBuilding);

        for (size_t i = 0; i < buildings.size(); ++i) {
            std::string distStr;
            if (!std::getline(lineStream, distStr, ',')) break;
            distStr = trim(distStr);

            int dist = 0;
            try {
                dist = std::stoi(distStr);
            } catch (...) {
                dist = 0;
            }

            distanceMatrix[rowBuilding][buildings[i]] = dist;
        }
    }

    return distanceMatrix;
}
