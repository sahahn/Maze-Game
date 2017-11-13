//
// Created by sage on 11/6/17.
//

#ifndef GRAPHICS_GAMEINFO_H
#define GRAPHICS_GAMEINFO_H


#include <cmath>
// Global settings for use in Graphics,cpp, Tile.cpp, Player.cpp, and Maze.cpp
// Include GameInfo.h to access fields.

// This is also where we might want to add in file I/O for game saves.
// I'm thinking we can use srand(int x) to save seeds of maps to output file,
// and then load them to regenerate the same level.

static const int HEIGHT = 100;
static const int WIDTH = 100;
static const int SCALE = 200;
static const int SCREEN_HEIGHT = 1000;
static const int SCREEN_WIDTH = 1000;
static const int SCOPE = 3;
static const int START_X = 3;
static const int START_Y = 3;

struct Point {   // Declare Point struct type
    int x;   // Declare member types
    int y;
};

/*
 * Takes in an x and y, and an angle in radians and returns the location of the point rotated around the center
 */
Point cp(int x, int y, double Angle);



#endif //GRAPHICS_GAMEINFO_H
