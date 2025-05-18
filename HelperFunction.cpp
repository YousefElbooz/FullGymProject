#include "HelperFunction.h"
#include <ctime>
#include <iomanip>
#include <fstream>
#include <sstream>
#include<map>
#include"BookingCourt.h"


string normalizeString(const string& input)
{
    string normalized = input;
    transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower); 
    return normalized;
}


string getDateAfterDays(int daysToAdd) {
    time_t now = time(0);
    /*time() built-in fun return el time delwaqty bs blseconds */
    /*time_t 3shan ely yrg3 mn el fun yt5zn feha zy datatype*/
    tm ltm;
    /* tm zy struct kda  to store data like year-month-day*/

    localtime_s(&ltm, &now);
    /*hna b7wl elseconds to date w b5zno fe ltm*/


    ltm.tm_mday += daysToAdd;/*bzwd 3la el date el days */

    mktime(&ltm);/*3shan my3desh el boundries*/


    stringstream ss;
    ss << (ltm.tm_year + 1900) << "-"
        << setfill('0') << setw(2) << (ltm.tm_mon + 1) << "-"
        << setfill('0') << setw(2) << ltm.tm_mday;

    return ss.str();
}


bool isValidDateFormat(string& date)
{
    if (date.length() != 10 || date[4] != '-' || date[7] != '-') return false;
    for (int i = 0; i < date.length(); ++i) {
        if (i == 4 || i == 7) continue;/*3shan hma kda kda not numbers -- */
        if (!isdigit(date[i])) return false;
    }
    return true;
    /* check for format index 4 ==- index 7==- */
}


bool isValidTime(string& time)
{
    return time == "03:00" || time == "05:00" || time == "07:00" || time == "09:00";
    /*lw el time ely el user d5lo msh wahda mnhm return false */
    
}

string getDateAfterDays(const string& baseDate, int daysToAdd) {
    tm ltm = {};
    istringstream ss(baseDate);
    ss >> get_time(&ltm, "%Y-%m-%d");
    if (ss.fail()) {
        cerr << "Invalid date format. Expected YYYY-MM-DD.\n";
        return "";
    }

    ltm.tm_mday += daysToAdd;
    mktime(&ltm); 

    stringstream out;
    out << (ltm.tm_year + 1900) << "-"
        << setfill('0') << setw(2) << (ltm.tm_mon + 1) << "-"
        << setfill('0') << setw(2) << ltm.tm_mday;

    return out.str();
}



