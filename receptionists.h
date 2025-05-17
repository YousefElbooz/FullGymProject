#ifndef RECEPTIONIST_H
#define RECEPTIONIST_H

#include "staff.h"

class Receptionist : public Staff {
public:
    Receptionist(const QString& name,const QString& email,const QString& password, const QString& phone, const QString& address, int age);
    QString getDetails() const override;
    QString toString() const override;

    //store member data
    //retrive
    //search
};

#endif // RECEPTIONIST_H
