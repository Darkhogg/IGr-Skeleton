#include "Turtle.hpp"

#include <math.h>

void Turtle::advance (GLdouble dist) {
    x += dist * cos(ang);
    y += dist * sin(ang);
}

void Turtle::rotate (GLdouble angle) {
    ang += angle;
    // TODO clamp betweet 0 and 2*PI
}

void Turtle::lookAt (GLdouble atX, GLdouble atY) {
    ang = atan2(atY - y, atX - x);
}

void Turtle::moveTo (GLdouble x, GLdouble y) {
    lookAt(x, y);
    pos(x, y);
}