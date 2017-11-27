//
// Created by Luke on 11/22/2017.
//

#ifndef PROJECT6_CS120_LIGHT_H
#define PROJECT6_CS120_LIGHT_H

#include "graphics.h"
#include "Maze.h"

class Light {
public:
    void renderLight(int pX, int pY, int pXShift, int pYShift, Maze &m) const;
    Light();
};
#endif //PROJECT6_CS120_LIGHT_H
