//
// Created by sage on 11/1/17.
//

#include "Maze.h"

using namespace std;

Maze::Maze() {

    //Set the start point, just the maze gen start point, to be not a wall, and add it to the list
    maze[3][3].setWall(false);
    addWall(3, 3);


    int remove;

    //Go through randomly and remove walls
    while (!wallList.empty()) {
        remove = static_cast<int>(rand() % wallList.size());
        removeOperation(remove);
    }
}

Maze::Maze(int sX, int sY) {

    //Set the start point, to be not a wall, and add it to the list
    maze[sX][sY].setWall(false);
    addWall(sX, sY);

    int remove;

    //Go through randomly and remove walls
    while (!wallList.empty()) {
        remove = static_cast<int>(rand() % wallList.size());
        removeOperation(remove);
    }

    addRoom({5, 4});

    addRoom((Point) {12,10}, (Point) {2,2});

}

Maze::Maze(int l, bool live) {
    empty = false;

    if (live) {
        loadMaze(l);

    } else {
        loadMazeEditor(l);
    }
}

//Load a maze/level into the real game
void Maze::loadMaze(int l) {
    //Open the file with error handling
    ifstream inFile;

    string level = std::to_string(l);

    string fileName = level + ".txt";

    inFile.open(fileName);

    if (!inFile) {
        cout << "Error: unable to open level file";
        exit(1); // terminate with error
    }

    string line;
    int intLine;

    //For each line in the file
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {

            std::getline(inFile, line);
            intLine = std::atoi(line.c_str());

            //1 == None, 2 == Start Loc, 3 = End Location
            if ((intLine == 1) || (intLine == 2) || (intLine) == 3) {

                //If not a wall, or enemy
                maze[i][j].setStati(Statee(intLine));
                maze[i][j].setWall(false);


                if (intLine == 2) {

                    playerStartX = i;
                    playerStartY = j;
                }
            }

            else if (intLine >= 4) {

                enemyInfo.push_back(make_pair(make_pair(i,j),intLine));
                maze[i][j].setWall(false);
            }

            //FlipperSpawn = 4, SizerSpawn = 5, ScaryThing = 6
        }
    }

    inFile.close(); //Close the file
}


//Load a maze into the editor
void Maze::loadMazeEditor(int l) {
    //Open the file with error handling
    ifstream inFile;

    string level = std::to_string(l);

    string fileName = level + ".txt";

    inFile.open(fileName);

    if (!inFile) {
        empty = true;
        return;
    }

    string line;
    int intLine;

    //For each line in the file
    for (auto &i : maze) {
        for (int j = 0; j < WIDTH; j++) {

            std::getline(inFile, line);
            intLine = std::atoi(line.c_str());

            //If not a wall, set the tile to what it should be
            if (intLine != 0) {
                i[j].setStati(Statee(intLine));
            }
        }
    }

    inFile.close(); //Close the file

}

void Maze::clearStart() {
    for (auto &i : maze) {
        for (int j = 0; j < WIDTH; j++) {
            if (i[j].getStati() == Start) {
                i[j].setStati(None);
            }
        }
    }
}

