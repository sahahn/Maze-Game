//
// Created by 34u-jH
//

#include "graphics.h"
#include <iostream>
#include "Maze.h"
#include "Character.h"
#include <ctime>
#include <math.h>
using namespace std;


Maze map; //Calling the maze object
Player p;
Enemy e, e2;
GameInfo game;

int wd;

bool keys[128]; //Holds value of key presses and releases

int angle; //angle of screen tilt in degrees
double angleR; //angle in radians

bool rState; //Flag for rotation

void init() {
    game = GameInfo();

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


bool doMove(Character &C) {

    if (C.temp2 < (-C.hBoundary)) {
        if (C.temp1 > C.hBoundary) {
            if (map.maze[C.x + 1][C.y - 1].getWall()) {

                C.xShift = C.hBoundary;
                C.yShift = -C.hBoundary;
                return false;;
            }
        }

        if (C.temp1 < (-C.hBoundary)) {
            if (map.maze[C.x + 1][C.y + 1].getWall()) {

                C.xShift = -C.hBoundary;
                C.yShift = -C.hBoundary;
                return false;;
            }
        }

        if (map.maze[C.x+1][C.y].getWall()) {

            C.yShift = -C.hBoundary;
            return false;;
        }
    }

    if (C.temp1 > (C.hBoundary)) {

        if (C.temp2 > C.hBoundary) {
            if (map.maze[C.x - 1][C.y - 1].getWall()) {

                C.xShift = C.hBoundary;
                C.yShift = C.hBoundary;
                return false;;
            }
        }

        if (map.maze[C.x][C.y-1].getWall()) {

            C.xShift = C.hBoundary;
            return false;;
        }

    }

    if (C.temp1 < (-C.hBoundary)) {

        if (C.temp2 > C.hBoundary) {

            if (map.maze[C.x - 1][C.y + 1].getWall()) {
                C.xShift = -C.hBoundary;
                C.yShift = C.hBoundary;
                return false;;
            }
        }

        if (map.maze[C.x][C.y+1].getWall()) {
            C.xShift = -C.hBoundary;
            return false;;
        }
    }

    if (C.temp2 > (C.hBoundary)) {

        if (map.maze[C.x-1][C.y].getWall()) {
            C.yShift = C.hBoundary;
            return false;;
        }
    }

    //If it reaches this point, there is no wall, update normally
    C.xShift = C.temp1;
    C.yShift = C.temp2;
    return true;

}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0, 0, 0, 0); // Changed color to black
}

/*
 * Main display loop
 */
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

    //lb and up refer to lower bound and upper bound, and are there as error checks to
    //make sure the program doesn't attempt to render outside of the maze array.
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

            //For each position in the array being drawn, i and j, tell the
            //tile piece to draw it at relative location x and y, according to
            //also the players x and yShift and the current angle of the screen.
            map.maze[i][j].draw(x, y, p.xShift, p.yShift, angleR);

            //If any enemies are in one of the squares,
            //store the location of that square in there location field.
            if ((e.x == i) && (e.y == j)) {
                e.setLocation(x, y);
            }

            if ((e2.x == i) && (e2.y == j)) {
                e2.setLocation(x, y);
            }
        }
    }

    //Render enemies, specifically after the maze tiles are displayed.
    e.draw(p.xShift,p.yShift,angleR);
    e2.draw(p.xShift,p.yShift,angleR);

    //Reset the enemies locations, solves a bug.
    e.resetLoc();
    e2.resetLoc();

    //Draw the player
    p.draw();

    // Render now
    glFlush();
}


void kbd(unsigned char key, int x, int y)
{
    // escape
    if (key == 27) {
        cout << "its ova actually tho" << endl;
        glutDestroyWindow(wd);

        game.end();
    }

    //key 32 is space, I just have that rotate the map here
    if (key == 32) {
        rState = true; //Animated transition
    }

    switch (key) {

        case 's':
            keys[GLUT_KEY_DOWN] = true;
            break;

        case 'a':
            keys[GLUT_KEY_LEFT] = true;
            break;

        case 'd':
            keys[GLUT_KEY_RIGHT] = true;
            break;

        case 'w':
            keys[GLUT_KEY_UP] = true;
            break;
    }

    glutPostRedisplay();
}

