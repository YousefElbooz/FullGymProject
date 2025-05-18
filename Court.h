#pragma once
#include <QString>

class Court
{
public:
    QString m_courtName;
    QString m_location;
    QString m_type;
    QString m_startTime;
    QString m_endTime;
    
    Court();
    Court(const QString& courtName, const QString& location, const QString& type, 
          const QString& startTime, const QString& endTime);
};

