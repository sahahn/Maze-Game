//
// Created by sage on 10/31/17.
//

#ifndef GRAPHICS_TILE_H
#define GRAPHICS_TILE_H

#include "graphics.h"
#include <math.h>

class Tile {
public:
    // constructors
    Tile();

    // getters
    bool getWall() const;

    bool getWasHere() const;

    bool getCorrectPath() const;

    bool getEnd() const;

    // setters
    void setWall(bool b);

    void setWasHere(bool b);

    void setCorrectPath(bool b);

    void setEnd(bool b);

    // renders a single tile of the maze
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
class MazePoint {
public:
    // constructors
    MazePoint();

    MazePoint(int X, int Y);

    // getters and setters
    int getX() const;

    int getY() const;

    void setX(int X);

    void setY(int Y);

    // overrides the == operator to work for points in the maze for easy comparison
    bool operator==(const MazePoint &b) const;

private:
    int x;
    int y;

};


#endif //GRAPHICS_TILE_H
