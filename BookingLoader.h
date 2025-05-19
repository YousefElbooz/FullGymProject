#pragma once
#include <QString>
#include <QMap>
#include <QVector>

class BookingLoader
{
public:
    static QMap<QString, QMap<QString, QVector<bool>>> loadBookingsFromFile(const QString& filename);
    static void saveBookingsToFile(const QString& filename, const QMap<QString, QMap<QString, QVector<bool>>>& bookings);
};
