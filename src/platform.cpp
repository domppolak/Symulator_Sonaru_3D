#include "platform.hh"
#include <assert.h>
#include <QDebug>

Platform::Platform()
{
    _loc_pos_mm = QVector3D(0,0,0);
    _gl_pos_mm = QVector3D(0,0,0);

    _size_mm = QVector3D(10,10,10);
    _color = QVector3D(0,0,0);
    _rotation_deg = 0;
}

void Platform::addSonar(Sonar &sonar)
{
    _sonarList.push_back(Sonar());
    _sonarList.back().set_id(sonar.get_id());
    _sonarList.back().set_LocPos(sonar.get_LocPos());
    _sonarList.back().set_GlPos(sonar.get_GlPos());
    _sonarList.back().set_color(sonar.get_color());
    _sonarList.back().set_rotation(sonar.get_rotation());
    _sonarList.back().set_size(sonar.get_size());

    _sonarList.back().transmitter_setSize(sonar.transmitter_getSize());
    _sonarList.back().transmitter_setLocPos(sonar.transmitter_getLocPos());
    _sonarList.back().transmitter_setGlPos(sonar.transmitter_getGlPos());

    for(uint i =0; i < sonar.get_numOfRec(); ++i){
        _sonarList.back().addReceiver(sonar.rec_getId(i), sonar.rec_getSize(i),
                                       sonar.rec_getRotation(i), sonar.rec_getLocPos(i),
                                       sonar.rec_getGlPos(i));
    }
}

bool Platform::deleteSonar()
{
    if(_sonarList.empty())
        return false;

    _sonarList.pop_back();
    return true;
}

bool Platform::deleteSonar(uint id)
{
    assert(id < _sonarList.size());

    if(_sonarList.empty())
        return false;

    _sonarList.erase(_sonarList.begin() + id);
    return true;
}

void Platform::deleteAllSonar()
{
    while(!_sonarList.empty())
        deleteSonar();
}

void Platform::resetPlatform()
{
    deleteAllSonar();
    _loc_pos_mm = QVector3D(0,0,0);
    _gl_pos_mm = QVector3D(0,0,0);

    _size_mm = QVector3D(10,10,10);
    _color = QVector3D(0,0,0);
    _rotation_deg = 0;
}

