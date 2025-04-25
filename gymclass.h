#ifndef GYMCLASS_H
#define GYMCLASS_H
#include <QVector>
#include "member.h"
#include "coach.h"
class Member;  // Forward declare Member
class Coach;

class GymClass
{
private:
    static int lastId;
    int id;
    QString name;
    QString schedule;
    int capacity;
    QVector<Member*> members;
    Coach* coach;
public:
    GymClass(QString name, QString schedule, int capacity);

    void addMember(Member* member);
    void setCoach(Coach* coach);

    QVector<Member*> getMembers() const ;
    int getId()const;
    QString getName() const;
    QString getSchedule() const;
    static int getLastId();
    int getCapacity();
    Coach* getCoach() const;

    QString toString() const;  // To display in listWidget
    void setId(int id);
};

#endif // GYMCLASS_H
