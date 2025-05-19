#pragma once
#include <QString>
#include <QDate>

QString normalizeString(const QString& input);
QString getDateAfterDays(int daysToAdd);
bool isValidDateFormat(const QString& date);
bool isValidTime(const QString& time);
QString getDateAfterDays(const QString& baseDate, int daysToAdd);
