//
// Created by sage on 10/31/17.
//

#ifndef GRAPHICS_TILE_H
#define GRAPHICS_TILE_H

#include "graphics.h"
#include <math.h>
#include "Box.h"


//The fake plural of status, the fake singular being stati
enum Statee {
    None = 1, Start = 2, End = 3, FlipperSpawn = 4, SizerSpawn = 5
};

class Tile {
public:
    // constructors
    Tile();

   // getters
    bool getWall() const;

    Statee getStati() const;

    // setters
    void setWall(bool b);

    void setStati(Statee s);

    //Used in editor
    void flipWall();

    // renders a single tile of the maze
    void draw(int x, int y, int xShift, int yShift, double angle) const;

    //Slightly dif from maze render draw, no need for angle, USED IN EDITOR
    void draw(int x, int y, int xShift, int yShift) const;

    //Draw used to generate map previews
    void draw(int x, int y, int scale, bool hover) const;

private:
    bool wall;
    Statee stati;

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
