#include "member.h"
#include <QDateTime>

// Initialize static member
int Member::lastId = 0;

Member::Member() : id(0), isVip(false), age(0), profilePicturePath("") {}

Member::Member(const QString& name, const QString& email, const QString &password, const QString& gender, bool isVip, const QString& phone, const QString& address, int age)
    : id(++lastId),name(name), email(email),password(password), gender(gender), isVip(isVip), phone(phone), address(address), age(age), profilePicturePath("") {}

void Member::setName(const QString& newName) {name = newName;}
void Member::addClass(GymClass* gymClass) {classes.push_back(gymClass);}
void Member::setEmail(const QString& newEmail) {email = newEmail;}
void Member::setGender(const QString& newGender) {gender = newGender;}
void Member::setPhone (const QString& newPhone) {phone = newPhone;}
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
QStack<QPair<QString, QDate>> Member::getWorkouts()const {return workouts;}
void Member::setWorkouts(const QStack<QPair<QString, QDate>>& newWorkouts){this->workouts=newWorkouts;}
void Member::setAge(int age) { this->age = age; }



// Subscription methods
void Member::setSubscription(const QString& type) {
    // Store the old subscription type
    QString oldType = subscriptionType;
    subscriptionType = type;
    
    // Check if there's an active subscription
    bool hadActiveSubscription = isSubscriptionActive();
    QDate startDate;
    
    if (hadActiveSubscription) {
        // Extend from end date of current subscription
        startDate = subscriptionEndDate;
    } else {
        // Start from today
        startDate = QDate::currentDate();
    }
    
    if (!hadActiveSubscription) {
        // No active subscription, set start date to today
        subscriptionStartDate = QDate::currentDate();
    }
    // Otherwise keep the original start date
    
    // Calculate the new end date based on the subscription type
    if (type == "1m") {
        subscriptionEndDate = startDate.addMonths(1);
    } else if (type == "3m") {
        subscriptionEndDate = startDate.addMonths(3);
    } else if (type == "6m") {
        subscriptionEndDate = startDate.addMonths(6);
    } else if (type == "12m") {
        subscriptionEndDate = startDate.addMonths(12);
    } else {
        // Invalid type or no subscription
        subscriptionStartDate = QDate();
        subscriptionEndDate = QDate();
    }
}

void Member::setSubscriptionStartDate(const QDate& date) {
    subscriptionStartDate = date;
}

void Member::setSubscriptionEndDate(const QDate& date) {
    subscriptionEndDate = date;
}

QString Member::getSubscriptionType() const {
    return subscriptionType;
}

QDate Member::getSubscriptionStartDate() const {
    return subscriptionStartDate;
}

QDate Member::getSubscriptionEndDate() const {
    return subscriptionEndDate;
}

int Member::getDaysRemaining() const {
    if (!subscriptionEndDate.isValid()) {
        return 0;
    }
    
    QDate currentDate = QDate::currentDate();
    if (currentDate > subscriptionEndDate) {
        return 0;
    }
    
    return currentDate.daysTo(subscriptionEndDate);
}

bool Member::isSubscriptionActive() const {
    if (!subscriptionEndDate.isValid()) {
        return false;
    }
    
    return QDate::currentDate() <= subscriptionEndDate;
}

QString Member::toString() const {
    return QString("ID: %1, Name: %2, Email: %3, Gender: %4, VIP: %5, Phone: %6, Address: %7, Age: %8")
    .arg(id).arg(name).arg(email).arg(gender).arg(isVip ? "Yes" : "No").arg(phone).arg(address).arg(age);
}
void Member::removeClass(GymClass* gymClass) {
    classes.removeOne(gymClass);
}
