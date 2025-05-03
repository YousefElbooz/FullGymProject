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


// Forward declarations
class Member;
class Staff;
class GymClass;
class Coach;

class FileHandler {
public:
    // Load functions
    static QMap<int, Member*> loadMembers(const QString& filePath);
    static QMap<int, Staff*> loadStaff(const QString& filePath);
    static QMap<int, GymClass*> loadClasses(const QString& filePath);  // ✅ Add thi
    static  void loadQueueMembers(const QString&, QQueue<Member*>&, QQueue<Member*>&);

    // Save functions
    static void saveMembers(const QString& filePath, const QMap<int, Member*>& members);
    static void saveStaff(const QString& filePath, const QMap<int, Staff*>& staffMap);
    static void saveClasses(const QString& filePath, const QMap<int, GymClass*>& classes);  // ✅ Add this
    static void saveQueueData(const QString& filePath, const QQueue<Member*>& normalList, const QQueue<Member*>& VIPList) ;

};

#endif // FILEHANDLER_H
