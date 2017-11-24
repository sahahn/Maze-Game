//
// Created by sage on 11/6/17.
//

#ifndef GRAPHICS_GAMEINFO_H
#define GRAPHICS_GAMEINFO_H


#include <cmath>
#include <ctime>
#include <string>

using namespace std;

// Global settings for use in Graphics,cpp, Tile.cpp, Player.cpp, and Maze.cpp
// Include GameInfo.h to access fields.

const int HEIGHT = 100;
const int WIDTH = 100;
const int SCALE = 200;
const int SCREEN_HEIGHT = 1000;
const int SCREEN_WIDTH = 1000;
const int SCOPE = 3;
const int START_X = 3;
const int START_Y = 3;
const int END_X = 10;
const int END_Y = 10;
const double LUKE_M_PI = 3.14159265358;
const bool TESTING = true;


struct Point {   // Declare Point struct type
    int x;   // Declare member types
    int y;
};

struct DoublePoint {   // Declare Point struct type
    double x;   // Declare member types (doubles)
    double y;
};

struct Score {
    int got;
    double time;
    string name;
    bool completed;
};

/*
 * Takes in an x and y, and an angle in radians and returns the location of the point rotated around the center
 */
Point rotate(int x, int y, double Angle);

/*
 * The actual GameInfo class is used for file IO
 */
class GameInfo {
public:

    //Default constructor
    GameInfo();

    Score score;

    //Calls saveScore and endTimer in one function
    void end();


private:

    //Called at the end of the game,
    //prompts user for name, and does the file IO.
    //Will change when we have menus.
    void saveScore();

    //Also called at end of game, calculates the time passed since start in seconds
    void endTimer();

    clock_t startTime;

    void junk();
};


#endif //GRAPHICS_GAMEINFO_H
