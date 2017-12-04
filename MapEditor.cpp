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

int MapEditor::getLevel() const {
    return level;
}

void MapEditor::setLevel(int l) {
    level = l;
}

void MapEditor::update() {

    if (yShift < (-sBoundary)) {
        loc.x +=1;
        yShift += EDITOR_SCALE;
    }

    if (xShift > (sBoundary)) {
        loc.y -= 1;
        xShift -= EDITOR_SCALE;
    }

    if (xShift < (-sBoundary)) {
        loc.y += 1;
        xShift += EDITOR_SCALE;
    }

    if (yShift > (sBoundary)) {
        loc.x -= 1;
        yShift -= EDITOR_SCALE;
    }
}