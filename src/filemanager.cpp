#include "filemanager.hh"
#include "qvectornd.h"
#include <QFile>
#include <QtXml>
#include <QTextStream>

FileManager::FileManager()
{

}

bool FileManager::saveSonarConf(QFile &file, Sonar const &sonar)
{
    QDomDocument xml("sonar_configuration");

    QDomElement root = xml.createElement("Sonar");

    root.setAttribute("Id", QString::number(sonar.get_id()));
    root.setAttribute("NumOfRec" ,QString::number(sonar.get_numOfRec()));
    root.setAttribute("Size", qVector3DtoQString(sonar.get_size()));
    root.setAttribute("LocPos", qVector3DtoQString(sonar.get_LocPos()));
    root.setAttribute("Color", qVector3DtoQString(sonar.get_color()));
    root.setAttribute("Rotation", QString::number(sonar.get_rotation()));

    xml.appendChild(root);

    QDomElement transmitter = xml.createElement("Transmitter");
    root.appendChild(transmitter);
    transmitter.setAttribute("LocPos", qVector3DtoQString(sonar.transmitter_getLocPos()));
    transmitter.setAttribute("Size", qVector3DtoQString(sonar.transmitter_getSize()));
    transmitter.setAttribute("Rotation", QString::number(sonar.transmitter_getRotation()));

    for(uint i=0; i < sonar.get_numOfRec(); ++i){
        QDomElement Receiver = xml.createElement("Receiver");

        Receiver.setAttribute("Id", QString::number(sonar.rec_getId(i)));
        Receiver.setAttribute("Size", qVector3DtoQString(sonar.rec_getSize(i)));
        Receiver.setAttribute("Rotation", QString::number(sonar.rec_getRotation(i)));
        Receiver.setAttribute("LocPos", qVector3DtoQString(sonar.rec_getLocPos(i)));

        root.appendChild(Receiver);
    }

    QTextStream output(&file);
    output << xml.toString();
    qDebug() << "koniec";

    return true;
}

bool FileManager::readSonarConf(QFile &file, Sonar &sonar)
{

    QDomDocument xml;

    if(!xml.setContent(&file)){
        qDebug() << " Failed to parse the file into a DOM tree.";
        return false;
    }

    QDomElement root = xml.documentElement();
    if(root.tagName() != "Sonar"){
        qDebug() << "Sonar item not found";
        return false;
    }

    QString sonar_id = root.attribute("Id", "0");
    QString sonar_scale = root.attribute("Size", "0 0 0");
    QString sonar_locpos = root.attribute("LocPos", "0 0 0");
    QString sonar_color = root.attribute("Color", "0 0 0");

    sonar.set_id(sonar_id.toUInt());
    sonar.set_LocPos(qStringToQVector3D(sonar_locpos));
    sonar.set_size(qStringToQVector3D(sonar_scale));
    sonar.set_color(qStringToQVector3D(sonar_color));

    QDomElement node = root.firstChild().toElement();

    if(node.tagName() == "Transmitter"){
        QString transmitter_locpos = node.attribute("LocPos", "0 0 0");
        QString transmitter_rotation = node.attribute("Rotation", "0");
        QString transmitter_size = node.attribute("Size", "0 0 0");

        sonar.transmitter_setLocPos(qStringToQVector3D(transmitter_locpos));
        sonar.transmitter_setSize(qStringToQVector3D(transmitter_size));
        sonar.transmitter_setRotation(transmitter_rotation.toDouble());

    }

    node = node.nextSibling().toElement();
    while(node.isNull() == false){
        if(node.tagName() == "Receiver"){
            QString rec_id = node.attribute("Id", "0");
            QString rec_scale = node.attribute("Size", "0 0 0");
            QString rec_rot = node.attribute("Rotation", "0");
            QString rec_locpos = node.attribute("LocPos", "0 0 0");

            uint rec_id_value = rec_id.toUInt();
            QVector3D rec_scale_value = qStringToQVector3D(rec_scale);
            double rec_rot_value = rec_rot.toDouble();
            QVector3D rec_locpos_value = qStringToQVector3D(rec_locpos);

            sonar.addReceiver(rec_id_value, rec_scale_value, rec_rot_value, rec_locpos_value);
        }
        node = node.nextSibling().toElement();
   }

    return true;
}

