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

    virtual void draw(int X, int Y, int pXShift, int pYShift, double angle) const = 0; //Make the class pure virtual

protected:

    Point location; //Location on screen
    int size;
    int speed;
    static const int sBoundary = (SCALE / 2);
};


class Player : public Character {
public:

    Player();
    Player(int X, int Y);
    void draw(int X, int Y, int pXShift, int pYShift, double angle) const override;
};



class Enemy : public Character {
public:

    Enemy();
    Enemy(int X, int Y);
    void draw(int X, int Y, int pXShift, int pYShift, double angle) const override;
};


#endif //GRAPHICS_CHARACTER_H
