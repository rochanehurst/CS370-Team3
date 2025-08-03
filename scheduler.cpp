#include "scheduler.h"
#include "data.h"  // for loadClassesFromCSV, loadDistanceMatrixFromCSV
#include <sstream>
#include <algorithm>
#include <iostream>

// Converts "HH:MM" time string to total minutes from midnight
int timeToMinutes(const std::string& timeStr) {
    int hours, minutes;
    char colon;
    std::istringstream iss(timeStr);
    iss >> hours >> colon >> minutes;
    return hours * 60 + minutes;
}

// Parses a time range string "HH:MM-HH:MM" into start and end minutes
std::pair<int, int> parseTimeRange(const std::string& timeRange) {
    size_t dashPos = timeRange.find('-');
    std::string startStr = timeRange.substr(0, dashPos);
    std::string endStr = timeRange.substr(dashPos + 1);
    return { timeToMinutes(startStr), timeToMinutes(endStr) };
}

// Checks if two classes conflict based on overlapping days, times, and travel time between buildings
bool classesConflict(const ClassInfo& class1, const ClassInfo& class2,
                     const std::map<std::string, std::map<std::string, int>>& distanceMatrix) {
    bool sameDay = false;
    for (const auto& d1 : class1.days) {
        for (const auto& d2 : class2.days) {
            if (d1 == d2) {
                sameDay = true;
                break;
            }
        }
        if (sameDay) break;
    }
    if (!sameDay) return false;

    auto [start1, end1] = parseTimeRange(class1.time);
    auto [start2, end2] = parseTimeRange(class2.time);

    if (end1 <= start2 || end2 <= start1)
        return false;

    int travelTime = 0;
    try {
        travelTime = distanceMatrix.at(class1.building).at(class2.building);
    } catch (const std::out_of_range&) {
        std::cerr << "Warning: Missing travel time between " << class1.building << " and " << class2.building << "\n";
        travelTime = 0; // Assume 0 if missing; adjust as needed
    }

    if (end1 + travelTime <= start2 || end2 + travelTime <= start1)
        return false;

    return true;
}

// Recursive backtracking to build all valid schedules (non-conflicting class combinations)
void backtrackSchedules(
    const std::vector<ClassInfo>& classes,
    const std::map<std::string, std::map<std::string, int>>& distanceMatrix,
    size_t start,
    std::vector<ClassInfo>& currentSchedule,
    std::vector<std::vector<ClassInfo>>& allSchedules)
{
    if (!currentSchedule.empty()) {
        allSchedules.push_back(currentSchedule);
    }

    for (size_t i = start; i < classes.size(); ++i) {
        bool conflict = false;
        for (const auto& scheduledClass : currentSchedule) {
            if (classesConflict(scheduledClass, classes[i], distanceMatrix)) {
                conflict = true;
                break;
            }
        }
        if (!conflict) {
            currentSchedule.push_back(classes[i]);
            backtrackSchedules(classes, distanceMatrix, i + 1, currentSchedule, allSchedules);
            currentSchedule.pop_back();
        }
    }
}

// Generate all non-conflicting class schedules from the selected classes
std::vector<std::vector<ClassInfo>> generateNonConflictingCombinations(
    const std::vector<ClassInfo>& classes,
    const std::map<std::string, std::map<std::string, int>>& distanceMatrix)
{
    std::vector<std::vector<ClassInfo>> allSchedules;
    std::vector<ClassInfo> currentSchedule;
    backtrackSchedules(classes, distanceMatrix, 0, currentSchedule, allSchedules);
    return allSchedules;
}

// Calculate total travel time in a schedule by summing distances between consecutive classes
int calculateTotalTravelTime(const std::vector<ClassInfo>& schedule,
                             const std::map<std::string, std::map<std::string, int>>& distanceMatrix) {
    int total = 0;
    for (size_t i = 0; i + 1 < schedule.size(); ++i) {
        try {
            total += distanceMatrix.at(schedule[i].building).at(schedule[i + 1].building);
        } catch (const std::out_of_range&) {
            std::cerr << "Warning: Missing travel time between " << schedule[i].building << " and " << schedule[i+1].building << "\n";
        }
    }
    return total;
}

