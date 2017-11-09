//
// Created by sage on 10/31/17.
//

#ifndef GRAPHICS_TILE_H
#define GRAPHICS_TILE_H
#include "graphics.h"
#include <math.h>

class Tile {
public:
    Tile();

    bool getWall() const;

    void setWall(bool b);

    void draw(int x, int y, int x_shift, int y_shift, double angle) const;

private:
    bool wall;

};


/*
 * point class is used in generating the maze class
 */
class point {
public:
    point();

    point(int X, int Y);

    int getX() const;
    int getY() const;
    void setX(int X);
    void setY(int Y);

    bool operator ==(const point &b) const;

private:
    int x;
    int y;

};


#endif //GRAPHICS_TILE_H
