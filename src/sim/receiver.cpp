#include "receiver.hh"
#include <QDebug>


Receiver::Receiver()
{
    resetReceiver();
}

void Receiver::resetReceiver()
{
    _id =0;
    _glPos_mm = QVector3D(0,0,0);
    _locPos_mm = QVector3D(0,0,0);
    _size_mm = QVector3D(10,10,10);
    _rotation_deg = 0;
}

