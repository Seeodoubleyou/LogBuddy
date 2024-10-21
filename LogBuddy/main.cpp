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
#include <numeric>
#include "models.h"





using namespace std;
namespace fs = std::filesystem; // Shortens "std::filesystem::(operator)" to "fs::(operator)"

vector<string> multipleLogsReader(string directory)
{
	vector<string> logData;
	string folderPath = directory;
	for (auto& entry : fs::directory_iterator(folderPath)) { // Allows us to iterate over elements in a container or range, in other words (For each entry that is within the directory given, do this) name of file not contents
		// Check if the entry is a regular file and has a .txt extension
		if (fs::is_regular_file(entry) && entry.path().extension() == ".log") { // Just want the files in this folder, not a folder or link etc...
			ifstream file(entry.path()); // Different way of saying ifstream file; file.open(entry.path());
			string line;

			cout << "Reading file: " << entry.path().filename() << std::endl;
			// Read contents of file (DOM (Document object model) Parser)
			// DOM = read all contents into memory (an alternative would be a SAX parser)
			// SAX parser would be more memory efficient but more complex
			while (getline(file, line)) { // eof bad practice???? ... assuming this is a read only operation
				logData.push_back(line);
			}
			file.close(); // not sure why we need this cause it worked fine without it (unloads from memory)

			cout << endl; // Add a new line between files
		}
	}
	return logData;
}

void logPrinter(vector<string> v)
{
	cout << "Printing contents...\n";
	int s = v.size();
	for (int i = 0; i < s; i++)
	{
		cout << v[i] << endl;
	}
}

vector<string> split(string& s, const string& delimiter) { // putting const in front of string& s will cause a compiler error on s.erase(0, pos + delimiter.length());

	stringstream test(s);
	string segment;
	vector<string> segList;

	while (getline(test, segment, ' ')) // Location, where its being stored, delimiter ( Search test until it finds ' ' and then store it in segment and keep doing it until the string is done
	{
		segList.push_back(segment);
	}

	return segList;
}

string splittedString(vector<string> v, int timesCalled)
{
	string stringNumber;
	stringNumber = v[timesCalled];
	return stringNumber; // each row will return as a string through this function after going through the loop
}

void printcsMethodData(map<string, int> m)
{
	for (pair<const string, int> myPair : m) {
		cout << "Type: " << myPair.first << ", Occurence: " << myPair.second << std::endl;
	}
}

Models dataReturn(vector<string> v) // why cant i use a pointer here
{
	Models models;
	string delimiter = " ";
	string stringForSplit;
	for (int i = 0; i < v.size(); i++)
	{
		stringForSplit = splittedString(v, i);
		if (stringForSplit.at(0) == '#') // Skipping IIS header (strings that start with #)
			continue;
		vector<string> v2 = split(stringForSplit, delimiter); // can we change this to just v
		// #Fields: date time s-ip cs-method cs-uri-stem cs-uri-query s-port cs-username c-ip cs(User-Agent) cs(Referer) sc-status sc-substatus sc-win32-status sc-bytes cs-bytes time-taken
		//           0    1    2      3          4           5           6        7       8        9            10          11         12             13           14      15         16

		if (models.csMethodDict.find(v2[3]) == models.csMethodDict.end()) {
			// not found
			models.csMethodDict.insert({ v2[3], 1 });
		}
		else {
			// found
			int i = models.csMethodDict[v2[3]];
			i++;
			models.csMethodDict[v2[3]] = i;
		}
		if (models.cipDict.find(v2[8]) == models.cipDict.end()) {
			// not found
			models.cipDict.insert({ v2[8], 1 });
		}
		else {
			// found
			int i = models.cipDict[v2[8]];
			i++;
			models.cipDict[v2[8]] = i;
		}
		if (models.httpStatusDict.find(v2[11]) == models.httpStatusDict.end()) {
			// not found
			models.httpStatusDict.insert({ v2[11], 1 });
		}
		else {
			// found
			int i = models.httpStatusDict[v2[11]];
			i++;
			models.httpStatusDict[v2[11]] = i;
		}

		int j = stoi(v2[16]); // unsafe (assumes the value is an integer)
		models.allResponseTimes.push_back(j);


		if (models.resourceResponseTimes.find(v2[4]) == models.resourceResponseTimes.end()) {
			// not found
			vector<int> list;// = new vector < int >();
			list.push_back(j);
			models.resourceResponseTimes.insert({ v2[4], list });
		}
		else {
			// found
			vector<int> list = models.resourceResponseTimes[v2[4]];
			list.push_back(j);
			models.resourceResponseTimes[v2[4]] = list;
		}

	}
	return models;
}


Models dataAnalysis(Models models)
{
	// Calculate all stats
	// count
	models.allResponseTimeStats.responseTimeCount = models.allResponseTimes.size();  // Object inside of an object
	// sum of the vector elements 
	models.allResponseTimeStats.responseTimeSum = accumulate(models.allResponseTimes.begin(), models.allResponseTimes.end(), 0.0);

	// average of the vector elements 
	models.allResponseTimeStats.responseTimeAverage = models.allResponseTimeStats.responseTimeSum / models.allResponseTimeStats.responseTimeCount;


	cout << "Printing contents...\n";
	for (auto keyvalue = models.resourceResponseTimes.begin(); keyvalue != models.resourceResponseTimes.end(); ++keyvalue) // instead of auto, you could state the actual object type of map<string, int>::iterator
	{
		// keyvalue->first is key
		// keyvalue->second is value
		vector<int> v = keyvalue->second;
		int count = v.size();
		long sum = accumulate(v.begin(), v.end(), 0.0);
		double avg = sum / count;
		Stat s;
		s.responseTimeAverage = avg;
		s.responseTimeCount = count;
		s.responseTimeSum = sum;
		models.resourceStats[keyvalue->first] = s;
	}


	return models;
}

void printcsMethodData(map<string, Stat> m)
{
	for (pair<const string, Stat> myPair : m) {
		Stat s = myPair.second;

		cout << "Resource: " << myPair.first << ", Average: " << s.responseTimeAverage << std::endl;
		cout << "Resource: " << myPair.first << ", Sum: " << s.responseTimeSum << std::endl;
		cout << "Resource: " << myPair.first << ", Count: " << s.responseTimeCount << std::endl;
	}
}

void generateReport(Models m)
{
	printcsMethodData(m.csMethodDict);
	printcsMethodData(m.cipDict);
	printcsMethodData(m.httpStatusDict);
	cout << "Overall Stats: " << endl;
	cout << "Overall Average: " << endl;
	cout << m.allResponseTimeStats.responseTimeAverage << endl;
	cout << "Overall Count: " << endl;
	cout << m.allResponseTimeStats.responseTimeCount << endl;
	cout << "Overall Sum: " << endl;
	cout << m.allResponseTimeStats.responseTimeSum << endl;

	cout << "Resource Stats: " << endl;
	printcsMethodData(m.resourceStats);
}

string processFilePath(string path) { // To make sure that the paths backslashes dont mess everything up
	size_t pos = 0;
	while ((pos = path.find('\\', pos)) != string::npos) {
		path.insert(pos, "\\");
		pos += 2; // Move past the newly inserted double backslash
	}
	return path;
}

int main()
{
	string userInputPath;
	cout << "Enter the file path of the folder containing the logs: ";
	getline(cin, userInputPath);
	string processedPath = processFilePath(userInputPath);
	cout << processedPath;
	vector<string> v = multipleLogsReader(processedPath);
	Models m = dataReturn(v);
	m = dataAnalysis(m);
	generateReport(m);

	return 0; // /
}