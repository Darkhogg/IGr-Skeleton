/**
 * @author Daniel Escoz Solana
 */
#include "Camera2D.hpp"

#include <cmath>

Quad Camera2D::view (int width, int height) {
//    double width = _vp.right - _vp.left;
//    double height = _vp.top - _vp.bottom;
    double zmult = 0.5 / exp(_zoom);

    return Quad(
        -width * zmult + _pos.x(),
        width * zmult + _pos.x(),
        -height * zmult + _pos.y(),
        height * zmult + _pos.y()
    );
}