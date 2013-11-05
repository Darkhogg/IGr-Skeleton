#include "Vect.hpp"

#include <cmath>

Vect& Vect::operator+= (Vect other) {
    _x += other._x;
    _y += other._y;
    _z += other._z;
    return *this;
}

Vect& Vect::operator-= (Vect other) {
    _x -= other._x;
    _y -= other._y;
    _z -= other._z;
    return *this;
}

Vect& Vect::operator*= (Vect::Component scale) {
    _x *= scale;
    _y *= scale;
    _z *= scale;
    return *this;
}

Vect& Vect::operator/= (Vect::Component scale) {
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
    // TODO
}

Vect::Component Vect::yaw () const {
    // TODO
}