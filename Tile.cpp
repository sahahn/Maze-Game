//
// Created by sage on 10/31/17.
//

#include "Tile.h"
#include "GameInfo.h"
#include <iostream>
Tile::Tile() {
    wall = true;
    stati = None;
}

bool Tile::getWall() const {
    return wall;
}

Statee Tile::getStati() const {
    return stati;
}


void Tile::setWall(bool b) {
    wall = b;

    //If wall, make sure no other status
    if (wall) {
        stati = None;
    }
}

void Tile::setStati(Statee s) {
    stati = s;
    wall = false;
}


//Take in coordinates in the way the array is stored, convert it to pixel coordinates
void Tile::draw(int x, int y, int xShift, int yShift, double angle) const {

    if (wall) {
        glColor3f(0, 0, 0);
    }

    else {
        if (stati == End) {
            glColor3f(1,0,0);

        } else {
            glColor3f(0, 0, 0);
        }
    }

    x = (x * SCALE) + yShift;
    y = (y * SCALE) + xShift;


    //Convert to pixel coordinates
    glBegin(GL_QUADS);

    if (angle == 0 && (wall || (stati == End))) {
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
        if ((wall || (stati == End))) {

            Point p1, p2, p3, p4;
            p1 = rotate(y, x, angle);
            p2 = rotate((y + SCALE), x, angle);
            p3 = rotate((y + SCALE), (x + SCALE), angle);
            p4 = rotate(y, (x + SCALE), angle);

            glVertex2i(p1.x, p1.y);
            glVertex2i(p2.x, p2.y);
            glVertex2i(p3.x, p3.y);
            glVertex2i(p4.x, p4.y);
        }

    }
}

//Flip wall status, used with editor
void Tile::flipWall() {

    if (wall == true) {
        wall = false;

    } else {
        wall = true;
        stati = None;
    }
}

//Modified draw, used in the map editor
void Tile::draw(int x, int y, int xShift, int yShift) const {

    if (wall) {
        glColor3f(0, 0, 0);
    }

    else {
        switch (stati) {

            case (None):
                glColor3f(1, 1, 0);
                break;

            case (Start):
                glColor3f(0, 1, 0);
                break;

            case (End):
                glColor3f(1, 0, 0);
                break;

            case (FlipperSpawn):
                glColor3f(.3, .7, .4);
                break;

            case (SizerSpawn):
                glColor3f(.7, .4, .8);
                break;

            case (ScaryThingSpawn):
                glColor3f(.2, .2, .2);
        }
    }

    x = (x * EDITOR_SCALE) + yShift;
    y = (y * EDITOR_SCALE) + xShift;


    //Convert to pixel coordinates
    glBegin(GL_QUADS);


    // top left corner
    glVertex2i(y, x);
    // top right corner
    glVertex2i((y + EDITOR_SCALE), x);
    // bottom right corner
    glVertex2i((y + EDITOR_SCALE), (x + EDITOR_SCALE));
    // bottom left corner
    glVertex2i(y, (x + EDITOR_SCALE));
    glEnd();
}

//Modified draw, used in the map editor preview
void Tile::draw(int x, int y, int scale, bool hover) const {

    if (wall) {
        glColor3f(.2, .2, .2);
    }

    else {
        switch (stati) {

            case (None):
                if (hover) {
                    glColor3f(1/2, 1/2, 0);
                } else {
                    glColor3f(1, 1, 0);
                }

                break;

            case (Start):
                if (hover) {
                    glColor3f(0, 1/2, 0);
                } else {
                    glColor3f(0, 1, 0);
                }

                break;

            case (End):
                if (hover) {
                    glColor3f(1/2, 0, 0);
                } else {

                    glColor3f(1, 0, 0);
                }

                break;

            case (FlipperSpawn):
                if (hover) {
                    glColor3f(.3/2, .7/2, .4/2);
                } else {
                    glColor3f(.3, .7, .4);
                }

                break;

            case (SizerSpawn):
                if (hover) {
                    glColor3f(.7/2, .4/2, .8/2);
                } else {
                    glColor3f(.7, .4, .8);
                }

                break;
        }
    }


    //Convert to pixel coordinates
    glBegin(GL_QUADS);

    // top left corner
    glVertex2i(x, y);
    // top right corner
    glVertex2i((x + scale), y);
    // bottom right corner
    glVertex2i((x + scale), (y + scale));
    // bottom left corner
    glVertex2i(x, (y + scale));
    glEnd();
}

MazePoint::MazePoint() {
    x = 0;
    y = 0;
}

MazePoint::MazePoint(int X, int Y) {
    x = X;
    y = Y;
}

int MazePoint::getX() const {
    return x;
}
int MazePoint::getY() const {
    return y;
}
void MazePoint::setX(int X) {
    x = X;
}
void MazePoint::setY(int Y) {
    y = Y;
}

bool MazePoint::operator ==(const MazePoint &b) const {
    if ((x == b.getX()) && (y == b.getY())) {
        return true;
    }
    else {
        return false;
    }
}
