/**
 * @author Daniel Escoz Solana
 */
#ifndef __CAMERA_2D__H__
#define __CAMERA_2D__H__

#include "types.h"
#include "Vect.hpp"

class Camera2D {
    private:
        Vect _pos;
        GLdouble _zoom;
        Quad _vp;

    public:
        Camera2D () : _pos(0, 0), _zoom(0), _vp(0, 0, 0, 0) {}
        ~Camera2D () {}

        Vect pos () {return _pos;}
        GLdouble zoom () {return _zoom;}
        Quad vp () {return _vp;}

        void pos (Vect pos) {_pos = pos;}
        void vp (Quad vp) {_vp = vp;}
        void zoom (GLdouble zoom) {_zoom = zoom;}

        Quad view (int w, int h);
};

#endif