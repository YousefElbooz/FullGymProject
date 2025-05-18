#include "Court.h"

Court::Court()
    : courtName(""), location(""), type(""), startTime(""), endTime("") {}

Court::Court(string cName, string loc, string ty, string sTime, string eTime)
    : courtName(cName), location(loc), type(ty), startTime(sTime), endTime(eTime) {}


