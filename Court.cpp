#include "Court.h"

Court::Court()
    : m_courtName(""), m_location(""), m_type(""), m_startTime(""), m_endTime("") {}

Court::Court(const QString& courtName, const QString& location, const QString& type, 
             const QString& startTime, const QString& endTime)
    : m_courtName(courtName), m_location(location), m_type(type), 
      m_startTime(startTime), m_endTime(endTime) {}


