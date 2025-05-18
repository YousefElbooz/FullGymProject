#include "filehandler.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include "member.h"
#include "staff.h"
#include "coach.h"
#include "manger.h"
#include "receptionists.h"
#include "gymclass.h"

QMap<int, Member*> FileHandler::loadMembers(const QString& filePath, QMap<int, Member*>& members, const QMap<int, GymClass*>& classesmap) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open member file";
        return members;
    }

    QTextStream in(&file);
    Member* m = nullptr;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            m = nullptr;
            continue;
        }

        // Member line
        if (!line.startsWith("Class: ")) {
            QStringList parts = line.split(":");
            if (parts.size() >= 9) {  // Changed from == 9 to >= 9 to handle optional subscription fields
                int id = parts[0].toInt();
                QString name = parts[1];
                QString email = parts[2];
                QString password = parts[3];
                QString gender = parts[4];
                bool isVip = (parts[5].toLower() == "true");
                QString phone = parts[6];
                QString address = parts[7];
                int age = parts[8].toInt();
                m = new Member(name, email, password, gender, isVip, phone, address, age);
                
                // Handle subscription data if available
                if (parts.size() >= 12) {
                    QString subscriptionType = parts[9];
                    QDate startDate = QDate::fromString(parts[10], Qt::ISODate);
                    QDate endDate = QDate::fromString(parts[11], Qt::ISODate);
                    
                    if (!subscriptionType.isEmpty() && startDate.isValid() && endDate.isValid()) {
                        m->setSubscription(subscriptionType);
                        m->subscriptionStartDate = startDate;
                        m->subscriptionEndDate = endDate;
                    }
                }
                
                // Load profile picture path if present
                if (parts.size() >= 13) {
                    m->setProfilePicturePath(parts[12]);
                } else {
                    m->setProfilePicturePath("");
                }
                
                // Load workouts if present
                if (parts.size() >= 14) {
                    QStringList workoutEntries = parts[13].split("|");
                    QStack<QPair<QString, QDate>> workoutStack;
                    for (const QString& entry : workoutEntries) {
                        QStringList workoutParts = entry.split("~");
                        if (workoutParts.size() == 2) {
                            QString workoutText = workoutParts[0].trimmed();
                            QDate date = QDate::fromString(workoutParts[1].trimmed(), Qt::ISODate);
                            if (!workoutText.isEmpty() && date.isValid()) {
                                workoutStack.push(qMakePair(workoutText, date));
                            }
                        }
                    }
                    m->setWorkouts(workoutStack);
                }
                
                members[id] = m;
            }
        }
        // Class line
        else if (m != nullptr) {  // Only process class if we have a valid member
            QString className = line.mid(QString("Class: ").length()).trimmed();
            // Find the class by name in classesmap
            for (auto gc : classesmap) {
                if (gc->getName().compare(className, Qt::CaseInsensitive) == 0) {
                    m->addClass(gc);
                    gc->addMember(m);  // Add member to class
                    gc->setEnrolled(gc->getEnrolled() + 1);  // Update enrolled count
                    break;
                }
            }
        }
    }
    file.close();
    return members;
}

QMap<int, Staff*> FileHandler::loadStaff(const QString& filePath,QMap<int, Staff*>& staffMap) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open staff file";
        return staffMap;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(":");
        if (parts.size() >= 8) {
            int id = parts[0].toInt();
            QString name = parts[1];
            QString email = parts[2];
            QString password = parts[3];
            int age = parts[4].toInt();
            QString address = parts[5];
            QString phone = parts[6];
            QString role = parts.last().toLower();

            Staff* staff = nullptr;

            if (role == "coach") {
                Coach* coach = new Coach(name, email, password, phone, address, age);
                // Read classes for coach
                while (!in.atEnd()) {
                    QString classLine = in.readLine().trimmed();
                    if (classLine == "---") break;
                    QStringList classParts = classLine.split("|");
                    if (classParts.size() >= 3) {
                        int classId = classParts[0].toInt();
                        QString className = classParts[1];
                        QString schedule = classParts[2];
                        GymClass* gymClass = new GymClass(className, schedule, 0);
                        gymClass->setId(classId);
                        gymClass->setCoach(coach);
                        coach->addClass(gymClass);
                    }
                }
                staff = coach;
            } else if (role == "manager") {
                staff = new Manger(name, email, password, phone, address, age);
            } else if (role == "receptionist") {
                staff = new Receptionist(name, email, password, phone, address, age);
            }

            if (staff) {
                staffMap[id] = staff;
            }
        }
    }

    file.close();
    return staffMap;
}

