#pragma once
#include<iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;
class BookingLoader
{
public:
    
    static map<string,map<string,vector<bool>>> loadBookingsFromFile(const string& filename);
    static void saveBookingsToFile(const string& filename, const map<string, map<string, vector<bool>>>& bookings);
};
