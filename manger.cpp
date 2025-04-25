#include "manger.h"

Manger::Manger(const QString& name, const QString& phone, const QString& address, int age)
    : Staff( name, "Manager", phone, address, age) {}

QString Manger::getDetails() const {
    return "Manager: " + name + " (ID: " + QString::number(id) + ")";
}



QString Manger::toString() const {
    return Staff::toString();
}


QString Manger::assignClass(Coach* coach,GymClass* gymClass){
    coach->addClass(gymClass);
    gymClass->setCoach(coach);
}