void kbu(unsigned char key, int x, int y) {

    switch (key) {

        case 's':
            keys[GLUT_KEY_DOWN] = false;
            break;

        case 'a':
            keys[GLUT_KEY_LEFT] = false;
            break;

        case 'd':
            keys[GLUT_KEY_RIGHT] = false;
            break;

        case 'w':
            keys[GLUT_KEY_UP] = false;
            break;
    }
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
    double ang;
    ang = atan2(y - (SCREEN_HEIGHT / 2), x - (SCREEN_WIDTH / 2));

    p.setPlayerRotation(ang);
      //glutPostRedisplay();
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
        E.calcMove(0,-E.getSpeed(),0);
        doMove(E);
    }

    //Move Left
    else if (map.maze[E.x-1][E.y].getCorrectPath()) {

        E.calcMove(0,E.getSpeed(),0);
        doMove(E);
    }

    //Move Up
    else if (map.maze[E.x][E.y+1].getCorrectPath()) {

        E.calcMove(-E.getSpeed(),0,0);
        doMove(E);
    }

    //Move Down
    else if (map.maze[E.x][E.y-1].getCorrectPath()) {

        E.calcMove(E.getSpeed(),0,0);
        doMove(E);
    }

    //When in the player's tile, behave differently
    //Specifically, compensate between the difference in player and enemy x and yShift.
    else if ((p.x == E.x) && (p.y == E.y)) {

        //First check for collision with the player, if no collision...
        if ((p.xShift + p.getSize() < E.xShift) || (E.xShift + E.getSize() < p.xShift) ||
                (p.yShift + p.getSize() < E.yShift) || (E.yShift + E.getSize() < p.yShift)) {


            if (p.yShift > E.yShift) {

                E.calcMove(0, E.getSpeed(), 0);
                doMove(E);

            } else if (p.yShift < E.yShift) {

                E.calcMove(0, -E.getSpeed(), 0);
                doMove(E);

            } else if (p.xShift < E.xShift) {

                E.calcMove(-E.getSpeed(), 0, 0);
                doMove(E);

            } else if (p.xShift > E.xShift) {

                E.calcMove(E.getSpeed(), 0, 0);
                doMove(E);
            }
        }

        //If a collision then, flip the player, and reset the location of the enemy!
        else {

            rState = true;
            E.x = END_X;
            E.y = END_Y;
            cout << "You got Got!" << endl;
            game.score.got++;  // Player gets got
        }
    }

    //Update with new array locations
    E.update();
}

void timer(int extra) {

    if (!rState) {

        if (keys[GLUT_KEY_DOWN]) {
            p.calcMove(0, -p.getSpeed(),angleR);

            if (doMove(p)) {
                p.updateVelocity(0, -p.getSpeed());
            } else {
                p.flipVelocity();
            }
        }

        if (keys[GLUT_KEY_LEFT]) {
            p.calcMove(p.getSpeed(), 0,angleR);

            if (doMove(p)) {
                p.updateVelocity(p.getSpeed(), 0);
            } else {
                p.flipVelocity();
            }
        }

        if (keys[GLUT_KEY_UP]) {
            p.calcMove(0, p.getSpeed(),angleR);

            if (doMove(p)) {
                p.updateVelocity(0, p.getSpeed());
            } else {
                p.flipVelocity();
            }
        }

        if (keys[GLUT_KEY_RIGHT]) {
            p.calcMove(-p.getSpeed(), 0,angleR);

            if (doMove(p)) {
                p.updateVelocity(-p.getSpeed(), 0);
            } else {
                p.flipVelocity();
            }
        }

        if (!keys[GLUT_KEY_DOWN] && !keys[GLUT_KEY_UP]) {
            p.calcMove((int)p.getMovementBuffer().x, 0,angleR);

            if (doMove(p)) {
                p.taperXVelocity();
            } else {
                p.flipVelocity();
            }
            //if (p.getMovementBuffer().x != 0) cout << p.getMovementBuffer().x << endl;
        }
        if (!keys[GLUT_KEY_LEFT] && !keys[GLUT_KEY_RIGHT]) {
            p.calcMove(0, (int)p.getMovementBuffer().y,angleR);

            if (doMove(p)) {
                p.taperYVelocity();
            } else {
                p.flipVelocity();
            }
            //if (p.getMovementBuffer().y != 0) cout << p.getMovementBuffer().y << endl;
        }
        p.update();

        //If you reach the END_X and END_Y square you win!
        if ((p.x == END_X) && (p.y == END_Y)) {
            cout << "You win! " << endl;

            //Mark as completed and end game.
            game.score.completed = true;

            glutDestroyWindow(wd);
            game.end();
        }

        map.solve_maze(e.x, e.y, p.x, p.y);
        follow_path(e);

        map.solve_maze(e2.x, e2.y, p.x, p.y);
        follow_path(e2);

    }

    else {

        angle = (angle + 1) % 360;
        angleR = angle * (LUKE_M_PI /180);

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

    glutKeyboardUpFunc(kbu);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    glutSpecialUpFunc(keyUp); // Tell GLUT to use the method "keyUp" for key up events

    glutSetKeyRepeat(0); //This func makes it so key repeats are off

    // handles mouse movement
    glutPassiveMotionFunc(cursor);

    //glutMotionFunc(mousemov);

    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, timer, 0);

    // Enter the event-processing loop
    glutMainLoop();


    return 0;
}