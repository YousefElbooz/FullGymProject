#pragma once
#include <algorithm>
#include<iostream>
#include<string>
#include <vector>
#include <fstream>

using namespace std;


	 string normalizeString(const string& input);
	 string getDateAfterDays(int daysToAdd);
	 bool isValidDateFormat(string& date);
	 bool isValidTime(string& time);
	 string getDateAfterDays(const string& baseDate, int daysToAdd);
