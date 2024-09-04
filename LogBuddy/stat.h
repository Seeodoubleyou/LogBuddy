#pragma once
#pragma once
#pragma once
#ifndef STAT_H
#define STAT_H

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

using namespace std;

class Stat {
public:
	int responseTimeCount;
	long responseTimeSum;
	double responseTimeAverage;
	int responseTimeMax;
	int responseTimeMin;
};
#endif