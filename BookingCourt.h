#pragma once
#include <QString>
#include <QVector>
#include "Court.h"

class BookingCourt {
public:
    QString m_courtLocation;
    QString m_date;
    QVector<bool> m_timeSlots;

    BookingCourt();
    BookingCourt(const QString& courtLocation, const QString& date, const QVector<bool>& timeSlots);
};