// Rank schedules to prioritize minimizing total walking distance (travel time)
std::vector<std::vector<ClassInfo>> rankByBuildingDistance(
    const std::vector<std::vector<ClassInfo>>& schedules,
    const std::map<std::string, std::map<std::string, int>>& distanceMatrix) {

    std::vector<std::pair<int, std::vector<ClassInfo>>> scored;
    for (const auto& sched : schedules) {
        int travelTime = calculateTotalTravelTime(sched, distanceMatrix);
        scored.push_back({ travelTime, sched });
    }

    // Sort ascending by travel time (minimize walking)
    std::sort(scored.begin(), scored.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });

    std::vector<std::vector<ClassInfo>> ranked;
    for (const auto& [_, sched] : scored) {
        ranked.push_back(sched);
    }
    return ranked;
}

// Rank schedules by how many classes are in the preferred building
std::vector<std::vector<ClassInfo>> rankByPreferredBuilding(
    const std::vector<std::vector<ClassInfo>>& schedules,
    const std::string& preferredBuilding) {

    std::vector<std::pair<int, std::vector<ClassInfo>>> scored;
    for (const auto& sched : schedules) {
        int count = 0;
        for (const auto& cls : sched) {
            if (cls.building == preferredBuilding) ++count;
        }
        scored.push_back({ count, sched });
    }

    // Sort descending by count of classes in preferred building
    std::sort(scored.begin(), scored.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });

    std::vector<std::vector<ClassInfo>> ranked;
    for (const auto& [_, sched] : scored) {
        ranked.push_back(sched);
    }
    return ranked;
}

// Return top 3 schedules from ranked list
std::vector<std::vector<ClassInfo>> returnTop3Schedules(
    const std::vector<std::vector<ClassInfo>>& rankedSchedules) {

    std::vector<std::vector<ClassInfo>> top3;
    size_t count = std::min<size_t>(3, rankedSchedules.size());
    for (size_t i = 0; i < count; ++i) {
        top3.push_back(rankedSchedules[i]);
    }
    return top3;
}

// Main schedule generation function called from main.cpp
std::vector<std::vector<ClassInfo>> generateSchedule(
    const std::vector<std::string>& userSelectedClassIDs,
    const UserPreferences& userPrefs,
    const std::map<std::string, std::map<std::string, int>>& distanceMatrix)
{
    // Load class data from CSV file
    std::map<std::string, ClassInfo> classes = loadClassesFromCSV("csusm_classes.csv");

    // Select classes based on user-selected IDs
    std::vector<ClassInfo> selectedClasses;
    for (const std::string& id : userSelectedClassIDs) {
        if (classes.count(id)) {
            selectedClasses.push_back(classes[id]);
        } else {
            std::cerr << "Class ID not found: " << id << "\n";
        }
    }

    // Generate all non-conflicting class combinations
    auto possibleSchedules = generateNonConflictingCombinations(selectedClasses, distanceMatrix);

    // Rank the schedules according to user preferences
    std::vector<std::vector<ClassInfo>> rankedSchedules;
    if (userPrefs.priority == "Minimize Walking") {
        rankedSchedules = rankByBuildingDistance(possibleSchedules, distanceMatrix);
    } else if (userPrefs.priority.rfind("Preferred Building:", 0) == 0) {
        std::string building = userPrefs.priority.substr(19);
        building.erase(0, building.find_first_not_of(" \t"));
        rankedSchedules = rankByPreferredBuilding(possibleSchedules, building);
    } else {
        rankedSchedules = possibleSchedules;
    }

    return returnTop3Schedules(rankedSchedules);
}
