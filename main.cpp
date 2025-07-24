#include <iostream>
#include "scheduler.h"
#include "data.h"

int main() {
    // Step 1: Load all class info from database
    // TO DO: Replace loadClassesFromDB() with actual data loading function
    std::vector<std::string> userSelectedClassIDs = {
        "CS370-01", "BIO210-01", "HIST105-01"
    };

    // Step 2: Define user preferences
    // TO DO: Replace with real user input or config file for preferences
    UserPreferences userPrefs;
    userPrefs.priority = "Minimize Walking";
    // userPrefs.priority = "Preferred Building: MARK";

    // Step 3: Load distance matrix
    // Static data
    std::map<std::string, std::map<std::string, int>> distanceMatrix = getDistanceMatrix();

    // Step 4: Generate best schedules based on selected classes and preferences
    // generateSchedule internally calls loadClassesFromDB() to get class data
    // Must update generateSchedule when changing class loading
    std::vector<std::vector<ClassInfo>> topSchedules = generateSchedule(
        userSelectedClassIDs, userPrefs, distanceMatrix);

    // Step 5: Output schedules or error message
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
