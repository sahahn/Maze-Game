//
// Created by sage on 11/10/17.
//

#ifndef GRAPHICS_CHARACTER_H
#define GRAPHICS_CHARACTER_H

#include "graphics.h"
#include "GameInfo.h"

class Character {

        //sBoundary = (scale/2);

public:

    //Location in the array
    int x;
    int y;
    int x_shift;
    int y_shift;

    int get_size() const;

    int get_speed() const;

    void update();

    virtual void draw(int X, int Y) const = 0; //Make the class pure virtual

protected:

    Point location; //Location on screen
    int size;
    int speed;
    static const int sBoundary = (GameInfo::scale / 2);
};


class Player : public Character {
public:

    Player();
    Player(int X, int Y);
    void draw(int X, int Y) const override;
};



class Enemy : public Character {
public:

    Enemy();
    Enemy(int X, int Y);
    void draw(int X, int Y) const override;
};


#endif //GRAPHICS_CHARACTER_H
