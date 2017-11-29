#include <iostream>
#include "graphics.h"
#include "GameInfo.h"
#include "Character.h"
#include "EditorGraphics.h"

using namespace std;
// test variable
bool testsPassed = true;

// this function makes it easy to print out testing information. This also stops the game from running if you fail.
void testWithBool(string testName, bool pass) {
    cout << "Test " << testName << (pass ? " passed." : " FAILED!!!") << endl;
    if (!pass) testsPassed = false;
}

int main(int argc, char **argv) {

    graphicsPlay(argc, argv);

    /*

    // TESTING
    if (TESTING) {

        // testing player, character, and enemy
        Player testPlayer = Player();

        testPlayer.setSize(10);
        testWithBool("Character > setSize", testPlayer.getSize() == 10);
        testPlayer.setSize(-10);
        testWithBool("Character > getSize", testPlayer.getSize() == 10);
        testPlayer.setSpeed(10);
        testWithBool("Character > setSpeed", testPlayer.getSpeed() == 10);
        testPlayer.setSpeed(-10);
        testWithBool("Character > getSpeed", testPlayer.getSpeed() == 10);
        testPlayer.setLocation(4, -3);
        testWithBool("Character > setLocation & getLocation",
                     testPlayer.getLocation().x == 4 && testPlayer.getLocation().y == -3);
        testPlayer.setSize(50);
        //testWithBool("Character > hBoundary", testPlayer.hBoundary == ((SCALE - 50) / 2));

        testPlayer.calcMove(10, 10, 90);
        //testWithBool("Player > calcMove", ((testPlayer.temp1) == 4 && (testPlayer.temp2 == -13)));
        testPlayer.xShift = SCALE;
        testPlayer.yShift = -SCALE;
        testPlayer.update();
        testWithBool("Player > update", ((testPlayer.xShift == 0) && (testPlayer.yShift == 0)));

        testPlayer.setPlayerRotation(3.5);
        testWithBool("Player > set&getPlayerRotation", testPlayer.getPlayerRotation() == 3.5);
        testPlayer.updateVelocity(10, 10);
        testWithBool("Player > update&getVelcoity",
                     testPlayer.getVelocity().x == 10 && testPlayer.getVelocity().y == 10);
        testPlayer.flipVelocity();
        testWithBool("Player > flipVelcoity", testPlayer.getVelocity().x == -10 && testPlayer.getVelocity().y == -10);

        testPlayer.taperXVelocity();
        testPlayer.taperYVelocity();
        testWithBool("Player > taperX&YVelocity", testPlayer.getVelocity().x > -10 && testPlayer.getVelocity().y > -10);

        Enemy testEnemy = Enemy(20, 20, 10, 1, Flipper);
        testWithBool("Enemy > Non-Default Constructor", testEnemy.x == 20 && testEnemy.y == 20
                                                        && testEnemy.getSize() == 10 && testEnemy.getSpeed() == 1);

        testEnemy.calcMove(10, 10, 0);
        //testWithBool("Enemy > calcMove", testEnemy.temp1 == 10 && testEnemy.temp2 == 10);
        testEnemy.resetLoc();
        testWithBool("Enemy > resetLoc", testEnemy.getLocation().x == -2 && testEnemy.getLocation().y == -2);
        testEnemy.setType(Flipper);
        testWithBool("Enemy > get&setType", testEnemy.getType() == Flipper);


        Point testRotate = rotate(600, 600, (90 * (LUKE_M_PI / 180)));
        //testWithBool("GameInfo > rotate", ((testRotate.x == 400) && (testRotate.y)));

        // testing maze and tile class
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
        MazePoint testMazePoint2 = MazePoint(5, 5);

        testMazePoint1.setX(5);
        testWithBool("MazePoint > setX", testMazePoint1.getX() == 5);
        testMazePoint1.setY(5);
        testWithBool("MazePoint > setY", testMazePoint1.getY() == 5);
        testWithBool("MazePoint > == overload", testMazePoint1 == testMazePoint2);

        // NOTE: the game information class is not tested because the methods all have side
        // effects that effect the console, requiring a user input in some of them, and would require
        // user input every time the tests are run.
    }

    // TESTING END
    if (testsPassed) {
        cout << "All tests PASSED!" << endl;
        cout << "Hello, Worm!" << endl;
        graphicsPlay(argc, argv);
    } else {
        cout << "ERROR: A TEST FAILED!" << endl;
    }

     */
    return 0;
}