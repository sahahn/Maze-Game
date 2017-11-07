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