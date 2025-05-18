#ifndef MEMBER_H
#define MEMBER_H

#include <QString>
#include <QVector>
#include <QString>
#include <QStack>
#include <QDate>
#include "gymclass.h"
class GymClass;  // Forward declaration


class Member {
private:
    static int lastId;      // Static counter for unique ID
    int id;
    QString name;
    QString email;
    QString password;
    QString gender;
    bool isVip;
    QVector<GymClass*> classes;
    // QVector<PadelCourt*> classes;
    QString phone;   // New field
    QString address; // New field
    int age;         // New
    QStack<QPair<QString, QDate>> workouts;
    //payment duration
    QString subscriptionType; // "1m", "3m", "6m", "12m" or empty for non-subscribed
    QDate subscriptionStartDate;
    QDate subscriptionEndDate;
    QString profilePicturePath; // Path to profile picture

public:
    Member();  // Default constructor
    Member(const QString& name, const QString& email,const QString& password, const QString& gender, bool isVip, const QString& phone, const QString& address, int age);

    QStack<QPair<QString, QDate>> getWorkouts() const;
    void setWorkouts(const QStack<QPair<QString, QDate>>& newWorkouts);


    void addClass(GymClass* gymClass);
    void setName(const QString& newName);
    void setEmail(const QString& newEmail);
    void setGender(const QString& newGender);
    void setIsVip(bool vipStatus);
    void setPhone (const QString& newPhone);
    void setAddress(const QString& address);
    void setAge(int age);
    
    // Subscription methods
    void setSubscription(const QString& type);
    void setSubscriptionStartDate(const QDate& date);
    void setSubscriptionEndDate(const QDate& date);
    QString getSubscriptionType() const;
    QDate getSubscriptionStartDate() const;
    QDate getSubscriptionEndDate() const;
    int getDaysRemaining() const;
    bool isSubscriptionActive() const;
    
    int getId() const;
    QString getName() const;
    QString getPassword() const;
    QString getEmail() const;
    QString getGender() const;
    bool getIsVip() const;
    QString getPhone() const;
    QString getAddress() const;
    int getAge() const;
    QVector<GymClass*> getClasses() const { return classes; }

    /*--------------helper Functions----------*/
    QString toString() const;


    void removeClass(GymClass* gymClass);

    // Make FileHandler a friend class to access private members
    friend class FileHandler;

    void setProfilePicturePath(const QString& path) { profilePicturePath = path; }
    QString getProfilePicturePath() const { return profilePicturePath; }
};

#endif // MEMBER_H
