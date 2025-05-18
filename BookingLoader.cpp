#include "BookingLoader.h"
#include"HelperFunction.h"
#include<iostream>
#include <fstream>
#include <sstream>
using namespace std;

map<string, map<string,vector<bool>>> BookingLoader::loadBookingsFromFile(const string&filename)
{
    map<string, map<string,vector<bool>>> bookings;

    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string location, date, timeSlotStr;

        getline(ss, location, '|');
        getline(ss, date, '|');
        getline(ss, timeSlotStr);
        location = normalizeString(location);
        vector<bool> timeSlots;
        stringstream timeSS(timeSlotStr);
        int val;
        while (timeSS >> val) {
            timeSlots.push_back(val == 1);
        }

        bookings[location][date] = timeSlots;
    }

    return bookings;
}

void BookingLoader::saveBookingsToFile(const string& filename, const map<string, map<string, vector<bool>>>& bookings) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }

    map<string, map<string, vector<bool>>> dateToLocations;

    for (const auto& locationPair : bookings) {
        const string& location = locationPair.first;
        const auto& datesMap = locationPair.second;

        for (const auto& datePair : datesMap) {
            const string& date = datePair.first;
            const vector<bool>& timeSlots = datePair.second;

            dateToLocations[date][location] = timeSlots;
        }
    }

    for (const auto& datePair : dateToLocations) {
        const string& date = datePair.first;
        const auto& locationsMap = datePair.second;

        for (const auto& locationPair : locationsMap) {
            const string& location = locationPair.first;
            const vector<bool>& timeSlots = locationPair.second;

            file << location << "|" << date << "|";
            for (size_t i = 0; i < timeSlots.size(); ++i) {
                file << (timeSlots[i] ? "1" : "0");
                if (i != timeSlots.size() - 1) file << " ";
            }
            file << "\n";
        }
    }

    file.close();
}

