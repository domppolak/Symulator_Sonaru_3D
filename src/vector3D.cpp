#include "vector3D.hh"

namespace geom
{

Vector3D Vector3D::operator + (Vector3D const &arg2) const
{
    Vector3D vec_res(*this);
    return vec_res += arg2;
}

Vector3D &Vector3D::operator += (Vector3D const &arg2)
{
    for(uint i=0; i < 3; i++ ) _Tab[i] += arg2[i];
    return *this;
}

Vector3D Vector3D::operator - (Vector3D const &arg2) const
{
    Vector3D vec_res(*this);
    return vec_res -= arg2;
}

Vector3D &Vector3D::operator -= (Vector3D const &arg2)
{

    for(uint i=0; i < 3; i++ ) _Tab[i] -= arg2[i];
    return *this;
}

Vector3D Vector3D::operator * (double digit) const
{
    Vector3D vec_res(*this);
    return vec_res *= digit;
}

Vector3D &Vector3D::operator *= (double digit)
{
    for(double &rTab : _Tab) rTab *= digit;
    return *this;
}

Vector3D Vector3D::operator / (double digit) const
{
    Vector3D vec_res(*this);
    return vec_res /= digit;
}

Vector3D &Vector3D::operator /= (double digit)
{
    assert(digit != 0);
    for(double &rTab : _Tab) rTab /= digit;
    return *this;
}

double Vector3D::operator & (Vector3D const &arg2) const
{
    double result(0.0);
    for(uint i = 0; i < 3; i++) result += _Tab[i] * arg2[i];
    return result;
}

bool Vector3D::operator ==(const Vector3D &arg2) const
{
    for(uint i = 0; i < 3; i++)
        if (_Tab[i] != arg2[i])
            return false;
    return true;
}

bool Vector3D::operator !=(const Vector3D &arg2) const
{
    if(*this == arg2)
        return false;

    return true;
}

double Vector3D::length() const
{
    return ((*this) & (*this));
}

Vector3D Vector3D::operator * (Vector3D const &arg2) const
{
    Vector3D vec_res(*this);
    return vec_res *= arg2;
}

Vector3D &Vector3D::operator *= (Vector3D const &arg2)
{
    Vector3D vec(*this);
    _Tab[0] = vec[1]*arg2[2] - vec[2]*arg2[1];
    _Tab[1] = -1 * (vec[0]*arg2[2] - vec[2]*arg2[0]);
    _Tab[2] = vec[0]*arg2[1] - vec[1]*arg2[0];

    return *this;
}
}
