#include "receptionists.h"

Receptionist::Receptionist(const QString& name,const QString& email,const QString& password, const QString& phone, const QString& address, int age)
    : Staff( name,email,password, "Receptionist", phone, address, age) {}


QString Receptionist::getDetails() const {
    return "Receptionist: " + name + " (ID: " + QString::number(id) + ")";
}

QString Receptionist::toString() const {
    return Staff::toString();
}