bool FileManager::savePlatformConf(QFile &file, Platform const &platform)
{
    QDomDocument xml("platform_configuration");

    QDomElement root = xml.createElement("Platform");

    root.setAttribute("NumOfSonars", QString::number(platform.get_NumOfSonars()));
    root.setAttribute("Size", qVector3DtoQString(platform.get_size()));
    root.setAttribute("LocPos", qVector3DtoQString(platform.get_LocPos()));

    xml.appendChild(root);

    for(uint i=0; i < platform.get_NumOfSonars(); ++i){
        Sonar son = platform.getSonar(i);
        QDomElement sonar = xml.createElement("Sonar");
        sonar.setAttribute("LocPos", qVector3DtoQString(son.get_LocPos()));
        sonar.setAttribute("Color", qVector3DtoQString(son.get_color()));
        sonar.setAttribute("Id", QString::number(son.get_id()));
        sonar.setAttribute("Size", qVector3DtoQString(son.get_size()));
        sonar.setAttribute("Rotation", QString::number(son.get_rotation()));

        root.appendChild(sonar);

        QDomElement transmitter = xml.createElement("Transmitter");
        transmitter.setAttribute("GlPos", qVector3DtoQString(son.transmitter_getGlPos()));
        transmitter.setAttribute("Size", qVector3DtoQString(son.transmitter_getSize()));
        transmitter.setAttribute("Rotation", QString::number(son.transmitter_getRotation()));
        transmitter.setAttribute("LocPos", qVector3DtoQString(son.transmitter_getLocPos()));

        sonar.appendChild(transmitter);

        for(uint i=0; i < son.get_numOfRec(); ++i){
            QDomElement receiver = xml.createElement("Receiver");

            receiver.setAttribute("Id", QString::number(son.rec_getId(i)));
            receiver.setAttribute("Size", qVector3DtoQString(son.rec_getSize(i)));
            receiver.setAttribute("Rotation", QString::number(son.rec_getRotation(i)));
            receiver.setAttribute("GlPos", qVector3DtoQString(son.rec_getGlPos(i)));
            receiver.setAttribute("LocPos", qVector3DtoQString(son.rec_getLocPos(i)));

            sonar.appendChild(receiver);
        }

    }

    QTextStream output(&file);
    output << xml.toString();

    return true;
}

bool FileManager::readPlatformConf(QFile &file, Platform &platform)
{
    QDomDocument xml;

    if(!xml.setContent(&file)){
        qDebug() << " Failed to parse the file into a DOM tree.";
        return false;
    }

    QDomElement root = xml.documentElement();
    if(root.tagName() != "Platform"){
        qDebug() << "Platform item not found";
        return false;
    }

    QString platfrm_size = root.attribute("Size", "0 0 0");
    QString platform_locpos = root.attribute("LocPos", "0 0 0");

    platform.set_size(qStringToQVector3D(platfrm_size));
    platform.set_LocPos(qStringToQVector3D(platform_locpos));


    QDomElement node = root.firstChild().toElement();

    while(!node.isNull()){
        Sonar sonar;
        if(node.tagName() == "Sonar"){
            QString sonar_id = node.attribute("Id", "0");
            QString sonar_scale = node.attribute("Size", "0 0 0");
            QString sonar_locpos = node.attribute("LocPos", "0 0 0");
            QString sonar_color = node.attribute("Color", "0 0 0");
            QString sonar_rotation = node.attribute("Rotation", "0");

            sonar.set_id(sonar_id.toUInt());
            sonar.set_LocPos(qStringToQVector3D(sonar_locpos));
            sonar.set_size(qStringToQVector3D(sonar_scale));
            sonar.set_color(qStringToQVector3D(sonar_color));
            sonar.set_rotation(sonar_rotation.toDouble());
        }

        QDomElement sonarElem = node.firstChild().toElement();
        if(sonarElem.tagName() == "Transmitter"){
            QString transmitter_rotation = sonarElem.attribute("Rotation", "0");
            QString transmitter_glpos = sonarElem.attribute("GlPos", "0 0 0");
            QString transmitter_size = sonarElem.attribute("Size", "0 0 0");
            QString transmitter_locpos = sonarElem.attribute("LocPos", "0 0 0");

            sonar.transmitter_setLocPos(qStringToQVector3D(transmitter_locpos));
            sonar.transmitter_setSize(qStringToQVector3D(transmitter_size));
            sonar.transmitter_setRotation(transmitter_rotation.toDouble());
            sonar.transmitter_setGlPos(qStringToQVector3D(transmitter_glpos));
        }

        sonarElem = sonarElem.nextSibling().toElement();

        while(!sonarElem.isNull()){
            if(sonarElem.tagName() == "Receiver"){
                QString rec_id = sonarElem.attribute("Id", "0");
                QString rec_size = sonarElem.attribute("Size", "0 0 0");
                QString rec_rot = sonarElem.attribute("Rotation", "0");
                QString rec_locpos = sonarElem.attribute("LocPos", "0 0 0");
                QString rec_glpos = sonarElem.attribute("GlPos", "0 0 0");

                uint rec_id_value = rec_id.toUInt();
                QVector3D rec_size_value = qStringToQVector3D(rec_size);
                double rec_rot_value = rec_rot.toDouble();
                QVector3D rec_locpos_value = qStringToQVector3D(rec_locpos);
                QVector3D rec_glpos_value = qStringToQVector3D(rec_glpos);
                sonar.addReceiver(rec_id_value, rec_size_value, rec_rot_value, rec_locpos_value, rec_glpos_value);
            }
            sonarElem = sonarElem.nextSibling().toElement();
        }
        platform.addSonar(sonar);
        node = node.nextSibling().toElement();
   }

    return true;
}

QVector3D FileManager::qStringToQVector3D(QString str)
{
    std::istringstream istrm;
    istrm.str(str.toStdString());

    double x,y,z;
    istrm >> x >> y >> z;

    if(istrm.fail()){
        return QVector3D(0,0,0);
    }

    return QVector3D(x, y, z);
}

QString FileManager::qVector3DtoQString(QVector3D vec)
{
    return QString("%1 %2 %3").arg(vec[0]).arg(vec[1]).arg(vec[2]);
}
