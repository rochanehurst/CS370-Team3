#pragma once
#include <string>
#include <vector>
#include <map>

struct ClassInfo {
    std::string id;
    std::string name;
    std::string building;
    std::vector<std::string> days;
    std::string time;
};

struct UserPreferences {
    std::string priority;
};

std::vector<std::vector<ClassInfo>> generateSchedule(
    const std::vector<std::string>& userSelectedClassIDs,
    const UserPreferences& userPrefs,
    const std::map<std::string, std::map<std::string, int>>& distanceMatrix);

bool classesConflict(const ClassInfo& class1, const ClassInfo& class2,
                     const std::map<std::string, std::map<std::string, int>>& distanceMatrix);

std::vector<std::vector<ClassInfo>> generateNonConflictingCombinations(
    const std::vector<ClassInfo>& classes,
    const std::map<std::string, std::map<std::string, int>>& distanceMatrix);

int calculateTotalTravelTime(const std::vector<ClassInfo>& schedule,
                             const std::map<std::string, std::map<std::string, int>>& distanceMatrix);

std::vector<std::vector<ClassInfo>> rankByBuildingDistance(
    const std::vector<std::vector<ClassInfo>>& schedules,
    const std::map<std::string, std::map<std::string, int>>& distanceMatrix);

std::vector<std::vector<ClassInfo>> rankByPreferredBuilding(
    const std::vector<std::vector<ClassInfo>>& schedules,
    const std::string& preferredBuilding);

std::vector<std::vector<ClassInfo>> returnTop3Schedules(
    const std::vector<std::vector<ClassInfo>>& rankedSchedules);

std::pair<int, int> parseTimeRange(const std::string& timeRange);
