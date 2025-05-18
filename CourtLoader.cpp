#include "CourtLoader.h"
#include "Court.h"
#include <QFile>
#include <QTextStream>

QVector<Court> CourtLoader::loadCourtsFromFile(const QString& fileName) {
    QVector<Court> courts;
    QFile file(fileName);
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return courts;
        
    QTextStream in(&file);
    
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split('|');
        
        if (parts.size() >= 5) {
            QString courtName = parts[0];
            QString location = parts[1];
            QString type = parts[2];
            QString startTime = parts[3];
            QString endTime = parts[4];
            
            courts.append(Court(courtName, location, type, startTime, endTime));
        }
    }
    
    file.close();
    return courts;
}
