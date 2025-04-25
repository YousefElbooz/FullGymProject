#include "gymclass.h"
int GymClass::lastId =0;

GymClass::GymClass(QString name, QString schedule, int capacity)
    : name(name), schedule(schedule), capacity(capacity), coach(nullptr) {
    this->id = ++lastId;  // Increment the static lastId for each new GymClass object
}

void GymClass::addMember(Member* member) {
    if (members.size() < capacity) {
        members.push_back(member);
        member->addClass(this);  // Add this class to the member's class list
    } else {
        //handle full capacity
    }
}

// Set the coach for the class
void GymClass::setCoach(Coach* coach) {
    this->coach = coach;
}

// Accessor methods
int GymClass::getId() const {
    return id;
}

QString GymClass::getName() const {
    return name;
}

QVector<Member*> GymClass::getMembers() const{
    return members;
}

QString GymClass::getSchedule() const {
    return schedule;
}

// Static method to get the last used ID
int GymClass::getLastId() {
    return lastId;
}

QString GymClass::toString() const {
    return QString("Class: %1 | Schedule: %2 | Capacity: %3").arg(name).arg(schedule).arg(capacity);
}

void GymClass::setId(int id) {
    this->id = id;
}

Coach* GymClass::getCoach() const{
    return coach;
}

int GymClass::getCapacity(){
    return this->capacity;
}
