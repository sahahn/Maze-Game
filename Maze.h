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
     * Constructors
     */
    Maze();
    /*
     * Int x and y must be greater than or equal to 1 and less then height(-1), and width(-1) respectively.
     * This constructor calls the starting point for maze generation in a different spot, since w/
     * Prim's algorithim the edges closer to the start have a lower effective weight.
     */
    Maze(int x, int y);

    Tile maze[GameInfo::height][GameInfo::width]; //Make the final maze w/ info public to easily get contents

    void solve_maze(int s_x, int s_y, int e_x, int e_y);


private:
    int scope;
    vector<point> wall_list;
    int start_X, start_Y; // Starting x and y for solving maze
    int end_X, end_Y;     // End x and y for for solving maze




    void check_and_add(int x, int y);
    void remove_operation(int r);
    void add_wall(int x, int y);

    bool recursive_solve (int x, int y);

};


#endif //GRAPHICS_MAZE_H
