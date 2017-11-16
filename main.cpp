#include <iostream>
#include "graphics.h"
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
        testWithBool("Character > setSize", testPlayer.getSize() == 10);
        testPlayer.setSize(-10);
        testWithBool("Character > getSize", testPlayer.getSize() == 10);
        testPlayer.setSpeed(10);
        testWithBool("Character > setSpeed", testPlayer.getSpeed() == 10);
        testPlayer.setSpeed(-10);
        testWithBool("Character > getSpeed", testPlayer.getSpeed() == 10);
        testPlayer.setLocation(4,-3);
        testWithBool("Character > setLocation & getLocation", testPlayer.getLocation().x == 4 && testPlayer.getLocation().y == -3);
//        testPlayer.calcMove(1,1,0);
//        testPlayer.update();
//        doMove(testPlayer);
//        testWithBool("Character > calcMove & update", testPlayer.getLocation().x == 5 && testPlayer.getLocation().y == -2);

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