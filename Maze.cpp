//
// Created by sage on 11/1/17.
//

#include "Maze.h"

Maze::Maze() {

    scope = SCOPE;

    maze[START_X][START_Y].setWall(false);
    addWall(START_X, START_Y);

    int remove;

    while (wallList.size() != 0) {
        remove = rand() % wallList.size();
        removeOperation(remove);
    }

    maze[END_X][END_Y].setWall(false);
    maze[END_X][END_Y].setEnd(true);
}

Maze::Maze(int x, int y) {

    maze[x][y].setWall(false);

    addWall(x, y);

    int remove;

    while (wallList.size() != 0) {
        remove = rand() % wallList.size();
        removeOperation(remove);
    }

    maze[END_X][END_Y].setWall(false);
    maze[END_X][END_Y].setEnd(true);

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


    if ((x+1) < (HEIGHT-scope)) {
        checkAndAdd((x+1),y);
    }

    if ((x-1) > scope-1) {
        checkAndAdd((x-1),y);
    }

    if ((y+1) < (WIDTH-scope)) {
        checkAndAdd(x,(y+1));
    }

    if ((y-1) > scope-1) {
        checkAndAdd(x,(y-1));
    }
}

void Maze::removeOperation(int r) {
    int count = 0;
    int x = wallList[r].getX();
    int y = wallList[r].getY();

    if ((x+1) < (HEIGHT-scope)) {
        if (maze[x+1][y].getWall() == false) {
            count += 1;
        }
    }

    if ((x-1) > scope-1) {
        if (maze[x-1][y].getWall() == false) {
            count += 1;
        }
    }

    if ((y+1) < (WIDTH-scope)) {
        if (maze[x][y+1].getWall() == false) {
            count += 1;
        }
    }

    if ((y-1) > scope-1) {
        if (maze[x][y-1].getWall() == false) {
            count += 1;
        }
    }

    if (count == 1) {
        maze[x][y].setWall(false);
        wallList.erase (wallList.begin()+r);
        addWall(x,y);
    }

    else {
        wallList.erase(wallList.begin()+r);
    }
}

bool Maze::recursiveSolve(int x, int y) {
    if ((x == endX) && (y == endY)) {
        return true;
    }

    if ((maze[x][y].getWall()) || (maze[x][y].getWasHere())) {
        return false;
    }

    // If you are on a wall or already were here
    maze[x][y].setWasHere(true);

    if (x != 0) {                              // Checks if not on left edge
        if (recursiveSolve(x - 1, y)) {       // Recalls method one to the left
            maze[x][y].setCorrectPath(true); // Sets that path value to true;
            return true;
        }
    }

    if (x != WIDTH - 1) {

                                                // Checks if not on right edge
        if (recursiveSolve(x+1, y)) {          // Recalls method one to the right
            maze[x][y].setCorrectPath(true);  // Sets that path value to true;
            return true;
        }
    }

    if (y != 0) {                               // Checks if not on top edge
        if (recursiveSolve(x, y-1)) {          // Recalls method one up
            maze[x][y].setCorrectPath(true);  // Sets that path value to true;
            return true;
        }
    }

    if (y != HEIGHT - 1) {            // Checks if not on bottom edge
        if (recursiveSolve(x, y + 1)) {        // Recalls method one down
            maze[x][y].setCorrectPath(true);  // Sets that path value to true;
            return true;
        }
    }

    return false;
}


void Maze::solve_maze(int s_x, int s_y, int e_x, int e_y) {

    for (int i=0; i < HEIGHT; i++) {
        for (int j=0; j < WIDTH; j ++) {
            maze[i][j].setWasHere(false);
            maze[i][j].setCorrectPath(false);

        }
    }


    startX = s_x;
    startY = s_y;
    endX = e_x;
    endY = e_y;

    bool b = recursiveSolve(s_x, s_y); //Will solve maze recursively, and leave solution in correct_path

    maze[endX][endY].setCorrectPath(true);
}

