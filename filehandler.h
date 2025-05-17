#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QMap>
#include <QString>
#include "filehandler.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include "member.h"
#include "coach.h"
#include "manger.h"
#include "receptionists.h"
#include "gymclass.h"
#include <QTableWidget>


// Forward declarations
class Member;
class Staff;
class GymClass;
class Coach;

class FileHandler {
public:
    // Load functions
    static QMap<int, Member*> loadMembers(const QString& filePath, QMap<int, Member*>& members, const QMap<int, GymClass*>& classesmap);
    static QMap<int, Staff*> loadStaff(const QString& filePath,QMap<int, Staff*>& staffMap);
    static QMap<int, GymClass*> loadClasses(const QString& filePath, QMap<int, GymClass*>& gymClasses, const QMap<int, Member*>& members, const QMap<int, Staff*>& staffMap);

    // Save functions
    static void saveMembers(const QString& filePath, const QMap<int, Member*>& members);
    static void saveStaff(const QString& filePath, const QMap<int, Staff*>& staffMap);
    static void saveClasses(const QString& filePath, const QMap<int, GymClass*>& classes);  // ✅ Add this
    //waitlist functions
    //    static QMap<int, GymClass*> loadClasses(const QString& filePath,QMap<int, GymClass*> gymClasses);  // ✅ Add thi
    //     static void saveClasses(const QString& filePath, const QMap<int, GymClass*>& classes);  // ✅ Add this

    //   static void saveQueueData(QTextStream &out, const QQueue<Member*>& normalList, const QQueue<Member*>& VIPList) ;
};

#endif // FILEHANDLER_H
