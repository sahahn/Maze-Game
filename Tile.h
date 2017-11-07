//
// Created by sage on 10/31/17.
//

#ifndef GRAPHICS_TILE_H
#define GRAPHICS_TILE_H
#include "graphics.h"

class Tile {
public:
    Tile();

    bool get_wall() const;

    void set_wall(bool b);

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

    int get_x() const;
    int get_y() const;
    void set_x(int X);
    void set_y(int Y);

    bool operator ==(const point &b) const;

private:
    int x;
    int y;

};


#endif //GRAPHICS_TILE_H
