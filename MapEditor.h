//
// Created by sage on 11/30/17.
//

#ifndef GRAPHICS_EDITOR_H
#define GRAPHICS_EDITOR_H
#include "GameInfo.h"

class MapEditor {
public:
    MapEditor();

    Point loc;
    int xShift, yShift;

    int moveSpeed;

    int sBoundary;

    int mX, mY;

    bool fillVal;
    bool clickTimer;
    int ticks;

    int level;

private:




};


#endif //GRAPHICS_EDITOR_H
