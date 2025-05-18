#ifndef GYMCLASS_H
#define GYMCLASS_H
#include <QVector>
#include <QTime>
#include <queue>
#include "member.h"
#include "coach.h"
class Member;  // Forward declare Member
class Coach;

// Custom comparator for Member priority queue - implementation in cpp file
struct MemberPriorityCompare {
    bool operator()(Member* a, Member* b) const;
};

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
    std::priority_queue<Member*, std::vector<Member*>, MemberPriorityCompare> waitlist;  // Priority queue for waitlist

public:
    GymClass(QString name, QString Statue, int capacity);

    void addMember(Member* member);
    void setCoach(Coach* coach);
    void addToWaitlist(Member* member);  // Add member to waitlist with priority
    void processWaitlist();  // Process waitlist when a spot becomes available

    QVector<Member*> getMembers() const;
    QVector<Member*> getWaitlistAsVector() const;  // Get waitlist as a vector for saving/UI display
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
