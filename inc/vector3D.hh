#ifndef vector_HH
#define vector_HH

#include <cassert>
#include <iostream>
#include <cmath>


namespace geom
{

class Vector3D
{
public:
    Vector3D() {for (double &arg2 : _Tab) arg2 = 0;};
    double &operator [](uint ind) { assert(ind < 3); return _Tab[ind]; }
    const double &operator [](uint ind) const { assert(ind < 3);  return _Tab[ind]; }
    Vector3D operator + (Vector3D const &arg2) const;
    Vector3D &operator += (Vector3D const &arg2);
    Vector3D operator - (Vector3D const &arg2) const;
    Vector3D &operator -= (Vector3D const &arg2);
    Vector3D operator * (Vector3D const &arg2) const;
    Vector3D &operator *= (Vector3D const &arg2);
    Vector3D operator * (double digit) const;
    Vector3D &operator *= (double digit);
    Vector3D operator / (double digit) const;
    Vector3D &operator /= (double digit);
    double operator & (Vector3D const &arg2) const; //scalar
    bool operator == (Vector3D const &arg2) const;
    bool operator != (Vector3D const &arg2) const;
    double length() const;

private:
    double _Tab[3];

};

}

#endif // vector_HH
