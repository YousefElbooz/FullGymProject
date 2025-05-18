#pragma once
#include<iostream>
#include<string>
using namespace std;
class Court
{
public:
    string courtName;
    string location;
    string type;
    string startTime;
    string endTime;
    Court();
    Court(string, string, string, string, string);
};

