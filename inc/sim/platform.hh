#ifndef PLATFORM_HH
#define PLATFORM_HH

#include <QObject>
#include <QVector>
#include <memory>
#include "sonar.hh"

class Platform
{
public:
    Platform();
    void set_LocPos(QVector3D const &locPos_mm);
    void set_LocPos(double x_mm, double y_mm, double z_mm);
    void set_GlPos(QVector3D const &glPos_mm);
    void set_GlPos(double x_mm, double y_mm, double z_mm);
    void set_size(QVector3D const &size);
    void set_size(double x_mm, double y_mm, double z_mm);
    void set_color(QVector3D const &color);
    void set_color(double red, double green, double blue);
    void set_rotation(double rot);

    QVector3D get_LocPos() const;
    QVector3D get_GlPos() const;
    QVector3D get_size() const;
    QVector3D get_color() const;
    double get_rotation() const;
    uint get_NumOfSonars() const;

    Sonar const &getSonar(uint const &id) const;
    Sonar &useSonar(uint const &id);

    void sonar_setLocPos(uint const &id, QVector3D const &LocPos_mm);
    void sonar_setSize(uint const &id, QVector3D const &size_mm);
    void sonar_setId(uint const &id, uint const &newId);
    void sonar_setColor(uint const &id, QVector3D const &color);
    void sonar_setRotation(uint const &id, double const &rot_deg);

    QVector3D sonar_getLocPos(uint const &id) const;
    QVector3D sonar_getSize(uint const &id) const;
    uint sonar_getId(uint const &id) const;
    QVector3D sonar_getColor(uint const &id) const;
    double sonar_getRotation(uint const &id) const;

    void addSonar(Sonar &sonar);
    void addSonar();

    bool deleteSonar();
    bool deleteSonar(uint id);
    void deleteAllSonar();

    void resetPlatform();

private:
    QVector3D _loc_pos_mm;
    QVector3D _gl_pos_mm;

    std::vector<Sonar> _sonarList;

    QVector3D _size_mm;
    QVector3D _color;
    double _rotation_deg;
};

inline void Platform::set_LocPos(QVector3D const &locPos_mm) {this->_loc_pos_mm = locPos_mm;}
inline void Platform::set_LocPos(double x_mm, double y_mm, double z_mm) {set_LocPos(QVector3D(x_mm, y_mm, z_mm));}
inline void Platform::set_GlPos(QVector3D const &glPos_mm) {this->_gl_pos_mm = glPos_mm;}
inline void Platform::set_GlPos(double x_mm, double y_mm, double z_mm) {set_GlPos(QVector3D(x_mm, y_mm, z_mm));}
inline void Platform::set_size(QVector3D const &size_mm) {this->_size_mm = size_mm;}
inline void Platform::set_size(double x_mm, double y_mm, double z_mm) {set_size(QVector3D(x_mm, y_mm, z_mm));}
inline void Platform::set_color(QVector3D const &color) {this->_color = color;}
inline void Platform::set_color(double red, double green, double blue) {set_color(QVector3D(red, blue, green));}

inline void Platform::set_rotation(double rot_deg) {this->_rotation_deg = rot_deg;}
inline QVector3D Platform::get_LocPos() const {return this->_loc_pos_mm;}
inline QVector3D Platform::get_GlPos() const {return this->_gl_pos_mm;}
inline QVector3D Platform::get_size() const {return this->_size_mm;}
inline QVector3D Platform::get_color() const {return this->_color;}
inline double Platform::get_rotation() const {return this->_rotation_deg;}
inline uint Platform::get_NumOfSonars() const {return _sonarList.size();}

inline const Sonar &Platform::getSonar(const uint &id) const
{
    assert(id < _sonarList.size());
    return _sonarList[id];
}

inline Sonar &Platform::useSonar(const uint &id)
{
    assert(id < _sonarList.size());
    return _sonarList[id];
}

inline void Platform::sonar_setLocPos(uint const &id, QVector3D const &LocPos)
{
    assert(id < _sonarList.size());
    _sonarList[id].set_LocPos(LocPos);
}

inline void Platform::sonar_setSize(uint const &id, QVector3D const &size)
{
    assert(id < _sonarList.size());
    _sonarList[id].set_size(size);
}

inline void Platform::sonar_setId(uint const &id, uint const &newId)
{
    assert(id < _sonarList.size());
    _sonarList[id].set_id(newId);
}

inline void Platform::sonar_setColor(uint const &id, QVector3D const &color)
{
    assert(id < _sonarList.size());
    _sonarList[id].set_color(color);
}

inline void Platform::sonar_setRotation(uint const &id, double const &rot)
{
    assert(id < _sonarList.size());
    _sonarList[id].set_rotation(rot);
}

inline QVector3D Platform::sonar_getLocPos(uint const &id) const
{
    assert(id < _sonarList.size());
    return _sonarList[id].get_LocPos();
}

inline QVector3D Platform::sonar_getSize(uint const &id) const
{
    assert(id < _sonarList.size());
    return _sonarList[id].get_size();
}

inline uint Platform::sonar_getId(uint const &id) const
{
    assert(id < _sonarList.size());
    return _sonarList[id].get_id();
}

inline QVector3D Platform::sonar_getColor(uint const &id) const
{
    assert(id < _sonarList.size());
    return _sonarList[id].get_color();
}

inline double Platform::sonar_getRotation(uint const &id) const
{
    assert(id < _sonarList.size());
    return _sonarList[id].get_rotation();
}


#endif // PLATFORM_HH
