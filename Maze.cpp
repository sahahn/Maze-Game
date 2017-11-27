//
// Created by sage on 11/1/17.
//

#include "Maze.h"

using namespace std;

Maze::Maze() {

    //Set the start point, to be not a wall, and add it to the list
    maze[START_X][START_Y].setWall(false);
    addWall(START_X, START_Y);

    int remove;

    //Go through randomly and remove walls
    while (wallList.size() != 0) {
        remove = rand() % wallList.size();
        removeOperation(remove);
    }

    //Set the end of the maze, to not a wall, and to be the end
    maze[END_X][END_Y].setWall(false);
    maze[END_X][END_Y].setEnd(true);

    addRoom({5, 4});

    addRoom((Point) {12,10}, (Point) {2,2});

}

void Maze::checkAndAdd(int x, int y) {

    if (maze[x][y].getWall()) {

        //Check if the point is already in the list
        if (std::find(wallList.begin(), wallList.end(), MazePoint(x,y)) != wallList.end()) {
        }
            //Only add if not already present
        else {
            wallList.push_back(MazePoint((x),y));
        }
    }
}


//Adds all available walls
void Maze::addWall(int x, int y) {

    if ((x + 1) < (HEIGHT - SCOPE)) {
        checkAndAdd((x + 1), y);
    }

    if ((x - 1) > SCOPE - 1) {
        checkAndAdd((x - 1), y);
    }

    if ((y + 1) < (WIDTH - SCOPE)) {
        checkAndAdd(x,(y + 1));
    }

    if ((y - 1) > SCOPE - 1) {
        checkAndAdd(x ,(y - 1));
    }
}

