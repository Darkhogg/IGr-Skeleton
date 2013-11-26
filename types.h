/**
 * @author Daniel Escoz Solana
 */
#ifndef __TYPES__H__
#define __TYPES__H__

#include <GL/glx.h>

struct Quad {
    GLdouble left;
    GLdouble right;
    GLdouble bottom;
    GLdouble top;

    Quad () : left(0), right(0), bottom(0), top(0) {}
    Quad (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
        : left(left), right(right), bottom(bottom), top(top) {}
    ~Quad () {} 
};

#endif