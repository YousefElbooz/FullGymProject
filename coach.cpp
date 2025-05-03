#include "coach.h"

Coach::Coach(const QString& name,const QString& email,const QString& password, const QString& phone, const QString& address, int age)
    : Staff(name,email,password ,"Coach", phone, address, age) {}

QString Coach::getDetails() const {
    return "Coach: " + name + " (ID: " + QString::number(id) + ")";
}


QString Coach::toString() const {
    return Staff::toString();
}

void Coach::addClass(GymClass* gymClass) {
    classes.push_back(gymClass);
}

QVector<GymClass*> Coach::getClasses(){
    return classes;
}

