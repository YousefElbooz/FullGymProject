#ifndef STAFF_H
#define STAFF_H

#include <QString>

class Staff {
protected:
    static int lastId;    // Shared across all staff types
    int id;
    QString email;
    QString password;
    QString name;
    QString role;
    QString phone;    // New field
    QString address;  // New field
    int age;          // New field

public:
    Staff(const QString& name,const QString& email,const QString& password, const QString& role, const QString& phone, const QString& address, int age);
    int getId() const;
    QString getName() const;
    QString getRole() const;
    QString getEmail() const;
    QString getPassword() const;
    QString getPhone() const;
    QString getAddress() const;
    int getAge() const;

    void setPhone(const QString& phone);
    void setAddress(const QString& address);
    void setAge(int age);
    void setPassword(QString& password);
    void setEmail(QString& email);
    virtual QString toString() const = 0;
    virtual QString getDetails() const = 0; // Abstract method
};

#endif // STAFF_H
