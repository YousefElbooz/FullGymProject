#include "BookingCourt.h"
#include"HelperFunction.h"
#include"Court.h"
#include"member.h"
#include<map>
#include <iostream>


BookingCourt::BookingCourt() : courtLocation(""), date(""), timeSlots() {}

BookingCourt::BookingCourt(const string& courtID, const string& date, const vector<bool>& timeSlots)
    : courtLocation(courtID), date(date), timeSlots(timeSlots) {}




