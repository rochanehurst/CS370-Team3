#include "data.h"

//CLASS DATABASE
std::map<std::string, ClassInfo> loadClassesFromDB() {
    std::map<std::string, ClassInfo> db;

    db["CS370-01"] = {"CS370-01", "Software Engineering", "MARK", {"M", "W"}, "10:30-11:45"};
    db["BIO210-01"] = {"BIO210-01", "Biology I", "SCI1", {"M", "W"}, "12:00-13:15"};
    db["HIST105-01"] = {"HIST105-01", "World History", "SBSB", {"T", "R"}, "10:30-11:45"};

    return db;
}

//DISTANCE MATRIX
std::map<std::string, std::map<std::string, int>> getDistanceMatrix() {
    return {
        {"MARK", {{"MARK", 0}, {"SCI1", 5}, {"SCI2", 6}, {"SBSB", 7}, {"COM", 4}, {"UNIV", 3}, {"ACD", 6}, {"ARTS", 5}}},
        {"SCI1", {{"MARK", 5}, {"SCI1", 0}, {"SCI2", 2}, {"SBSB", 2}, {"COM", 5}, {"UNIV", 6}, {"ACD", 7}, {"ARTS", 6}}},
        {"SCI2", {{"MARK", 6}, {"SCI1", 2}, {"SCI2", 0}, {"SBSB", 3}, {"COM", 6}, {"UNIV", 7}, {"ACD", 8}, {"ARTS", 7}}},
        {"SBSB", {{"MARK", 7}, {"SCI1", 2}, {"SCI2", 3}, {"SBSB", 0}, {"COM", 3}, {"UNIV", 4}, {"ACD", 6}, {"ARTS", 6}}},
        {"COM",  {{"MARK", 4}, {"SCI1", 5}, {"SCI2", 6}, {"SBSB", 3}, {"COM", 0}, {"UNIV", 2}, {"ACD", 3}, {"ARTS", 2}}},
        {"UNIV", {{"MARK", 3}, {"SCI1", 6}, {"SCI2", 7}, {"SBSB", 4}, {"COM", 2}, {"UNIV", 0}, {"ACD", 2}, {"ARTS", 3}}},
        {"ACD",  {{"MARK", 6}, {"SCI1", 7}, {"SCI2", 8}, {"SBSB", 6}, {"COM", 3}, {"UNIV", 2}, {"ACD", 0}, {"ARTS", 1}}},
        {"ARTS", {{"MARK", 5}, {"SCI1", 6}, {"SCI2", 7}, {"SBSB", 6}, {"COM", 2}, {"UNIV", 3}, {"ACD", 1}, {"ARTS", 0}}}
    };
}
