#include "HelperFunction.h"
#include <QDateTime>
#include <QRegularExpression>
#include <QDebug>
#include <iomanip>
#include <fstream>
#include <sstream>
#include<map>
#include"BookingCourt.h"

QString normalizeString(const QString& input)
{
    return input.toLower().trimmed();
}

QString getDateAfterDays(int daysToAdd) 
{
    QDate currentDate = QDate::currentDate();
    QDate futureDate = currentDate.addDays(daysToAdd);
    return futureDate.toString("yyyy-MM-dd");
}

bool isValidDateFormat(const QString& date)
{
    QRegularExpression dateRegex("^\\d{4}-\\d{2}-\\d{2}$");
    return dateRegex.match(date).hasMatch();
}

bool isValidTime(const QString& time)
{
    return time == "03:00" || time == "05:00" || time == "07:00" || time == "09:00";
}

QString getDateAfterDays(const QString& baseDate, int daysToAdd) 
{
    QDate date = QDate::fromString(baseDate, "yyyy-MM-dd");
    if (!date.isValid()) {
        qDebug() << "Invalid date format. Expected YYYY-MM-DD.";
        return "";
    }
    
    QDate futureDate = date.addDays(daysToAdd);
    return futureDate.toString("yyyy-MM-dd");
}



