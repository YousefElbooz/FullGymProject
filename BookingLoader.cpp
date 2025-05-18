#include "BookingLoader.h"
#include "HelperFunction.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>

QMap<QString, QMap<QString, QVector<bool>>> BookingLoader::loadBookingsFromFile(const QString& filename)
{
    QMap<QString, QMap<QString, QVector<bool>>> bookings;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return bookings;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split('|');
        
        if (parts.size() >= 3) {
            QString location = parts[0];
            QString date = parts[1];
            QString timeSlotStr = parts[2];
            
            location = normalizeString(location);
            
            QVector<bool> timeSlots;
            QStringList slotValues = timeSlotStr.split(' ', Qt::SkipEmptyParts);
            
            for (const QString& val : slotValues) {
                timeSlots.push_back(val == "1");
            }
            
            bookings[location][date] = timeSlots;
        }
    }
    
    file.close();
    return bookings;
}

void BookingLoader::saveBookingsToFile(const QString& filename, const QMap<QString, QMap<QString, QVector<bool>>>& bookings) 
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error opening file for writing:" << filename;
        return;
    }

    QMap<QString, QMap<QString, QVector<bool>>> dateToLocations;

    // Invert the map to organize by date first
    for (auto locationIt = bookings.constBegin(); locationIt != bookings.constEnd(); ++locationIt) {
        const QString& location = locationIt.key();
        const QMap<QString, QVector<bool>>& datesMap = locationIt.value();

        for (auto dateIt = datesMap.constBegin(); dateIt != datesMap.constEnd(); ++dateIt) {
            const QString& date = dateIt.key();
            const QVector<bool>& timeSlots = dateIt.value();

            dateToLocations[date][location] = timeSlots;
        }
    }

    QTextStream out(&file);
    
    // Write organized data to file
    for (auto dateIt = dateToLocations.constBegin(); dateIt != dateToLocations.constEnd(); ++dateIt) {
        const QString& date = dateIt.key();
        const QMap<QString, QVector<bool>>& locationsMap = dateIt.value();

        for (auto locationIt = locationsMap.constBegin(); locationIt != locationsMap.constEnd(); ++locationIt) {
            const QString& location = locationIt.key();
            const QVector<bool>& timeSlots = locationIt.value();

            out << location << "|" << date << "|";
            
            for (int i = 0; i < timeSlots.size(); ++i) {
                out << (timeSlots[i] ? "1" : "0");
                if (i != timeSlots.size() - 1) out << " ";
            }
            
            out << "\n";
        }
    }
    
    file.close();
}

