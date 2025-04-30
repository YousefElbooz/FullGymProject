#ifndef MEMBER_H
#define MEMBER_H

#include <QString>
#include <QVector>
#include <QString>
#include <QStack>
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
    QStack<QString> workout;
    //payment duration

public:
    Member();  // Default constructor
    Member(const QString& name, const QString& email,const QString& password, const QString& gender, bool isVip, const QString& phone, const QString& address, int age);

    void addClass(GymClass* gymClass);
    void setName(const QString& newName);
    void setEmail(const QString& newEmail);
    void setGender(const QString& newGender);
    void setIsVip(bool vipStatus);
    void setPhone(const QString& phone);
    void setAddress(const QString& address);
    void setAge(int age);
    void setWorkouts(const QStack<QString>& workout);
    int getId() const;
    QString getName() const;
    QString getPassword() const;
    QString getEmail() const;
    QString getGender() const;
    bool getIsVip() const;
    QString getPhone() const;
    QString getAddress() const;
    int getAge() const;
    QStack<QString> getWorkouts() const;

    /*--------------helper Functions----------*/
    QString toString() const;

    //Enroll classes
        //Search (fillter By class details)
    //keep track of workout
    //cancel classes
    //book padel court
        //search by (location, date ,time )
    //cancel padel court
        //Before class within 3 hours
};

#endif // MEMBER_H
