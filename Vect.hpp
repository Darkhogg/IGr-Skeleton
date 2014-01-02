/**
 * @author Daniel Escoz Solana
 */
#ifndef __VECT__H__
#define __VECT__H__

#include <SDL.h>
#include <GL/glx.h>

#include <iostream>

class Vect {
    public: 
        typedef GLdouble Component;

    private:
        Vect::Component _x;
        Vect::Component _y;
        Vect::Component _z;
        Vect::Component _w;

    public:
        Vect () : _x(0), _y(0), _z(0), _w(0) {}
        Vect (const Vect& v) : _x(v._x), _y(v._y), _z(v._z), _w(v._w) {}
        Vect (Vect::Component x, Vect::Component y, Vect::Component w) : _x(x), _y(y), _z(0), _w(w) {}
        Vect (Vect::Component x, Vect::Component y, Vect::Component z, Vect::Component w) : _x(x), _y(y), _z(z), _w(w) {}
        ~Vect () {}

        Vect::Component x () const {return _x;}
        Vect::Component y () const {return _y;}
        Vect::Component z () const {return _z;}
        Vect::Component w () const {return _w;}

        void x (Vect::Component x) {_x = x;}
        void y (Vect::Component y) {_y = y;}
        void z (Vect::Component z) {_z = z;}
        void w (Vect::Component w) {_w = w;}

        Vect& operator+= (const Vect& other);
        Vect& operator-= (const Vect& other);
        Vect& operator*= (const Vect::Component& scale);
        Vect& operator/= (const Vect::Component& scale);
        Vect operator- () { return Vect(-_x, -_y, -_z, _w); }

        Vect::Component mod_sqr () const;
        Vect::Component mod () const;

        Vect::Component pitch () const;
        Vect::Component yaw () const;

        Vect::Component dot (const Vect& other) const {
            return _x*other._x + _y*other._y + _z*other._z;
        }

        Vect normalized () const;

        Vect project (const Vect& other);

        // Only for 2D vectors!
        Vect perp_left_z0 () const;
        Vect perp_right_z0 () const;
};

inline Vect operator+ (Vect v1, Vect v2) {
    v1 += v2;
    return v1;
}

inline Vect operator- (Vect v1, Vect v2) {
    v1 -= v2;
    return v1;
}

inline Vect operator* (Vect v, Vect::Component k) {
    v *= k;
    return v;
}

inline Vect operator/ (Vect v, Vect::Component k) {
    v /= k;
    return v;
}


inline std::ostream& operator<<(std::ostream& os, const Vect& v) {
    os << "(" << v.x() << ", " << v.y() << ", " << v.z() << " | " << v.w() << ")";
    return os;
}

#endif