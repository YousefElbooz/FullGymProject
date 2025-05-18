#pragma once
#include "Court.h"
#include<iostream>
#include<string>
#include<vector>
class CourtLoader
{
public:
	static vector<Court> loadCourtsFromFile(const string& filename);
};

