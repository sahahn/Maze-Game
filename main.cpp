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
        testWithBool("player > getSize", testPlayer.getSize() == 10);
        testPlayer.setSize(-10);
        testWithBool("player > getSize", testPlayer.getSize() == 10);

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