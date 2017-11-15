//
// Created by sage on 11/1/17.
//

#ifndef GRAPHICS_MAZE_H
#define GRAPHICS_MAZE_H
#include "Tile.h"
#include <vector>
#include <iostream>
#include <vector>
#include <random>
#include <memory>
#include <algorithm>
#include "GameInfo.h"
using namespace std;

class Maze {

public:

    /*
     * Constructor, uses global Constants
     */
    Maze();

    Tile maze[HEIGHT][WIDTH]; //Make the final maze w/ info public to easily get contents

    void solve_maze(int s_x, int s_y, int e_x, int e_y);


private:
    vector<point> wallList;
    int endX, endY;     // End x and y for for solving maze, DIF THEN GLOBAL CONST

    void checkAndAdd(int x, int y);
    void removeOperation(int r);
    void addWall(int x, int y);

    bool recursiveSolve (int x, int y);

};


#endif //GRAPHICS_MAZE_H