//Removes a random wall from the wallList, if it is a valid removal
void Maze::removeOperation(int r) {
    int count = 0;
    int x = wallList[r].getX();
    int y = wallList[r].getY();

    if ((x+1) < (HEIGHT-SCOPE)) {
        if (maze[x+1][y].getWall() == false) {
            count += 1;
        }
    }

    if ((x-1) > SCOPE-1) {
        if (maze[x-1][y].getWall() == false) {
            count += 1;
        }
    }

    if ((y+1) < (WIDTH-SCOPE)) {
        if (maze[x][y+1].getWall() == false) {
            count += 1;
        }
    }

    if ((y-1) > SCOPE-1) {
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


bool Maze::checkBound(int row, int col) {

    return (row >= 0) && (row < HEIGHT) &&
           (col >= 0) && (col < WIDTH);
}


bool Maze::checkWall(int row, int col) {
    // Returns true if the cell is not blocked else false
    if (maze[row][col].getWall()) {
        return true;
    } else {
        return false;
    }
}


bool Maze::isDest(int row, int col, Pair dest) {
    if (row == dest.first && col == dest.second) {
        return true;
    }
    else {
        return false;
    }
}

double Maze::calcH(int row, int col, Pair dest) {
    // Distance formula, can replace w/ dif hueristic also
    return (sqrt ((row-dest.first)*(row-dest.first)
                          + (col-dest.second)*(col-dest.second)));
}

void Maze::setNextMove(MazeCell cellInfo[][WIDTH], Pair dest) {

    int row = dest.first;
    int col = dest.second;

    stack<Pair> Path;

    while (!(cellInfo[row][col].parent.first == row
             && cellInfo[row][col].parent.second == col )) {

        Path.push (make_pair(row, col));
        int temp_row = cellInfo[row][col].parent.first;
        int temp_col = cellInfo[row][col].parent.second;
        row = temp_row;
        col = temp_col;
    }

    nextX = Path.top().first;
    nextY = Path.top().second;

    return;
}

void Maze::aStarSearch(int s_x, int s_y, int e_x, int e_y) {
    
    Pair src = make_pair(s_x, s_y);
    Pair dest = make_pair(e_x, e_y);

    //Perform checks to make sure
    if (checkBound (src.first, src.second) == false) {
        return;
    }

    if (checkBound (dest.first, dest.second) == false) {
        return;
    }

    if (checkWall(src.first, src.second) == true || checkWall(dest.first, dest.second) == true) {
        return;
    }

    //If in square, set nextX to be -2 as a flag
    if (isDest(src.first, src.second, dest) == true) {
        nextX = -2;
        nextY = -2;
        return;
    }

    //A Star algorithm uses two lists, closedList and openList
    bool closedList[HEIGHT][WIDTH];
    memset(closedList, false, sizeof(closedList));

    //Cell info holds the parents and different scores for each cell
    MazeCell cellInfo[HEIGHT][WIDTH];

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {

            //Set f,g,h to FLT_MAX at start
            cellInfo[i][j].f = FLT_MAX;
            cellInfo[i][j].g = FLT_MAX;
            cellInfo[i][j].h = FLT_MAX;

            //Set no parent, i.e. -1
            cellInfo[i][j].parent = make_pair(-1,-1);
        }
    }

    int i, j;

    //Init the starting point of the search
    i = src.first, j = src.second;

    cellInfo[i][j].f = 0.0;
    cellInfo[i][j].g = 0.0;
    cellInfo[i][j].h = 0.0;

    cellInfo[i][j].parent = src;

    /*
     Create an open list having information as-
     <f, <i, j>>
     where f = g + h,
     and i, j are the row and column index of that cell
     Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
     This open list is implenented as a set of pair of pair.*/
    set<pPair> openList;

    // Put the starting cell on the open list and set its
    // 'f' as 0
    openList.insert(make_pair (0.0, make_pair (i, j)));

    // We set this boolean value as false as initially
    // the destination is not reached.
    bool foundDest = false;

    while (!openList.empty()) {
        pPair p = *openList.begin();

        // Remove this vertex from the open list
        openList.erase(openList.begin());

        // Add this vertex to the open list
        i = p.second.first;
        j = p.second.second;

        closedList[i][j] = true;

        double gNew, hNew, fNew;

        //Check above
        if (checkBound(i - 1, j) == true) {

            //Check if destination
            if (isDest(i - 1, j, dest) == true) {

                //Set Parent
                cellInfo[i - 1][j].parent = make_pair(i,j);

                //Set the next move
                setNextMove(cellInfo, dest);

                //Mark as found
                foundDest = true;
                return;
            }

                //Check if on closed list, or if blocked
            else if (closedList[i - 1][j] == false && checkWall(i - 1, j) == false) {

                //Calculate new g,h and f
                gNew = cellInfo[i][j].g + 1.0;
                hNew = calcH(i - 1, j, dest);
                fNew = gNew + hNew;

                //If it isn't on the open list, or if it scores better, add/update it to openlist
                if (cellInfo[i - 1][j].f == FLT_MAX || cellInfo[i - 1][j].f > fNew) {

                    openList.insert(make_pair(fNew, make_pair(i - 1, j)));

                    // Update the details
                    cellInfo[i - 1][j].f = fNew;
                    cellInfo[i - 1][j].g = gNew;
                    cellInfo[i - 1][j].h = hNew;

                    cellInfo[i - 1][j].parent = make_pair(i,j);
                }
            }
        }

        //Check below
        if (checkBound(i + 1, j) == true) {

            //Check if destination
            if (isDest(i + 1, j, dest) == true) {

                //Set Parent
                cellInfo[i + 1][j].parent = make_pair(i,j);

                //Set the next move
                setNextMove(cellInfo, dest);

                //Mark as found
                foundDest = true;
                return;
            }

                //Check if on closed list, or if blocked
            else if (closedList[i + 1][j] == false && checkWall(i + 1, j) == false) {

                //Calculate new g,h and f
                gNew = cellInfo[i][j].g + 1.0;
                hNew = calcH(i + 1, j, dest);
                fNew = gNew + hNew;

                //If it isn't on the open list, or if it scores better, add/update it to openlist
                if (cellInfo[i + 1][j].f == FLT_MAX || cellInfo[i + 1][j].f > fNew) {

                    openList.insert(make_pair(fNew, make_pair(i + 1, j)));

                    // Update the details
                    cellInfo[i + 1][j].f = fNew;
                    cellInfo[i + 1][j].g = gNew;
                    cellInfo[i + 1][j].h = hNew;

                    cellInfo[i + 1][j].parent = make_pair(i,j);
                }
            }
        }

        //Check right
        if (checkBound(i, j+1) == true) {

            //Check if destination
            if (isDest(i, j+1, dest) == true) {

                //Set Parent
                cellInfo[i][j+1].parent = make_pair(i,j);

                //Set the next move
                setNextMove(cellInfo, dest);

                //Mark as found
                foundDest = true;
                return;
            }

                //Check if on closed list, or if blocked
            else if (closedList[i][j+1] == false && checkWall(i, j+1) == false) {

                //Calculate new g,h and f
                gNew = cellInfo[i][j].g + 1.0;
                hNew = calcH(i, j+1, dest);
                fNew = gNew + hNew;

                //If it isn't on the open list, or if it scores better, add/update it to openlist
                if (cellInfo[i][j+1].f == FLT_MAX || cellInfo[i][j+1].f > fNew) {

                    openList.insert(make_pair(fNew, make_pair(i, j+1)));

                    // Update the details
                    cellInfo[i][j+1].f = fNew;
                    cellInfo[i][j+1].g = gNew;
                    cellInfo[i][j+1].h = hNew;

                    cellInfo[i][j+1].parent = make_pair(i,j);
                }
            }
        }

        //Check left
        if (checkBound(i, j-1) == true) {

            //Check if destination
            if (isDest(i, j-1, dest) == true) {

                //Set Parent
                cellInfo[i][j-1].parent = make_pair(i,j);

                //Set the next move
                setNextMove(cellInfo, dest);

                //Mark as found
                foundDest = true;
                return;
            }

                //Check if on closed list, or if blocked
            else if (closedList[i][j-1] == false && checkWall(i, j-1) == false) {

                //Calculate new g,h and f
                gNew = cellInfo[i][j].g + 1.0;
                hNew = calcH(i, j-1, dest);
                fNew = gNew + hNew;

                //If it isn't on the open list, or if it scores better, add/update it to openlist
                if (cellInfo[i][j-1].f == FLT_MAX || cellInfo[i][j-1].f > fNew) {

                    openList.insert(make_pair(fNew, make_pair(i, j-1)));

                    // Update the details
                    cellInfo[i][j-1].f = fNew;
                    cellInfo[i][j-1].g = gNew;
                    cellInfo[i][j-1].h = hNew;

                    cellInfo[i][j-1].parent = make_pair(i,j);
                }
            }
        }

        //If we want to include maze solving w/ diagonals, just add four more cases here
    }
        //Maze cant be solved for some reason
        if (foundDest == false) {
            return;
        }
}

int Maze::getNextX() const {
    return nextX;
}

int Maze::getNextY() const {
    return nextY;
}

void Maze::addRoom(Point loc){
    for(int i = loc.x; i < loc.x + 4; i++){
        for(int j = loc.y; j < loc.y + 3; j++){
            maze[j][i].setWall(false);
        }
    }


}

void Maze::addRoom(Point loc, Point size) {
    for (int i = loc.x; i < loc.x + size.x + 4; i++) {
        for (int j = loc.y; j < loc.y + size.y + 3; j++) {
            maze[j][i].setWall(false);
        }
    }
}

/*    IMPLIMENTATION TO SOLVE PERFECT MAZE
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

    if (y != HEIGHT - 1) {                      // Checks if not on bottom edge
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

    endX = e_x;
    endY = e_y;

    bool b = recursiveSolve(s_x, s_y); //Will solve maze recursively, and leave solution in correct_path

    maze[endX][endY].setCorrectPath(true);
}
 
 */





