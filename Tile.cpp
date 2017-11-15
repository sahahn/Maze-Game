//
// Created by sage on 10/31/17.
//

#include "Tile.h"
#include "GameInfo.h"
#include <iostream>
#include <math.h>

Tile::Tile() {
    wall = true;
    wasHere = false;
    correctPath = false;
    end = false;
}

bool Tile::getWall() const {
    return wall;
}

bool Tile::getWasHere() const {
    return wasHere;
}

bool Tile::getCorrectPath() const {
    return correctPath;
}

bool Tile::getEnd() const {
    return end;
}

void Tile::setWall(bool b) {
    wall = b;
}

void Tile::setWasHere(bool b) {
    wasHere = b;
}

void Tile::setCorrectPath(bool b) {
    correctPath = b;
}

void Tile::setEnd(bool b) {
    end = b;
}


//Take in coordinates in the way the array is stored, convert it to pixel coordinates
void Tile::draw(int x, int y, int xShift, int yShift, double angle) const {
    
    if (wall) {
        glColor3f(0, 0, 0);
    }

    else {

        if (end) {
            glColor3f(1,.5,.5);

        }
        else {
            glColor3f(1, 0, 1);
        }
    }

    x = (x * SCALE) + yShift;
    y = (y * SCALE) + xShift;


    //Convery to pixel coordinates
    glBegin(GL_QUADS);

    if (angle == 0) {
        // top left corner
        glVertex2i(y, x);
        // top right corner
        glVertex2i((y + SCALE), x);
        // bottom right corner
        glVertex2i((y + SCALE), (x + SCALE));
        // bottom left corner
        glVertex2i(y, (x + SCALE));
        glEnd();
    }
        //If using the rotate func,
    else {

        Point p1,p2,p3,p4;
        p1 = cp(y,x,angle);
        p2 = cp((y+SCALE),x,angle);
        p3 = cp((y+SCALE),(x+SCALE),angle);
        p4 = cp(y,(x+SCALE),angle);

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

int point::getX() const {
    return x;
}
int point::getY() const {
    return y;
}
void point::setX(int X) {
    x = X;
}
void point::setY(int Y) {
    y = Y;
}

bool point::operator ==(const point &b) const {
    if ((x == b.getX()) && (y == b.getY())) {
        return true;
    }
    else {
        return false;
    }
}
