#ifndef __TURTLE__H__
#define __TURTLE__H__

/* SDL and OpenGL */
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>

#include <math.h>

class Turtle {
    private:
        GLdouble _x, _y;
        GLdouble _ang;

    public:
        Turtle () : _x(0.0), _y(0.0), _ang(0.0) {}
        ~Turtle () {}

        // ===============
        // === GETTERS ===
        
        GLdouble x () { return _x; }
        GLdouble y () { return _y; }
        GLdouble ang () { return _ang; }

        // ===============
        // === SETTERS ===

        void x (GLdouble x) { _x = x; }
        void y (GLdouble y) { _y = y; }
        void pos (GLdouble nx, GLdouble ny) { x(nx); y(ny); }
        void ang (GLdouble angle) { _ang = fmod(angle, 2*M_PI); }

        // ==============
        // === MOVING ===
        
        void advance (GLdouble distance);
        void rotate (GLdouble angle);

        void lookAt (GLdouble x, GLdouble y);
        void moveTo (GLdouble x, GLdouble y);
};

#endif