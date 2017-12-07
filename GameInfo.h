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

const int HEIGHT = 30;
const int WIDTH = 30;

//For Live Game
const int SCALE = 300;
const int SCREEN_HEIGHT = 900;
const int SCREEN_WIDTH = 900;
const int SCOPE = 2;

//For Editor
const int EDITOR_SCALE = 70;
const int EDITOR_SCOPE = 8;

const double LUKE_M_PI = 3.14159265358;
const bool TESTING = true;
const int MAX_ENEMIES = 20;
const int NUM_LEVELS = 12;

//Only store up to 100 players in players.txt
const int MAX_PLAYERS = 100;


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

    int loadPlayer(string name);
    void updatePlayer(string name, int level);
    void makePlayer(string name);


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
