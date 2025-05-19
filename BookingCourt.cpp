#include "BookingCourt.h"
#include "Court.h"
#include <QDebug>

BookingCourt::BookingCourt() 
    : m_courtLocation(""), m_date(""), m_timeSlots() {}

BookingCourt::BookingCourt(const QString& courtLocation, const QString& date, const QVector<bool>& timeSlots)
    : m_courtLocation(courtLocation), m_date(date), m_timeSlots(timeSlots) {}




