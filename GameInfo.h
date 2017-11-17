//
// Created by sage on 11/6/17.
//

#ifndef GRAPHICS_GAMEINFO_H
#define GRAPHICS_GAMEINFO_H


#include <cmath>
#include <ctime>
#include <string>

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
static const int END_X = 10;
static const int END_Y = 10;
static const bool TESTING = true;


struct Point {   // Declare Point struct type
    int x;   // Declare member types
    int y;
};
struct DoublePoint {   // Declare Point struct type
    double x;   // Declare member types (doubles)
    double y;
};

/*
 * Takes in an x and y, and an angle in radians and returns the location of the point rotated around the center
 */
Point rotate(int x, int y, double Angle);

struct Score{
    int got;
    double time;
    std::string name;
};

using namespace std;

class GameInfo {
public:
    Score score;

    GameInfo();

    void saveScore();

    void endTimer();

private:
    static clock_t start;

    void junk();
};


#endif //GRAPHICS_GAMEINFO_H
