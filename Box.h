//
// Created by sage on 11/30/17.
//

#ifndef GRAPHICS_BOX_H
#define GRAPHICS_BOX_H

#include "graphics.h"
#include "GameInfo.h"


class Box {
public:
    Box();
    Box(int X, int Y, int s, int R, int G, int B);
    int x, y, size;
    double r,g,b;

    bool hover;

    bool checkHover(int X, int Y);

    void draw() const;

    void randomMenuMovement();



};


#endif //GRAPHICS_BOX_H
