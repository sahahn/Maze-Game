//
// Created by sage on 11/1/17.
//

#include "Maze.h"

Maze::Maze() {

    scope = GameInfo::scope;

    maze[GameInfo::start_x][GameInfo::start_y].set_wall(false);
    add_wall(GameInfo::start_x, GameInfo::start_y);
    int remove;

    while (wall_list.size() != 0) {
        remove = rand() % wall_list.size();
        remove_operation(remove);
    }
}

Maze::Maze(int x, int y) {

    maze[x][y].set_wall(false);
    add_wall(x, y);
    maze[10][10].set_wall(false);
    add_wall(10,10);

    int remove;

    while (wall_list.size() != 0) {
        remove = rand() % wall_list.size();
        remove_operation(remove);
    }

}

void Maze::check_and_add(int x, int y) {

    if (maze[x][y].get_wall()) {

        //Check if the point is already in the list
        if (std::find(wall_list.begin(), wall_list.end(), point((x),y)) != wall_list.end()) {
        }
            //Only add if not already present
        else {
            wall_list.push_back(point((x),y));
        }
    }
}

void Maze::add_wall(int x, int y) {


    if ((x+1) < (GameInfo::height-scope)) {
        check_and_add((x+1),y);
    }

    if ((x-1) > scope-1) {
        check_and_add((x-1),y);
    }

    if ((y+1) < (GameInfo::width-scope)) {
        check_and_add(x,(y+1));
    }

    if ((y-1) > scope-1) {
        check_and_add(x,(y-1));
    }
}

void Maze::remove_operation(int r) {
    int count = 0;
    int x = wall_list[r].get_x();
    int y = wall_list[r].get_y();

    if ((x+1) < (GameInfo::height-scope)) {
        if (maze[x+1][y].get_wall() == false) {
            count += 1;
        }
    }

    if ((x-1) > scope-1) {
        if (maze[x-1][y].get_wall() == false) {
            count += 1;
        }
    }

    if ((y+1) < (GameInfo::width-scope)) {
        if (maze[x][y+1].get_wall() == false) {
            count += 1;
        }
    }

    if ((y-1) > scope-1) {
        if (maze[x][y-1].get_wall() == false) {
            count += 1;
        }
    }

    if (count == 1) {
        maze[x][y].set_wall(false);
        wall_list.erase (wall_list.begin()+r);
        add_wall(x,y);
    }

    else {
        wall_list.erase(wall_list.begin()+r);
    }
}

bool Maze::recursive_solve(int x, int y) {
    if ((x == end_X) && (y == end_Y)) {
        return true;
    }

    if ((maze[x][y].get_wall()) || (maze[x][y].get_was_here())) {
        return false;
    }

    // If you are on a wall or already were here
    maze[x][y].set_was_here(true);

    if (x != 0) {                              // Checks if not on left edge
        if (recursive_solve(x - 1, y)) {       // Recalls method one to the left
            maze[x][y].set_correct_path(true); // Sets that path value to true;
            return true;
        }
    }

    if (x != GameInfo::width - 1) {

                                                // Checks if not on right edge
        if (recursive_solve(x+1, y)) {          // Recalls method one to the right
            maze[x][y].set_correct_path(true);  // Sets that path value to true;
            return true;
        }
    }

    if (y != 0) {                               // Checks if not on top edge
        if (recursive_solve(x, y-1)) {          // Recalls method one up
            maze[x][y].set_correct_path(true);  // Sets that path value to true;
            return true;
        }
    }

    if (y != GameInfo::height - 1) {            // Checks if not on bottom edge
        if (recursive_solve(x, y + 1)) {        // Recalls method one down
            maze[x][y].set_correct_path(true);  // Sets that path value to true;
            return true;
        }
    }

    return false;
}


void Maze::solve_maze(int s_x, int s_y, int e_x, int e_y) {

    for (int i=0; i < GameInfo::height; i++) {
        for (int j=0; j < GameInfo::width; j ++) {
            maze[i][j].set_was_here(false);
            maze[i][j].set_correct_path(false);

        }
    }


    start_X = s_x;
    start_Y = s_y;
    end_X = e_x;
    end_Y = e_y;

    bool b = recursive_solve(s_x, s_y); //Will solve maze recursively, and leave solution in correct_path

    maze[end_X][end_Y].set_correct_path(true);
}

