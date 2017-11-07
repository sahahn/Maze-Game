//
// Created by sage on 11/6/17.
//

#ifndef GRAPHICS_PLAYER_H
#define GRAPHICS_PLAYER_H
#include "graphics.h"
#include "GameInfo.h"


class Player {
public:

    Player();

    //Location in the array
    int x;
    int y;
    void draw() const;

private:
    Point location; //Location on screen
    int size;



};


#endif //GRAPHICS_PLAYER_H
