#include "Turtle.hpp"

void Turtle::advance (GLdouble dist) {
    pos(x() + dist * cos(ang()), y() + dist * sin(ang()));
}

void Turtle::rotate (GLdouble angle) {
    ang(ang() + angle);
}

void Turtle::lookAt (GLdouble atX, GLdouble atY) {
    GLdouble angle = atan2(atY - y(), atX - x());
    ang(angle);
}

void Turtle::moveTo (GLdouble x, GLdouble y) {
    lookAt(x, y);
    pos(x, y);
}