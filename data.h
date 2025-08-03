#pragma once
#include <map>
#include <string>
#include <vector>
#include "scheduler.h"

// Load classes from CSV file
std::map<std::string, ClassInfo> loadClassesFromCSV(const std::string& filename);

// Load building distances from CSV file
std::map<std::string, std::map<std::string, int>> loadDistanceMatrixFromCSV(const std::string& filename);

