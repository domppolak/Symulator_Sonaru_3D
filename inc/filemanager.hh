#ifndef FILEMANAGER_HH
#define FILEMANAGER_HH

#include <QString>
#include "sonar.hh"
#include "platform.hh"
#include <QFile>
#include <memory>

class FileManager
{
public:
    FileManager();
    bool saveSonarConf(QFile &file, Sonar const &sonar);
    bool readSonarConf(QFile &file, Sonar &sonar);
    bool savePlatformConf(QFile &file, Platform const &platform);
    bool readPlatformConf(QFile &file, Platform &platform);
private:
    QVector3D qStringToQVector3D(QString str);
    QString qVector3DtoQString(QVector3D vec);
};

#endif // FILEMANAGER_HH
