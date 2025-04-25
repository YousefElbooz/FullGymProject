#include "staff.h"

int Staff::lastId = 0; // Static ID initializer
Staff::Staff(const QString& name, const QString& role, const QString& phone, const QString& address, int age)
    : id(++lastId), name(name), role(role), phone(phone), address(address), age(age) {}

int Staff::getId() const {return id;}
QString Staff::getName() const {return name;}
QString Staff::getRole() const {return role;}
QString Staff::getPhone() const { return phone; }
QString Staff::getAddress() const { return address; }
int Staff::getAge() const { return age; }

void Staff::setPhone(const QString& phone) { this->phone = phone; }
void Staff::setAddress(const QString& address) { this->address = address; }
void Staff::setAge(int age) { this->age = age; }
QString Staff::toString() const {
    return QString("ID: %1, Name: %2, Role: %3, Phone: %4, Address: %5, Age: %6")
    .arg(id).arg(name).arg(role).arg(phone).arg(address).arg(age);
}
