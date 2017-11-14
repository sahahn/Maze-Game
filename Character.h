//
// Created by sage on 11/10/17.
//

#ifndef GRAPHICS_CHARACTER_H
#define GRAPHICS_CHARACTER_H

#include "graphics.h"
#include "GameInfo.h"

class Character {


public:

    //Location in the array
    int x;
    int y;
    int xShift;
    int yShift;

    int getSize() const;

    int getSpeed() const;

    void update();

protected:

    int size;
    int speed;
    static const int sBoundary = (SCALE / 2);

};


class Player : public Character {
public:

    Player();
    void draw() const;
    Point location; //Location on screen

};



class Enemy : public Character {
public:

    Enemy();
    Enemy(int X, int Y);
    Enemy(int X, int Y, int s, int sp);
    void draw(int X, int Y, int pXShift, int pYShift, double angle) const;

};


#endif //GRAPHICS_CHARACTER_H
