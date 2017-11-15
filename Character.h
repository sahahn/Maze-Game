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

    int temp1;
    int temp2;

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
    void calcMove(int x, int y, double angleR);


};



class Enemy : public Character {
public:

    Enemy();
    Enemy(int X, int Y);
    Enemy(int X, int Y, int s, int sp);
    void draw(int pXShift, int pYShift, double angle) const;

    void calcMove(int x, int y);

    void moveL();
    void moveR();
    void moveU();
    void moveD();

};


#endif //GRAPHICS_CHARACTER_H
