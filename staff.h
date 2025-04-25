#ifndef STAFF_H
#define STAFF_H

#include <QString>

class Staff {
protected:
    static int lastId;    // Shared across all staff types
    int id;
    QString name;
    QString role;
    QString phone;    // New field
    QString address;  // New field
    int age;          // New field

public:
    Staff(const QString& name, const QString& role, const QString& phone, const QString& address, int age);
    int getId() const;
    QString getName() const;
    QString getRole() const;

    QString getPhone() const;
    QString getAddress() const;
    int getAge() const;

    void setPhone(const QString& phone);
    void setAddress(const QString& address);
    void setAge(int age);

    virtual QString toString() const = 0;
    virtual QString getDetails() const = 0; // Abstract method
};

#endif // STAFF_H