void Maze::saveLevel(int l) {

    string level = std::to_string(l);

    ofstream fileOut;

    string fileName = level + ".txt";

    fileOut.open(fileName, ios::out);

    if (fileOut) {

        for (auto &i : maze) {
            for (int j = 0; j < WIDTH; j++) {

                if (i[j].getWall()) {
                    fileOut << 0 << "\n";

                } else {
                    fileOut << i[j].getStati() << "\n";
                }
            }
        }
    }

    fileOut.close();
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
        if (!maze[x + 1][y].getWall()) {
            count += 1;
        }
    }

    if ((x-1) > SCOPE-1) {
        if (!maze[x - 1][y].getWall()) {
            count += 1;
        }
    }

    if ((y+1) < (WIDTH-SCOPE)) {
        if (!maze[x][y + 1].getWall()) {
            count += 1;
        }
    }

    if ((y-1) > SCOPE-1) {
        if (!maze[x][y - 1].getWall()) {
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
    return (maze[row][col].getWall());
}


bool Maze::isDest(int row, int col, Pair dest) {
    return (row == dest.first && col == dest.second);
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
}

void Maze::aStarSearch(int s_x, int s_y, int e_x, int e_y) {
    
    Pair src = make_pair(s_x, s_y);
    Pair dest = make_pair(e_x, e_y);

    //Perform checks to make sure
    if (!checkBound (src.first, src.second)) {
        return;
    }

    if (!checkBound (dest.first, dest.second)) {
        return;
    }

    if (checkWall(src.first, src.second) || checkWall(dest.first, dest.second)) {
        return;
    }

    //If in square, set nextX to be -2 as a flag
    if (isDest(src.first, src.second, dest)) {
        nextX = -2;
        nextY = -2;
        return;
    }

    //A Star algorithm uses two lists, closedList and openList
    bool closedList[HEIGHT][WIDTH];
    memset(closedList, false, sizeof(closedList));

    //Cell info holds the parents and different scores for each cell
    MazeCell cellInfo[HEIGHT][WIDTH];

    for (auto &i : cellInfo) {
        for (int j = 0; j < WIDTH; j++) {

            //Set f,g,h to FLT_MAX at start
            i[j].f = FLT_MAX;
            i[j].g = FLT_MAX;
            i[j].h = FLT_MAX;

            //Set no parent, i.e. -1
            i[j].parent = make_pair(-1,-1);
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
        if (checkBound(i - 1, j)) {

            //Check if destination
            if (isDest(i - 1, j, dest)) {

                //Set Parent
                cellInfo[i - 1][j].parent = make_pair(i,j);

                //Set the next move
                setNextMove(cellInfo, dest);

                //Mark as found
                foundDest = true;
                return;
            }

                //Check if on closed list, or if blocked
            else if (!closedList[i - 1][j] && !checkWall(i - 1, j)) {

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
        if (checkBound(i + 1, j)) {

            //Check if destination
            if (isDest(i + 1, j, dest)) {

                //Set Parent
                cellInfo[i + 1][j].parent = make_pair(i,j);

                //Set the next move
                setNextMove(cellInfo, dest);

                //Mark as found
                foundDest = true;
                return;
            }

                //Check if on closed list, or if blocked
            else if (!closedList[i + 1][j] && !checkWall(i + 1, j)) {

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
        if (checkBound(i, j+1)) {

            //Check if destination
            if (isDest(i, j+1, dest)) {

                //Set Parent
                cellInfo[i][j+1].parent = make_pair(i,j);

                //Set the next move
                setNextMove(cellInfo, dest);

                //Mark as found
                foundDest = true;
                return;
            }

                //Check if on closed list, or if blocked
            else if (!closedList[i][j + 1] && !checkWall(i, j + 1)) {

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
        if (checkBound(i, j-1)) {

            //Check if destination
            if (isDest(i, j-1, dest)) {

                //Set Parent
                cellInfo[i][j-1].parent = make_pair(i,j);

                //Set the next move
                setNextMove(cellInfo, dest);

                //Mark as found
                foundDest = true;
                return;
            }

                //Check if on closed list, or if blocked
            else if (!closedList[i][j - 1] && !checkWall(i, j - 1)) {

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
        if (!foundDest == false) {
            return;
        }
}


int Maze::getNextX() const {
    return nextX;
}

int Maze::getNextY() const {
    return nextY;
}

int Maze::getStartX() const {
    return playerStartX;
}

int Maze::getStartY() const {
    return playerStartY;
}

bool Maze::getEmpty() const {
    return empty;
}

void Maze::setEmpty(bool b) {
    empty = b;
}

void Maze::addRoom(Point loc){
    for (int i = loc.x; i < loc.x + 4; i++){
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

void Maze::fillFromSpot(int x, int y) {
    //Reuse wallList to store non wall tiles
    //Make sure wallList is empty

    wallList.clear();

    wallList.push_back(MazePoint(x,y));

    int amt = -1;
    while (wallList.size() != amt) {

        amt = static_cast<int>(wallList.size());

        //Check from every open square
        for (auto &i : wallList) {
            add(i.getX(), i.getY());

            //If this would override a status box, don't do the fill
            if (maze[i.getX()][i.getY()].getStati() != None) {
                return;
            }
        }
    }

    //Go through and make everything that wall
    for (auto &i : wallList) {
        maze[i.getX()][i.getY()].setWall(true);
    }
}


void Maze::fillAround(int x, int y) {

    if (!maze[x][y].getWall()) {

        //Check if the point is already in the list
        if (std::find(wallList.begin(), wallList.end(), MazePoint(x,y)) != wallList.end()) {
            return;
        }
            //Only add if not already present
        else {
            wallList.push_back(MazePoint(x,y));
        }
    }
}

//Adds all available walls
void Maze::add(int x, int y) {

    if ((x + 1) < (HEIGHT - SCOPE)) {
        fillAround((x + 1), y);
    }

    if ((x - 1) > SCOPE - 1) {
        fillAround((x - 1), y);
    }

    if ((y + 1) < (WIDTH - SCOPE)) {
        fillAround(x,(y + 1));
    }

    if ((y - 1) > SCOPE - 1) {
        fillAround(x ,(y - 1));
    }
}




