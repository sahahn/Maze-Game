//
// Created by sage on 11/1/17.
//

#include "Maze.h"



Maze::Maze() {


    maze[START_X][START_Y].setWall(false);
    addWall(START_X, START_Y);
    int remove;

    while (wallList.size() != 0) {
        remove = rand() % wallList.size();
        removeOperation(remove);
    }
}

Maze::Maze(int x, int y) {

    maze[x][y].setWall(false);
    addWall(x, y);
    int remove;

    while (wallList.size() != 0) {
        remove = rand() % wallList.size();
        removeOperation(remove);
    }

}

void Maze::checkAndAdd(int x, int y) {

    if (maze[x][y].getWall()) {

        //Check if the point is already in the list
        if (std::find(wallList.begin(), wallList.end(), point((x),y)) != wallList.end()) {
        }
            //Only add if not already present
        else {
            wallList.push_back(point((x),y));
        }
    }
}

void Maze::addWall(int x, int y) {


    if ((x+1) < (HEIGHT-SCOPE)) {
        checkAndAdd((x + 1), y);
    }

    if ((x-1) > SCOPE-1) {
        checkAndAdd((x - 1), y);
    }

    if ((y+1) < (WIDTH-SCOPE)) {
        checkAndAdd(x, (y + 1));
    }

    if ((y-1) > SCOPE-1) {
        checkAndAdd(x, (y - 1));
    }
}

void Maze::removeOperation(int r) {
    int count = 0;
    int x = wallList[r].getX();
    int y = wallList[r].getY();

    if ((x+1) < (HEIGHT-SCOPE)) {
        if (maze[x + 1][y].getWall() == false) {
            count += 1;
        }
    }

    if ((x-1) > SCOPE-1) {
        if (maze[x - 1][y].getWall() == false) {
            count += 1;
        }
    }

    if ((y+1) < (WIDTH-SCOPE)) {
        if (maze[x][y + 1].getWall() == false) {
            count += 1;
        }
    }

    if ((y-1) > SCOPE-1) {
        if (maze[x][y - 1].getWall() == false) {
            count += 1;
        }
    }

    if (count == 1) {
        maze[x][y].setWall(false);
        wallList.erase (wallList.begin()+r);
        addWall(x, y);
    }

    else {
        wallList.erase(wallList.begin()+r);
    }
}