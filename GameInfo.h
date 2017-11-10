//
// Created by sage on 11/6/17.
//

#ifndef GRAPHICS_GAMEINFO_H
#define GRAPHICS_GAMEINFO_H
// Global settings for use in Graphics,cpp, Tile.cpp, Player.cpp, and Maze.cpp
// Include GameInfo.h to access fields.

// This is also where we might want to add in file I/O for game saves.
// I'm thinking we can use srand(int x) to save seeds of maps to output file,
// and then load them to regenerate the same level.

static const int HEIGHT = 100;
static const int WIDTH = 100;
static const int SCALE = 100;
static const int SCREEN_HEIGHT = 700;
static const int SCREEN_WIDTH = 700;
static const int SCOPE = 4;
static const int START_X = 4;
static const int START_Y = 4;



#endif //GRAPHICS_GAMEINFO_H
