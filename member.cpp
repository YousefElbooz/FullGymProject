#include "member.h"

// Initialize static member
int Member::lastId = 0;

Member::Member() : id(0), isVip(false), age(0) {}

Member::Member(const QString& name, const QString& email, const QString &password, const QString& gender, bool isVip, const QString& phone, const QString& address, int age)
    : id(++lastId),name(name), email(email),password(password), gender(gender), isVip(isVip), phone(phone), address(address), age(age) {}

void Member::setName(const QString& newName) {name = newName;}
void Member::addClass(GymClass* gymClass) {classes.push_back(gymClass);gymClass->getMembers().append(this);}
void Member::setEmail(const QString& newEmail) {email = newEmail;}
void Member::setGender(const QString& newGender) {gender = newGender;}
void Member::setIsVip(bool vipStatus) {isVip = vipStatus;}
int Member::getId() const {return id;}
QString Member::getName() const {return name;}
QString Member::getPassword() const {return password;}
QString Member::getEmail() const {return email;}
QString Member::getGender() const {return gender;}
bool Member::getIsVip() const {return isVip;}
QString Member::getPhone() const { return phone; }
QString Member::getAddress() const { return address; }
int Member::getAge() const { return age; }

QString Member::toString() const {
    return QString("ID: %1, Name: %2, Email: %3, Gender: %4, VIP: %5, Phone: %6, Address: %7, Age: %8")
    .arg(id).arg(name).arg(email).arg(gender).arg(isVip ? "Yes" : "No").arg(phone).arg(address).arg(age);
}
