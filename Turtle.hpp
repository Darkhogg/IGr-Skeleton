#ifndef __TURTLE__H__
#define __TURTLE__H__

class Turtle {
    private:
        GLdouble x, y;
        GLdouble ang;

    public:
        Turtle () : x(0.0), y(0.0), ang(0.0) {}
        ~Turtle () {}

        // ===============
        // === GETTERS ===
        
        GLdouble x () { return x; }
        GLdouble y () { return y; }
        GLdouble ang () { return ang; }

        // ===============
        // === SETTERS ===

        void x (GLdouble x) { this.x = x; }
        void y (GLdouble y) { this.y = y; }
        void pos (GLdouble x, GLdouble y) { x(x); y(y); }
        void ang (GLdouble angle) { this.ang = ang; }

        // ==============
        // === MOVING ===
        
        void advance (GLdouble distance);
        void rotate (GLdouble angle);

        void lookAt (GLdouble x, GLdouble y);
        void moveTo (GLdouble x, GLdouble y);
};

#endif