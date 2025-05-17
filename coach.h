#ifndef COACH_H
#define COACH_H
#include <QVector>
#include "staff.h"
#include "gymclass.h"

class GymClass;
class Staff;

class Coach : public Staff {
private:
    QVector<GymClass*> classes;
public:
    Coach(const QString& name,const QString& email,const QString& password, const QString& phone, const QString& address, int age);
    QString getDetails() const override;
    QString toString() const override;

    void addClass(GymClass* gymClass);
    QVector<GymClass*> getClasses();

    //Check classes that have been assigned
        //make their classes workout
};

#endif // COACH_H
