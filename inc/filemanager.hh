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
    void saveSonarConf(QFile &file, Sonar const &sonar);
    void readSonarConf(QFile &file, Sonar &sonar);
    void savePlatformConf(QFile &file, Platform const &platform);
    void readPlatformConf(QFile &file, Platform &platform);
private:
    QVector3D qStringToQVector3D(QString str);
    QString qVector3DtoQString(QVector3D vec);
};

#endif // FILEMANAGER_HH
