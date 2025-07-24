#include "scheduler.h"
#include <sstream>
#include <algorithm>
#include <iostream>

int timeToMinutes(const std::string& timeStr) {
    int hours, minutes;
    char colon;
    std::istringstream iss(timeStr);
    iss >> hours >> colon >> minutes;
    return hours * 60 + minutes;
}

std::pair<int, int> parseTimeRange(const std::string& timeRange) {
    size_t dashPos = timeRange.find('-');
    std::string startStr = timeRange.substr(0, dashPos);
    std::string endStr = timeRange.substr(dashPos + 1);
    return { timeToMinutes(startStr), timeToMinutes(endStr) };
}

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

    int travelTime = distanceMatrix.at(class1.building).at(class2.building);
    if (end1 + travelTime <= start2 || end2 + travelTime <= start1)
        return false;

    return true;
}

// Recursive function to build all valid schedules
void backtrackSchedules(
    const std::vector<ClassInfo>& classes,
    const std::map<std::string, std::map<std::string, int>>& distanceMatrix,
    size_t start,
    std::vector<ClassInfo>& currentSchedule,
    std::vector<std::vector<ClassInfo>>& allSchedules)
{
    // If current schedule is valid, add it to the result list
    if (!currentSchedule.empty()) {
        allSchedules.push_back(currentSchedule);
    }
    // Try adding each remaining class starting from 'start' index
    for (size_t i = start; i < classes.size(); ++i) {
        bool conflict = false;
        //Check for conflicts with classes already in currentSchedule
        for (const auto& scheduledClass : currentSchedule) {
            if (classesConflict(scheduledClass, classes[i], distanceMatrix)) {
                conflict = true;
                break;
            }
        }
        // If no conflict, include the class and recurse
        if (!conflict) {
            currentSchedule.push_back(classes[i]);
            backtrackSchedules(classes, distanceMatrix, i + 1, currentSchedule, allSchedules);
            currentSchedule.pop_back(); // backtrack after recursion
        }
    }
}

// Public function to generate all valid class schedules
std::vector<std::vector<ClassInfo>> generateNonConflictingCombinations(
    const std::vector<ClassInfo>& classes,
    const std::map<std::string, std::map<std::string, int>>& distanceMatrix)
{
    std::vector<std::vector<ClassInfo>> allSchedules;   // Stores all valid combos
    std::vector<ClassInfo> currentSchedule;             // Temporary holder while recursing

    //Begin recursive backtrack
    backtrackSchedules(classes, distanceMatrix, 0, currentSchedule, allSchedules);
    return allSchedules;
}

int calculateTotalTravelTime(const std::vector<ClassInfo>& schedule,
                             const std::map<std::string, std::map<std::string, int>>& distanceMatrix) {
    int total = 0;
    for (size_t i = 0; i + 1 < schedule.size(); ++i) {
        total += distanceMatrix.at(schedule[i].building).at(schedule[i + 1].building);
    }
    return total;
}

std::vector<std::vector<ClassInfo>> rankByBuildingDistance(
    const std::vector<std::vector<ClassInfo>>& schedules,
    const std::map<std::string, std::map<std::string, int>>& distanceMatrix) {

    std::vector<std::pair<int, std::vector<ClassInfo>>> scored;
    for (const auto& sched : schedules) {
        int travelTime = calculateTotalTravelTime(sched, distanceMatrix);
        scored.push_back({ travelTime, sched });
    }

    std::sort(scored.begin(), scored.end(),
                [](const auto& a, const auto& b) { return a.first > b.first; });

    std::vector<std::vector<ClassInfo>> ranked;
    for (const auto& [_, sched] : scored) {
        ranked.push_back(sched);
    }
    return ranked;
}

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

    std::sort(scored.begin(), scored.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });

    std::vector<std::vector<ClassInfo>> ranked;
    for (const auto& [_, sched] : scored) {
        ranked.push_back(sched);
    }
    return ranked;
}

std::vector<std::vector<ClassInfo>> returnTop3Schedules(
    const std::vector<std::vector<ClassInfo>>& rankedSchedules) {

    std::vector<std::vector<ClassInfo>> top3;
    size_t count = std::min<size_t>(3, rankedSchedules.size());
    for (size_t i = 0; i < count; ++i) {
        top3.push_back(rankedSchedules[i]);
    }
    return top3;
}

std::vector<std::vector<ClassInfo>> generateSchedule(
    const std::vector<std::string>& userSelectedClassIDs,
    const UserPreferences& userPrefs,
    const std::map<std::string, std::map<std::string, int>>& distanceMatrix) {

    extern std::map<std::string, ClassInfo> loadClassesFromDB(); // forward declaration
    std::map<std::string, ClassInfo> classes = loadClassesFromDB();

    std::vector<ClassInfo> selectedClasses;
    for (const std::string& id : userSelectedClassIDs) {
        if (classes.count(id)) {
            selectedClasses.push_back(classes[id]);
        } else {
            std::cerr << "Class ID not found: " << id << "\n";
        }
    }

    auto possibleSchedules = generateNonConflictingCombinations(selectedClasses, distanceMatrix);

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
