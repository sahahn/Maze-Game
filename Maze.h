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
#include<bits/stdc++.h>
using namespace std;


//Pairs for solving maze
typedef pair<int, int> Pair;
typedef pair<double, Pair> pPair;

// cell for use with A star algorithim
struct MazeCell {

    //Stores parent + a star fields f,g,h
    Pair parent;
    double f, g, h;
};

class Maze {

public:

    /*
     * Constructor, uses global Constants
     */
    Maze();

    Tile maze[HEIGHT][WIDTH]; //Make the final maze w/ info public to easily get contents

    //void solve_maze(int s_x, int s_y, int e_x, int e_y);

    void aStarSearch(int s_x, int s_y, int e_x, int e_y);

    int getNextX() const;
    int getNextY() const;


private:
    vector<MazePoint> wallList;
    //int endX, endY;
    int nextX, nextY;

    // attempts to add a wall, if there isn't a wall in the position already
    void checkAndAdd(int x, int y);

    //Removes a random wall from the wallList, if it is a valid removal
    void removeOperation(int r);

    //Adds all available walls
    void addWall(int x, int y);

    //bool recursiveSolve (int x, int y);

    bool checkBound(int row, int col);

    bool checkWall(int row, int col);

    bool isDest(int row, int col, Pair dest);

    double calcH(int row, int col, Pair dest);

    void setNextMove(MazeCell cellInfo[][WIDTH], Pair dest);

};


#endif //GRAPHICS_MAZE_H
