#include "Vect.hpp"

#include <cmath>

Vect& Vect::operator+= (const Vect& other) {
    _x += other._x;
    _y += other._y;
    _z += other._z;
    return *this;
}

Vect& Vect::operator-= (const Vect& other) {
    _x -= other._x;
    _y -= other._y;
    _z -= other._z;
    return *this;
}

Vect& Vect::operator*= (const Vect::Component& scale) {
    _x *= scale;
    _y *= scale;
    _z *= scale;
    return *this;
}

Vect& Vect::operator/= (const Vect::Component& scale) {
    _x /= scale;
    _y /= scale;
    _z /= scale;
    return *this;
}

Vect::Component Vect::mod_sqr () const {
    return _x*_x + _y*_y + _z*_z;
}

Vect::Component Vect::mod () const {
    return sqrt(mod_sqr());
}

Vect::Component Vect::pitch () const {
    return atan2(sqrt(_x*_x + _y*_y), _z);
}

Vect::Component Vect::yaw () const {
    return atan2(_x, -_y);
}

Vect Vect::normalized () const {
    auto m = mod();
    return Vect(_x/m, _y/m, _z/m);
}

Vect Vect::perp_left_z0 () const {
    return Vect(-_y, _x);
}

Vect Vect::perp_right_z0 () const {
    return Vect(_y, -_x);
}

Vect Vect::project (const Vect& v) {
    return normalized() * dot(v) / mod();
}