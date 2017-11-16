//
// Created by sage on 10/30/17.
//

#include "graphics.h"
#include <iostream>
#include "Maze.h"
#include "Character.h"
using namespace std;
// this function makes it easy to print out testing information
void testWithBool(string testName, bool pass) {
    cout << "Test " << testName << (pass ? " passed." : " FAILED!!!") << endl;
}



Maze map; //Calling the maze object
int wd;
Player p;
Enemy e, e2;
const double M_PI = 3.14159265358;

bool keys[128]; //Holds value of key presses and releases

int angle; //angle of screen tilt in degrees
double angleR; //angle in radians


bool wall; //flag value for collisions

bool rState; //flag for rotation

void init() {
// TESTING
    if (TESTING) {
        Player testPlayer = Player();

        testPlayer.setSize(10);
        testWithBool("player > getSize", testPlayer.getSize() == 10);
        testPlayer.setSize(-10);
        testWithBool("player > getSize", testPlayer.getSize() == 10);

    }
// TESTING END

    map = Maze();
    p = Player();

    for (int i =0; i < 128; i++) {
        keys[i] = false;
    }

    angle = 0;

    rState = false;

    e = Enemy(10, 10, 20, 1);
    e2 = Enemy(10, 11, 20, 1);

}


void doMove(Character &C) {

    wall = false;

    if (C.temp2 < (-C.hBoundary)) {
        if (C.temp1 > C.hBoundary) {
            if (map.maze[C.x + 1][C.y - 1].getWall()) {

                C.xShift = C.hBoundary;
                C.yShift = -C.hBoundary;
                wall = true;
            }
        }

        if (C.temp1 < (-C.hBoundary)) {
            if (map.maze[C.x + 1][C.y + 1].getWall()) {

                C.xShift = -C.hBoundary;
                C.yShift = -C.hBoundary;
                wall = true;
            }
        }

        if (map.maze[C.x+1][C.y].getWall()) {

            C.yShift = -C.hBoundary;
            wall = true;
        }
    }

    if (C.temp1 > (C.hBoundary)) {

        if (C.temp2 > C.hBoundary) {
            if (map.maze[C.x - 1][C.y - 1].getWall()) {

                C.xShift = C.hBoundary;
                C.yShift = C.hBoundary;
                wall = true;
            }
        }

        if (map.maze[C.x][C.y-1].getWall()) {

            C.xShift = C.hBoundary;
            wall = true;
        }

    }

    if (C.temp1 < (-C.hBoundary)) {

        if (C.temp2 > C.hBoundary) {

            if (map.maze[C.x - 1][C.y + 1].getWall()) {
                C.xShift = -C.hBoundary;
                C.yShift = C.hBoundary;
                wall = true;
            }
        }

        if (map.maze[C.x][C.y+1].getWall()) {
            C.xShift = -C.hBoundary;
            wall = true;
        }
    }

    if (C.temp2 > (C.hBoundary)) {

        if (map.maze[C.x-1][C.y].getWall()) {
            C.yShift = C.hBoundary;
            wall = true;
        }
    }

    if (!wall) {
        C.xShift = C.temp1;
        C.yShift = C.temp2;
    }
}


/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0, 0, 0, 0); // Changed color to blue
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // tell OpenGL to use the whole window for drawing
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, -1.f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //This part is a little sketchy too, but basically the lower boundaries and upper boundaries
    //lb1 and up1 ect.. are used to keep what is displayed from trying to render parts that are not in the array
    int lb1 = p.x - SCOPE;
    int upb1 = p.x + SCOPE;
    int lb2 = p.y - SCOPE;
    int upb2 = p.y + SCOPE;

    if (lb1 < 0) {
        lb1 = 0;
    }
    if (lb2 < 0) {
        lb2 = 0;
    }

    if (upb1 > HEIGHT) {
        upb1 = HEIGHT;
    }

    if (upb2 > HEIGHT) {
        upb2 = HEIGHT;
    }

    //x and y here are the relative locations to be rendered on the screen,
    //so by starting with -1, the maze generates from 1 square off the screen
    int x = -1;

    for (int i = lb1; i < upb1+1; i++, x++) {

        int y = -1;

        for (int j = lb2; j < upb2+1; j++, y++) {
            map.maze[i][j].draw(x,y,p.xShift,p.yShift,angleR);

            if ((e.x == i) && (e.y == j)) {
                e.setLocation(x,y);
            }

            if ((e2.x == i) && (e2.y == j)) {
                e2.setLocation(x,y);
            }

        }
    }

    e.draw(p.xShift,p.yShift,angleR);
    e2.draw(p.xShift,p.yShift,angleR);

    e.resetLoc();
    e2.resetLoc();

    p.draw(); //Draw the player

    glFlush();  // Render now
}


// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y)
{
    // escape
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }

    //key 32 is space, I just have that rotate the map here
    if (key == 32) {
        rState = true; //Animated transition
    }

    glutPostRedisplay();

}


