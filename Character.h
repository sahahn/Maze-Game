//
// Created by sage on 11/10/17.
//

#ifndef GRAPHICS_CHARACTER_H
#define GRAPHICS_CHARACTER_H

#include "graphics.h"
#include "GameInfo.h"

class Character {


public:

    //Location in the maze array
    int x;
    int y;

    //Shifted position within the location
    int xShift;
    int yShift;

    //Temp values for intended movement, i.e. only
    //updated to xShift, yShift if a valid movement.
    int temp1;
    int temp2;

    //The "Hard Boundary" specific to each character, used in collision detection.
    int hBoundary;

    int getSize() const;
    int getSpeed() const;
    //Point getLocation() const;  Not needed!

    void setLocation(int xL, int yL);

    void update();

protected:

    int size;
    int speed;
    static const int sBoundary = (SCALE / 2);
    Point location; //Location on screen

};


class Player : public Character {
public:

    Player();
    void draw() const;
    void calcMove(int xDelta, int yDelta, double angleR);
};



class Enemy : public Character {
public:

    Enemy();
    Enemy(int X, int Y, int s, int sp);

    void draw(int pXShift, int pYShift, double angleR) const;
    void calcMove(int x, int y);
};


#endif //GRAPHICS_CHARACTER_H
