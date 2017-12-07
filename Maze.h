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

//For loading in enemies
typedef pair<Pair, int> eInfo;


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
    Maze(int l, bool edit);
    Maze(int sX, int sY);

    vector<eInfo> enemyInfo;

    /*
     * Clears instances of start squares, because there can only be one!
     */
    void clearStart();

    /*
     * Saves the level to 'l.txt', where l in an int input.
     */
    void saveLevel(int l);

    //Make the final maze w/ info public to easily get contents
    Tile maze[HEIGHT][WIDTH];

    //void solve_maze(int s_x, int s_y, int e_x, int e_y);
    void aStarSearch(int s_x, int s_y, int e_x, int e_y);

    int getNextX() const;
    int getNextY() const;
    bool getEmpty() const;

    int getStartX() const;
    int getStartY() const;

    void setEmpty(bool b );
    void fillFromSpot(int x, int y);


private:
    vector<MazePoint> wallList;
    //int endX, endY;
    int nextX, nextY;
    int playerStartX, playerStartY;

    bool empty;

    /*
     * Loads in a maze/level to the game, where int l in the level to load
     */
    void loadMaze(int l);

    /*
     * Loads in a maze/level to the editor, where int l in the level to load
     */
    void loadMazeEditor(int l);


    // attempts to add a wall, if there isn't a wall in the position already
    void checkAndAdd(int x, int y);

    //Removes a random wall from the wallList, if it is a valid removal
    void removeOperation(int r);

    //Adds all available walls
    void addWall(int x, int y);

    //bool recursiveSolve (int x, int y);

    //Adds a 3x4 room starting at x y coordinates
    void addRoom(Point loc);

    //Adds a room starting at location and builds to size
    void addRoom(Point loc, Point size);

    bool checkBound(int row, int col);

    bool checkWall(int row, int col);

    bool isDest(int row, int col, Pair dest);

    double calcH(int row, int col, Pair dest);

    void setNextMove(MazeCell cellInfo[][WIDTH], Pair dest);

    void fillAround(int x, int y);

    void add(int x, int y);

};


#endif //GRAPHICS_MAZE_H
