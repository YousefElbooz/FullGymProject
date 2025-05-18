#pragma once
#include <string>
#include <vector>
#include"Court.h"
#include"member.h"
using namespace std;
class BookingCourt {
public:
    string courtLocation;
    string date;
    vector<bool> timeSlots;


    BookingCourt();
    BookingCourt(const string& courtID, const string& date, const vector<bool>& timeSlots);
};