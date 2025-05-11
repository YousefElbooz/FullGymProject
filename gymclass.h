#ifndef GYMCLASS_H
#define GYMCLASS_H
#include <QVector>
#include <QTime>
#include <QQueue>
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
    QQueue<Member*> normalWaitlist;  // Queue for normal members
    QQueue<Member*> vipWaitlist;     // Queue for VIP members

public:
    GymClass(QString name, QString Statue, int capacity);

    void addMember(Member* member);
    void setCoach(Coach* coach);
    void addToWaitlist(Member* member);  // Add member to appropriate waitlist
    void processWaitlist();  // Process waitlist when a spot becomes available

    QVector<Member*> getMembers() const;
    QQueue<Member*>& getNormalList() { return normalWaitlist; }
    QQueue<Member*>& getVIPList() { return vipWaitlist; }
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