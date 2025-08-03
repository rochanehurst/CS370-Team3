#include <iostream>
#include "scheduler.h"
#include "data.h"

int main() {
    // Step 1: Load classes from CSV file
    const std::string classFile = "csusm_classes.csv";
    auto classes = loadClassesFromCSV(classFile);
    if (classes.empty()) {
        std::cerr << "Failed to load classes from CSV file: " << classFile << std::endl;
        return 1;
    }

    // Step 2: Load building distances from CSV file
    const std::string buildingFile = "CSUSM-Buildings.csv";
    auto distanceMatrix = loadDistanceMatrixFromCSV(buildingFile);
    if (distanceMatrix.empty()) {
        std::cerr << "Failed to load building distances from CSV file: " << buildingFile << std::endl;
        return 1;
    }

    // Step 3: Example user-selected classes (hardcoded for now)
    std::vector<std::string> userSelectedClassIDs = {
        "44163",
        "43050",
        "43817"
    };

    // Step 4: Define user preferences (hardcoded for now)
    UserPreferences userPrefs;
    userPrefs.priority = "Minimize Walking";

    // Step 5: Generate schedules using scheduler function
    std::vector<std::vector<ClassInfo>> topSchedules = generateSchedule(
        userSelectedClassIDs, userPrefs, distanceMatrix);

    // Step 6: Output schedules or message if none found
    if (topSchedules.empty()) {
        std::cout << "No valid schedules found (conflicts or missing data)." << std::endl;
    } else {
        for (size_t i = 0; i < topSchedules.size(); ++i) {
            std::cout << "Schedule " << (i + 1) << ":\n";
            for (const auto& cls : topSchedules[i]) {
                std::cout << "  " << cls.id << " - " << cls.name << " | "
                          << cls.time << " | Building: " << cls.building << " | Days: ";
                for (const auto& day : cls.days)
                    std::cout << day << " ";
                std::cout << "\n";
            }
            std::cout << "-------------------------\n";
        }
    }

    return 0;
}
