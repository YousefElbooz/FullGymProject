#include "gymclass.h"
#include "member.h"
#include "coach.h"

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
// addTowaitList
void GymClass::addToWaitList( Member* member) {
    if(member->getIsVip()){
         this->VIPList.enqueue(member);
    }
    else
        this->normalList.enqueue(member);
}
void GymClass::moveToClass() {
    if (VIPList.isEmpty() && !normalList.isEmpty()) {
        this->addMember(this->normalList.head());

        normalList.dequeue(); // Remove from front of queue
    }
    else if(!VIPList.isEmpty() ){
        this->addMember(this->VIPList.head());

        VIPList.dequeue(); // Remove from front of queue
    }
    else{     //(normalList isempty & vipList isempty)
      // make warning that there is no one in waitlists
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
//////////////////////////////////////////
QQueue<Member*>& GymClass::getVIPList() {
    return this->VIPList;
}

QQueue<Member*>& GymClass::getNormalList() {
    return this->normalList;
}
