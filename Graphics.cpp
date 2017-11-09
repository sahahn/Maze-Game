//
// Created by sage on 10/30/17.
//

#include "GameInfo.h"
#include "graphics.h"
#include "Maze.h"
#include "Player.h"
#include "Tile.h"

#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <random>
#include <vector>

using namespace std;

Maze map; //Calling the maze object
int wd;
Player p;
Point pt;

//I use the xShift and true x_shifts in calculating movement
int xShift, yShift;
double true_x_shift, true_y_shift;

bool keys[128]; //not used yet

int angle; //angle of screen tilt in degrees
double angleR; //angle in radians

int temp1, temp2;

int person_size; //Size of the "player"

//Used in keeping track of playing location in array and checking for collisions
int sBoundary;
int hBoundary;

bool wall; //flag value for collisions

bool rState;

void init() {
    map = Maze();
    p = Player();

    xShift = 0;
    yShift = 0;

    for (int i =0; i < 128; i++) {
        keys[i] = false;
    }

    angle = 0;

    hBoundary = ((SCALE-p.getSize()) / 2);
    sBoundary = (SCALE/2);

    rState = false;
}



/*
 * My rough first attempt at calculating collisions, will probally want to re-work this.
 * NOTE: as currently implemented still misses certain edge cases, I'll try and fix this soon
 */
void calcShift(int x, int y) {

    wall = false;

    temp1 = rint(xShift + ((x * cos(-angleR)) - (y * sin(-angleR))));
    temp2 = rint(yShift + ((y * cos(-angleR)) + (x * sin(-angleR))));


    if (temp2 < (-hBoundary)) {

        if (temp1 > hBoundary) {
            if (map.maze[p.x + 1][p.y - 1].getWall()) {

                xShift = hBoundary;
                yShift = -hBoundary;
                wall = true;
            }
        }

        else if (temp1 < (-hBoundary)) {
            if (map.maze[p.x + 1][p.y + 1].getWall()) {

                xShift = -hBoundary;
                yShift = -hBoundary;
                wall = true;
            }
        }

        else if (map.maze[p.x + 1][p.y].getWall()) {

            yShift = -hBoundary;
            wall = true;
        }
    }

    else if (temp1 > (hBoundary)) {

        if (temp2 > hBoundary) {
            if (map.maze[p.x - 1][p.y - 1].getWall()) {

                xShift = hBoundary;
                yShift = hBoundary;
                wall = true;
            }
        }

        else if (map.maze[p.x][p.y - 1].getWall()) {

            xShift = hBoundary;
            wall = true;
        }
    }

    else if (temp1 < (-hBoundary)) {

        if (temp2 > hBoundary) {
            if (map.maze[p.x - 1][p.y + 1].getWall()) {
                xShift = -hBoundary;
                yShift = hBoundary;
                wall = true;
            }
        }

        else if (map.maze[p.x][p.y + 1].getWall()) {
            xShift = -hBoundary;
            wall = true;
        }
    }

    else if (temp2 > (hBoundary)) {

        if (map.maze[p.x - 1][p.y].getWall()) {
            yShift = hBoundary;
            wall = true;
        }
    }

    if (wall == false) {
        xShift = temp1;
        yShift = temp2;
    }
}


/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.1f, 0.1f, 1.0f, 1.0f); // Changed color to blue
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

    if (upb2 > WIDTH) {
        upb2 = WIDTH;
    }

    //x and y here are the relative locations to be rendered on the screen,
    //so by starting with -1, the maze generates from 1 square off the screen
    int x = -1;

    for (int i = lb1; i < upb1+1; i++, x++) {

        int y = -1;

        for (int j = lb2; j < upb2+1; j++, y++) {
            map.maze[i][j].draw(x,y,xShift,yShift,angleR);

        }
    }

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
        //angle = (angle + 90) % 360;
        //angleR = angle * (M_PI /180);
        rState = true; //Animated transition
    }

    glutPostRedisplay();

    return;
}


void kbdS(int key, int x, int y) {

    /*
     * This whole chunk could be reworked, or atleast redesigned so that we keep track of
     * key up and key down, and do the actual movement somewhere else.
     */
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

    //std::cout << true_x_shift << " " << true_y_shift << std::endl;

    glutPostRedisplay();

    return;
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

//void cursor(int x, int y) {
//

  //  glutPostRedisplay();
//}

//void mousemov(int x, int y) {


//    glutPostRedisplay();
//}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {

//glutPostRedisplay();
}


void timer(int extra) {
    //slowly rotate for fun
    //angle = (angle + 1) % 360;
    //angleR = angle * (M_PI /180);

    if (rState == false) {

        if (keys[GLUT_KEY_DOWN]) {
            calcShift(0, -p.speed);
        }

        if (keys[GLUT_KEY_LEFT]) {
            calcShift(p.speed, 0);
        }

        if (keys[GLUT_KEY_UP]) {
            calcShift(0, p.speed);
        }

        if (keys[GLUT_KEY_RIGHT]) {
            calcShift(-p.speed, 0);
        }

        //So what I'm doing here, is with the true_x and true_y shifts.
        //I determine the place where loc.x and y ends up, so basically
        //keeping track of the players location in the maze array. The exact location/ what appears
        //on screen a mix of loc.x and loc.y, with the x and y shift handling all the small movement between array chunks.
        if (yShift < (-sBoundary)) {
            p.x += 1;
            yShift += SCALE;
        }

        if (xShift > (sBoundary)) {
            p.y -= 1;
            xShift -= SCALE;
        }

        if (xShift < (-sBoundary)) {
            p.y += 1;
            xShift += SCALE;
        }

        if (yShift > (sBoundary)) {
            p.x -= 1;
            yShift -= SCALE;
        }
    }

    else {

        angle = (angle + 3) % 360;
        angleR = angle * (M_PI /180);

        if (angle % 90 == 0) {
            rState = 0;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(50, timer, 0);
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
   // glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, timer, 0);

    // Enter the event-processing loop
    glutMainLoop();


    return 0;
}