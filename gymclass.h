#ifndef GYMCLASS_H
#define GYMCLASS_H
#include <QVector>
#include <QTime>
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
    QString statue;
    int capacity;
    int enrolled;
    QTime time;
    QVector<Member*> members;
    Coach* coach;
public:
    GymClass(QString name, QString Statue, int capacity);

    void addMember(Member* member);
    void setCoach(Coach* coach);

    QVector<Member*> getMembers() const ;
    int getId()const;
    QString getName() const;
    QString getStatue() const;
    void setStatue(QString statue);
    static int getLastId();
    int getCapacity();
    Coach* getCoach() const;
    QTime getTime()const;
    void setTime(QTime time);
    QString toString() const;  // To display in listWidget
    void setId(int id);
    void setEnrolled(int enrolled);
    int getEnrolled() const;
    void removeMember(Member* member);
};

#endif // GYMCLASS_H
