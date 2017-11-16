#include <iostream>
#include "graphics.h"
#include "GameInfo.h"
#include "Character.h"
using namespace std;
// test variable
bool testsPassed = true;
// this function makes it easy to print out testing information. This also stops the game from running if you fail.
void testWithBool(string testName, bool pass) {
    cout << "Test " << testName << (pass ? " passed." : " FAILED!!!") << endl;
    if (!pass) testsPassed = false;
}
int main(int argc, char** argv){
    // TESTING
    if (TESTING) {
        Player testPlayer = Player();

        testPlayer.setSize(10);
        testWithBool("Player > getSize", testPlayer.getSize() == 10);
        testPlayer.setSize(-10);
        testWithBool("Player > getSize", testPlayer.getSize() == 10);


        Tile testTile = Tile();

        testTile.setWall(true);
        testWithBool("Tile > setWall", testTile.getWall() == true);
        testTile.setEnd(true);
        testWithBool("Tile > setEnd", testTile.getEnd() == true);
        testTile.setWasHere(true);
        testWithBool("Tile > setWashere", testTile.getWasHere() == true);
        testTile.setCorrectPath(true);
        testWithBool("Tile > setCorrectPath", testTile.getCorrectPath() == true);

        MazePoint testMazePoint1 = MazePoint();
        MazePoint testMazePoint2 = MazePoint(5,5);

        testMazePoint1.setX(5);
        testWithBool("MazePoint > setX", testMazePoint1.getX() == 5);
        testMazePoint1.setY(5);
        testWithBool("MazePoint > setY", testMazePoint1.getY() == 5);
        testWithBool("MazePoint > == overload", testMazePoint1 == testMazePoint2);


    }
    // TESTING END
    if (testsPassed) {
        cout << "All tests PASSED!" << endl;
        cout << "Hello, Worm!" << endl;
        graphicsPlay(argc, argv);
    } else {
        cout << "ERROR: A TEST FAILED!" << endl;
    }
    return 0;
}