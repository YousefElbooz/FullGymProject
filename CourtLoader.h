#pragma once
#include "Court.h"
#include <QVector>

class CourtLoader
{
public:
	static QVector<Court> loadCourtsFromFile(const QString& filename);
};