void kbdS(int key, int x, int y) {

    switch(key) {

        case GLUT_KEY_DOWN:
            keys[GLUT_KEY_DOWN] = true;
            break;

        case GLUT_KEY_LEFT:
            keys[GLUT_KEY_LEFT] = true;
            break;

        case GLUT_KEY_RIGHT:
            keys[GLUT_KEY_RIGHT] = true;
            break;

        case GLUT_KEY_UP:
            keys[GLUT_KEY_UP] = true;
            break;
    }
    glutPostRedisplay();
}


void keyUp (int key, int x, int y) {


    switch(key) {

        case GLUT_KEY_DOWN:
            keys[GLUT_KEY_DOWN] = false;
            break;

        case GLUT_KEY_LEFT:
            keys[GLUT_KEY_LEFT] = false;
            break;

        case GLUT_KEY_RIGHT:
            keys[GLUT_KEY_RIGHT] = false;
            break;

        case GLUT_KEY_UP:
            keys[GLUT_KEY_UP] = false;
            break;
    }

}

void cursor(int x, int y) {


    //  glutPostRedisplay();
}

void mousemov(int x, int y) {

    //glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {

//glutPostRedisplay();
}

//Implements the logic for any Enemy to follow the correct path to the player
void follow_path(Enemy &E) {

    //Move Right
    if (map.maze[E.x+1][E.y].getCorrectPath()) {

        //First calculate move, then doMove, same for all movement.
        E.calcMove(0,-E.getSpeed());
        doMove(E);
    }

    //Move Left
    else if (map.maze[E.x-1][E.y].getCorrectPath()) {

        E.calcMove(0,E.getSpeed());
        doMove(E);
    }

    //Move Up
    else if (map.maze[E.x][E.y+1].getCorrectPath()) {

        E.calcMove(-E.getSpeed(),0);
        doMove(E);
    }

    //Move Down
    else if (map.maze[E.x][E.y-1].getCorrectPath()) {

        E.calcMove(E.getSpeed(),0);
        doMove(E);
    }

    //When in the player's tile, behave differently
    //Specifically, compensate between the difference in player and enemy x and yShift.
    else if ((p.x == E.x) && (p.y == E.y)) {

        if (p.yShift > E.yShift) {

            E.calcMove(0,E.getSpeed());
            doMove(E);
        }

        else if (p.yShift < E.yShift) {

            E.calcMove(0,-E.getSpeed());
            doMove(E);
        }

        else if (p.xShift < E.xShift) {

            E.calcMove(-E.getSpeed(),0);
            doMove(E);
        }

        else if (p.xShift > E.xShift) {

            E.calcMove(E.getSpeed(),0);
            doMove(E);
        }

        //If no further adjustments, you got tagged!
        //If we want to make it easier to get tagged, we can change how this func works,
        //right now they have to get right up in your face,
        //but we can easily add collision detection between Enemy and Player, instead.
        else {
            cout << "You got Got!" << endl;
        }
    }

    //Update with new array locations
    E.update();
}

void timer(int extra) {

    if (!rState) {

        if (keys[GLUT_KEY_DOWN]) {
            p.calcMove(0, -p.getSpeed(),angleR);
            doMove(p);
        }

        if (keys[GLUT_KEY_LEFT]) {
            p.calcMove(p.getSpeed(), 0,angleR);
            doMove(p);
        }

        if (keys[GLUT_KEY_UP]) {
            p.calcMove(0, p.getSpeed(),angleR);
            doMove(p);
        }

        if (keys[GLUT_KEY_RIGHT]) {
            p.calcMove(-p.getSpeed(), 0,angleR);
            doMove(p);
        }

        p.update();

        if ((p.x == END_X) && (p.y == END_Y)) {
            cout << "You win... " << endl;
        }

        map.solve_maze(e.x, e.y, p.x, p.y);
        follow_path(e);

        map.solve_maze(e2.x, e2.y, p.x, p.y);
        follow_path(e2);

    }

    else {

        angle = (angle + 1) % 360;
        angleR = angle * (M_PI /180);

        if (angle % 90 == 0) {
            rState = 0;
        }
    }



    glutPostRedisplay();
    glutTimerFunc(3, timer, 0);
}


/* Main function: GLUT runs as a console application starting at main()  */
int graphicsPlay(int argc, char** argv) {

    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-SCREEN_WIDTH)/2,
                           (glutGet(GLUT_SCREEN_HEIGHT)-SCREEN_HEIGHT)/2);
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Fun with...Fans!");

    // Register callback handler for window re-paint event
    glutDisplayFunc(display);

    // Our own OpenGL initialization
    initGL();

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    glutSpecialUpFunc(keyUp); // Tell GLUT to use the method "keyUp" for key up events

    glutSetKeyRepeat(0); //This func makes it so key repeats are off



    // handles mouse movement
    //    glutPassiveMotionFunc(cursor);

    //  glutMotionFunc(mousemov);

    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, timer, 0);

    // Enter the event-processing loop
    glutMainLoop();


    return 0;
}