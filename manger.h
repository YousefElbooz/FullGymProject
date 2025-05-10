#ifndef MANGER_H
#define MANGER_H

#include "staff.h"
#include "gymClass.h"
#include "coach.h"

class Manger : public Staff {
public:
    Manger(const QString& name,const QString& email,const QString& password, const QString& phone, const QString& address, int age);
    QString getDetails() const override;
    QString toString() const override;
    QString assignClass(Coach* coach,GymClass* gymClass);

    //store->all members data
    //retrive
    //search

};

#endif // MANAGER_H
