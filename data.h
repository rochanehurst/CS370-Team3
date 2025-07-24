#pragma once
#include "scheduler.h"
#include <map>

std::map<std::string, ClassInfo> loadClassesFromDB();
std::map<std::string, std::map<std::string, int>> getDistanceMatrix();
