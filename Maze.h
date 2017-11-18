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

    // This method solves the maze for the enemies based on the position of the enemy and the player
    void solve_maze(int s_x, int s_y, int e_x, int e_y);


private:
    vector<MazePoint> wallList;
    int endX, endY;     // End x and y for for solving maze, DIF THEN GLOBAL CONST

    // attempts to add a wall, if there isn't a wall in the position already
    void checkAndAdd(int x, int y);

    //Removes a random wall from the wallList, if it is a valid removal
    void removeOperation(int r);

    //Adds all available walls
    void addWall(int x, int y);
    // solves the maze (documentation inside function by line comments)
    bool recursiveSolve (int x, int y);

};


#endif //GRAPHICS_MAZE_H
