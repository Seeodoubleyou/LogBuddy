#pragma once
#pragma once
#ifndef MODELS_H
#define MODELS_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <cstdint>
#include <stdio.h>
#include <algorithm>
#include <set>
#include <ctime>
#include <ratio>
#include <chrono>
#include <map>
#include <filesystem>
#include <sstream>
#include "stat.h"


using namespace std;

class Models {
public:
	map<string, int> csMethodDict;
	map<string, int> cipDict;
	map <string, int> httpStatusDict;
	vector<int> allResponseTimes;
	map <string, vector<int>> resourceResponseTimes;
	Stat allResponseTimeStats;
	map<string, Stat> resourceStats;
};
#endif