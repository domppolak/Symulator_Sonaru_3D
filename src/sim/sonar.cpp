#include "sonar.hh"
#include <QDebug>
#include <QVector3D>
#include <algorithm>
#include <iterator>

Sonar::Sonar()
{
    _id = 0;
    _locPos_mm = QVector3D(0,0,0);
    _size_mm = QVector3D(1,1,1);
    _rotation_deg = 0;
    _color = QVector3D(0,0,0);

}

Sonar::~Sonar()
{

}

uint Sonar::addReceiver()
{
    _receiverList.push_back(Receiver());
    _receiverList.back().set_id(_receiverList.size());

    return _receiverList.back().get_id();
}

void Sonar::addReceiver(uint id, QVector3D size_mm, double rot_deg, QVector3D locPos_mm)
{
    _receiverList.push_back(Receiver());
    _receiverList.back().set_id(id);
    _receiverList.back().set_size(size_mm);
    _receiverList.back().set_rotation(rot_deg);
    _receiverList.back().set_LocPos(locPos_mm);
}

void Sonar::addReceiver(uint id, QVector3D size_mm, double rot_deg, QVector3D locPos_mm, QVector3D glPos_mm)
{
    _receiverList.push_back(Receiver());
    _receiverList.back().set_id(id);
    _receiverList.back().set_size(size_mm);
    _receiverList.back().set_rotation(rot_deg);
    _receiverList.back().set_LocPos(locPos_mm);
    _receiverList.back().set_GlPos(glPos_mm);
}

bool Sonar::deleteReceiver()
{
    if(_receiverList.empty())
        return false;

    _receiverList.pop_back();
    return true;
}

bool Sonar::deleteReceiver(const uint &id)
{
    assert(id < _receiverList.size());

    if(_receiverList.empty())
        return false;

    _receiverList.erase(_receiverList.begin() +id);
    return true;
}

void Sonar::deleteAllRec()
{
    while(!_receiverList.empty())
        deleteReceiver();
}

void Sonar::resetSonar()
{
    _id = 0;
    _locPos_mm = QVector3D(0,0,0);
    _glPos_mm = QVector3D(0,0,0);
    _size_mm = QVector3D(10,10,10);
    _rotation_deg = 0;
    _color = QVector3D(0,0,0);

    _receiverList.clear();
}


