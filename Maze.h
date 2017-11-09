//
// Created by sage on 11/1/17.
//

#ifndef GRAPHICS_MAZE_H
#define GRAPHICS_MAZE_H
#include "Tile.h"
#include "GameInfo.h"
#include <vector>
#include <iostream>
#include <vector>
#include <random>
#include <memory>
#include <algorithm>
using namespace std;

class Maze {

public:

    /*
     * Constructors
     */
    Maze();
    /*
     * Int x and y must be greater than or equal to 1 and less then height(-1), and width(-1) respectively.
     * This constructor calls the starting point for maze generation in a different spot, since w/
     * Prim's algorithim the edges closer to the start have a lower effective weight.
     */
    Maze(int x, int y);

    Tile maze[HEIGHT][WIDTH]; //Make the final maze w/ info public to easily get contents


private:
    int scope;
    vector<point> wallList;

    void checkAndAdd(int x, int y);
    void removeOperation(int r);
    void addWall(int x, int y);
};


#endif //GRAPHICS_MAZE_H