void FileHandler::saveMembers(const QString& filePath, const QMap<int, Member*>& members) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open member file for writing";
        return;
    }

    QTextStream out(&file);
    for (auto it = members.begin(); it != members.end(); ++it) {
        Member* m = it.value();
        out << it.key() << ":" << m->getName() << ":" << m->getEmail()<<":"<< m->getPassword()
            << ":" << m->getGender() << ":" << (m->getIsVip() ? "true" : "false")
            << ":" << m->getPhone() << ":" << m->getAddress() << ":" << m->getAge();
        
        // Add subscription data if available
        if (!m->getSubscriptionType().isEmpty() && m->getSubscriptionStartDate().isValid() && m->getSubscriptionEndDate().isValid()) {
            out << ":" << m->getSubscriptionType()
                << ":" << m->getSubscriptionStartDate().toString(Qt::ISODate)
                << ":" << m->getSubscriptionEndDate().toString(Qt::ISODate);
        }
        
        // Always add profile picture path (even if empty)
        out << ":" << m->getProfilePicturePath();
        
        // Save workouts
        QStack<QPair<QString, QDate>> workouts = m->getWorkouts();
        QStringList workoutList;
        while (!workouts.isEmpty()) {
            auto entry = workouts.pop();
            workoutList.prepend(entry.first + "~" + entry.second.toString(Qt::ISODate));
        }
        out << ":" << workoutList.join("|");

        out << "\n";
        
        // Write enrolled classes
        for (auto gc : m->getClasses()) {
            out << "Class: " << gc->getName() << "\n";
        }
        out << "\n"; // Blank line after each member
    }
    file.close();
}

void FileHandler::saveStaff(const QString& filePath, const QMap<int, Staff*>& staffMap) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open staff file for writing";
        return;
    }

    QTextStream out(&file);

    for (auto it = staffMap.begin(); it != staffMap.end(); ++it) {
        Staff* s = it.value();
        QString role = s->getRole().toLower();

        // Write all staff in the same format, including email and password
        out << s->getId() << ":"
            << s->getName() << ":"
            << s->getEmail() << ":"
            << s->getPassword() << ":"
            << s->getAge() << ":"
            << s->getAddress() << ":"
            << s->getPhone() << ":"
            << role << "\n";

        // For coaches, write their unique classes after the staff line
        if (role == "coach") {
            Coach* coach = dynamic_cast<Coach*>(s);
            if (!coach) continue;

            // Use QSet to track unique class IDs
            QSet<int> uniqueClassIds;
            for (GymClass* gymClass : coach->getClasses()) {
                if (uniqueClassIds.contains(gymClass->getId())) continue;
                uniqueClassIds.insert(gymClass->getId());

                out << gymClass->getId() << "|"
                    << gymClass->getName() << "|"
                    << gymClass->getStatue() << "|"
                    << (gymClass->getCoach() ? gymClass->getCoach()->getName() : "Unknown") << "|"
                    << "Scheduled\n";
            }
            out << "---\n";
        }
    }

    file.close();
}

void FileHandler::saveClasses(const QString& filePath, const QMap<int, GymClass*>& gymClasses) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open gym classes file for writing";
        return;
    }

    QTextStream out(&file);
    for (auto it = gymClasses.begin(); it != gymClasses.end(); ++it) {
        GymClass* gymClass = it.value();
        QString coachName = gymClass->getCoach() ? gymClass->getCoach()->getName() : "None";
        out << gymClass->getId() << ","
            << gymClass->getName() << ","
            << gymClass->getTime().toString("hh:mm AP") << ","
            << gymClass->getStatue() << ","
            << gymClass->getCapacity() << ","
            << gymClass->getEnrolled() << ","
            << "Coach: " << coachName << "\n";
        // Write enrolled members by ID
        for (auto member : gymClass->getMembers()) {
            out << "Member: " << member->getId() << "\n";
        }
        out << "\n"; // Blank line after each class
    }
    file.close();
}

QMap<int, GymClass*> FileHandler::loadClasses(const QString& filePath, QMap<int, GymClass*>& gymClasses, const QMap<int, Member*>& members, const QMap<int, Staff*>& staffMap) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open gym classes file";
        return gymClasses;
    }

    QTextStream in(&file);
    GymClass* gc = nullptr;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith("#")) {
            gc = nullptr;
            continue;
        }
        if (line.startsWith("Member: ")) {
            if (gc) {
                QString memberIdStr = line.mid(QString("Member: ").length()).trimmed();
                bool ok = false;
                int memberId = memberIdStr.toInt(&ok);
                if (ok && members.contains(memberId)) {
                    gc->addMember(members[memberId]);
                }
            }
            continue;
        }
        // Class line
        QStringList parts = line.split(",");
        if (parts.size() < 7) {
            qWarning() << "Malformed class line: " << line;
            continue;
        }
        bool ok = false;
        int id = parts[0].toInt(&ok);
        if (!ok) continue;
        QString name = parts[1];
        QTime time = QTime::fromString(parts[2], "hh:mm AP");
        QString status = parts[3];
        int capacity = parts[4].toInt(&ok);
        if (!ok) continue;
        int enrolled = parts[5].toInt(&ok);
        if (!ok) continue;
        QString coachPart = parts[6].trimmed();
        QString coachName = coachPart.startsWith("Coach: ") ? coachPart.mid(7) : "None";

        gc = new GymClass(name, status, capacity);
        gc->setId(id);
        gc->setTime(time);
        gc->setEnrolled(enrolled);

        // Find and set the coach if one exists
        if (coachName != "None") {
            for (auto staff : staffMap) {
                if (staff->getName() == coachName && staff->getRole().toLower() == "coach") {
                    Coach* coach = dynamic_cast<Coach*>(staff);
                    if (coach) {
                        gc->setCoach(coach);
                        coach->addClass(gc);
                        break;
                    }
                }
            }
        }

        gymClasses[id] = gc;
    }
    file.close();
    return gymClasses;
}
