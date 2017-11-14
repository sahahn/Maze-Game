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

    bool getWasHere() const;

    bool getCorrectPath() const;

    bool getEnd() const;

    void setWall(bool b);

    void setWasHere(bool b);

    void setCorrectPath(bool b);

    void setEnd(bool b);

    void draw(int x, int y, int xShift, int yShift, double angle) const;

private:
    bool wall;
    bool wasHere;
    bool correctPath;
    bool end;

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
