//
// Created by sage on 11/30/17.
//

#include "MapEditor.h"

MapEditor::MapEditor() {
    //Loads in center of the map
    loc.x = HEIGHT / 2;
    loc.y = WIDTH / 2;

    //Xshift and yShift are just to control the camera in map editor
    xShift = 0;
    yShift = 0;

    sBoundary = (EDITOR_SCALE/2);

    //How fast the camera moves
    moveSpeed = 5;

    //Holds mouse position in the map array
    mX = 0;
    mY = 0;

    fillVal = false;
    clickTimer = false;
    ticks = 0;

}