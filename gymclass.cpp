#include "gymclass.h"
int GymClass::lastId =0;

GymClass::GymClass(QString name, QString statue, int capacity)
    : name(name), statue(statue), capacity(capacity), coach(nullptr) {
    this->id = ++lastId;
}

void GymClass::addMember(Member* member) {
    if (members.size() < capacity) {
        members.push_back(member);
        member->addClass(this);
    } else {
        //handle full capacity
    }
}
void GymClass::setCoach(Coach* coach) {this->coach = coach;}
void GymClass::setTime(QTime time) {this->time = time;}
int GymClass::getId() const {return id;}
QTime GymClass::getTime()const {return time;}
QString GymClass::getName() const {return name;}
QVector<Member*> GymClass::getMembers() const{return members;}
QString GymClass::getStatue() const {return statue;}
void GymClass::setStatue(QString statue)  {this->statue=statue;}
int GymClass::getLastId() {return lastId;}
QString GymClass::toString() const {return QString("Class: %1 | Schedule: %2 | Capacity: %3").arg(name).arg(statue).arg(capacity);}
void GymClass::setId(int id) {this->id = id;}
Coach* GymClass::getCoach() const{return coach;}
int GymClass::getCapacity() {return this->capacity;}
int GymClass::getEnrolled()const {return this->enrolled;}
void GymClass::setEnrolled(int enrolled){this->enrolled=enrolled;}
void GymClass::removeMember(Member* member) {
    members.removeOne(member);
}
