//
// Created by sage on 10/31/17.
//

#include "Tile.h"
#include "GameInfo.h"
#include <iostream>
#include <math.h>


/*
 * Takes in an x and y, and an angle in radians and returns the location of the point rotated around the center
 */
Point cp(int x, int y, double Angle) {

    x-= (GameInfo::screen_width / 2);
    y-= (GameInfo::screen_height / 2);

    double xP = ((x * cos(Angle)) - (y * sin(Angle)));
    double yP = ((y * cos(Angle)) + (x * sin(Angle)));

    xP += (GameInfo::screen_width / 2);
    yP += (GameInfo::screen_height / 2);

    Point newPoint;
    newPoint.x = rint (xP);
    newPoint.y = rint (yP);

    return newPoint;
}


Tile::Tile() {
    wall = true;
}

bool Tile::get_wall() const {
    return wall;
}

void Tile::set_wall(bool b) {
    wall = b;
}

//Take in coordinates in the way the array is stored, convert it to pixel coordinates
void Tile::draw(int x, int y, int x_shift, int y_shift, double angle) const {

    int const scale = GameInfo::scale; // The size of each array piece, e.g. 100x100 if scale = 100

    if (wall) {
        glColor3f(0, 0, 0);
    }

    else {
        glColor3f(1, 0, 1);
    }

    x = (x * scale) + y_shift;
    y = (y * scale) + x_shift;


    //Convery to pixel coordinates
    glBegin(GL_QUADS);

    if (angle == 0) {
        // top left corner
        glVertex2i(y, x);
        // top right corner
        glVertex2i((y + scale), x);
        // bottom right corner
        glVertex2i((y + scale), (x + scale));
        // bottom left corner
        glVertex2i(y, (x + scale));
        glEnd();
    }
        //If using the rotate func,
    else {

        Point p1,p2,p3,p4;
        p1 = cp(y,x,angle);
        p2 = cp((y+scale),x,angle);
        p3 = cp((y+scale),(x+scale),angle);
        p4 = cp(y,(x+scale),angle);

        glVertex2i(p1.x,p1.y);
        glVertex2i(p2.x,p2.y);
        glVertex2i(p3.x,p3.y);
        glVertex2i(p4.x,p4.y);
    }
}

point::point() {
    x = 0;
    y = 0;
}

point::point(int X, int Y) {
    x = X;
    y = Y;
}

int point::get_x() const {
    return x;
}
int point::get_y() const {
    return y;
}
void point::set_x(int X) {
    x = X;
}
void point::set_y(int Y) {
    y = Y;
}

bool point::operator ==(const point &b) const {
    if ((x == b.get_x()) && (y == b.get_y())) {
        return true;
    }
    else {
        return false;
    }
}
