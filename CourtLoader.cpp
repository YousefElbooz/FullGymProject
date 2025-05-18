#include "CourtLoader.h"
#include"Court.h"
#include <fstream>
#include <sstream>

vector<Court> CourtLoader::loadCourtsFromFile(const string& fileName) {
    vector<Court> courts;
    ifstream file(fileName);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string courtName, location, type, startTime, endTime;

        getline(ss, courtName, '|');
        getline(ss, location, '|');
        getline(ss, type, '|');
        getline(ss, startTime, '|');
        getline(ss, endTime, '|');

        courts.emplace_back(courtName, location, type, startTime, endTime);
    }

    return courts;
}
