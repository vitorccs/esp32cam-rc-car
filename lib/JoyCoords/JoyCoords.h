#ifndef JOYCOORDS_H
#define JOYCOORDS_H

#include <iostream>

struct JoyCoords
{
    // the cardial point (C, N, NE, E, SE, S, SW, W, NW)
    String direction;

    // the motion speed (from 0 to 100)
    int16_t speed;
};
#endif
