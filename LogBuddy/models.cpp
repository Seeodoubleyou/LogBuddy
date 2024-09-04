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
#include "models.h"
#include "stat.h"

using namespace std;


// raw data
map<string, int> csMethodDict;
map<string, int> cipDict;
map <string, int> httpStatusDict;
vector<int> allResponseTimes;
map <string, vector<int>> resourceResponseTimes;

//stats
Stat allResponseTimeStats; // Object within an object its objectception
map<string, Stat> resourceStats;
