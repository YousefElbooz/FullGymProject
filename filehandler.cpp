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
        if (line.isEmpty() || line.startsWith("#")) {
            m = nullptr;
            continue;
        }
        if (line.startsWith("Class: ")) {
            if (m) {
                QString className = line.mid(QString("Class: ").length()).trimmed();
                // Find the class by name in classesmap (now passed as parameter)
                for (auto gc : classesmap) {
                    if (gc->getName().compare(className, Qt::CaseInsensitive) == 0) {
                        m->addClass(gc);
                        break;
                    }
                }
            }
            continue;
        }
        // Member line
        QStringList parts = line.split(":");
        if (parts.size() == 9) {
            int id = parts[0].toInt();
            QString name = parts[1];
            QString email = parts[2];
            QString password = parts[3];
            QString gender = parts[4];
            bool isVip = (parts[5].toLower() == "true");
            QString phone = parts[6];
            QString address = parts[7];
            int age = parts[8].toInt();
            m = new Member(name, email,password, gender, isVip, phone, address, age);
            members[id] = m;
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
                Coach* coach = new Coach(name,email,password, phone, address, age);
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
                staff = new Manger(name,email,password, phone, address, age);
            } else if (role == "receptionist") {
                staff = new Receptionist(name, email,password,phone, address, age);
            }

            if (staff) {
                staffMap[staff->getId()] = staff;
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
            << ":" << m->getPhone() << ":" << m->getAddress() << ":" << m->getAge() << "\n";
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

        if (role == "coach") {
            Coach* coach = dynamic_cast<Coach*>(s);
            if (!coach) continue;

            out << coach->getId() << ":"
                << coach->getName() << ":"
                << coach->getAge() << ":"
                << coach->getAddress() << ":"
                << coach->getPhone() << ":"
                << "N/A:N/A:N/A:N/A:"  // Placeholders
                << "coach\n";

            for (GymClass* gymClass : coach->getClasses()) {
                out << gymClass->getId() << "|"
                    << gymClass->getName() << "|"
                    << gymClass->getStatue() << "|"
                    << (gymClass->getCoach() ? gymClass->getCoach()->getName() : "Unknown") << "|"
                    << "Scheduled\n";
            }

            out << "---\n";
        } else {
            out << s->getId() << ":"
                << s->getName() << ":"
                << s->getAge() << ":"
                << s->getAddress() << ":"
                << s->getPhone() << ":"
                << "N/A:N/A:N/A:N/A:" << role << "\n";
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

QMap<int, GymClass*> FileHandler::loadClasses(const QString& filePath, QMap<int, GymClass*>& gymClasses, const QMap<int, Member*>& members) {
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
        QString timeStr = parts[2];
        QString status = parts[3];
        int capacity = parts[4].toInt(&ok);
        if (!ok) continue;
        int enrolled = parts[5].toInt(&ok);
        if (!ok) continue;
        QString coachPart = parts[6].trimmed();
        QString coachName = coachPart.startsWith("Coach: ") ? coachPart.mid(7) : "None";
        gc = new GymClass(name, timeStr, capacity);
        gc->setId(id);
        gc->setStatue(status);
        gc->setEnrolled(enrolled);
        // Note: gc->setCoach() should be set if coach lookup is supported
        gymClasses[id] = gc;
    }
    file.close();
    return gymClasses;
}
