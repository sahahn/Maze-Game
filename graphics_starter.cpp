//
// Created by sage on 10/30/17.
//

#include "graphics.h"
#include <iostream>
#include <cmath>
#include <memory>
#include <random>
#include "Maze.h"
#include "Character.h"
using namespace std;


Maze map; //Calling the maze object
int wd;
Player p;
Enemy e, e2;

bool keys[128]; //Holds value of key presses and releases

int angle; //angle of screen tilt in degrees
double angleR; //angle in radians

//Used in keeping track of playing location in array and checking for collisions
int hBoundary;

bool wall; //flag value for collisions

bool rState; //flag for rotation

void init() {
    map = Maze();
    p = Player();

    for (int i =0; i < 128; i++) {
        keys[i] = false;
    }

    angle = 0;

    hBoundary = ((SCALE-p.getSize()) / 2);

    rState = false;

    e = Enemy(10, 10, 20, 10);
    e2 = Enemy(10, 10, 20, 1);

}



/*
 * Calculate collisions
 */
void calcShift(int x, int y) {

    int temp1, temp2;

    wall = false;

    temp1 = rint(p.xShift + ((x * cos(-angleR)) - (y * sin(-angleR))));
    temp2 = rint(p.yShift + ((y * cos(-angleR)) + (x * sin(-angleR))));

    if (temp2 < (-hBoundary)) {

        if (temp1 > hBoundary) {
            if (map.maze[p.x + 1][p.y - 1].getWall()) {

                p.xShift = hBoundary;
                p.yShift = -hBoundary;
                wall = true;
            }
        }

        else if (temp1 < (-hBoundary)) {
            if (map.maze[p.x + 1][p.y + 1].getWall()) {

                p.xShift = -hBoundary;
                p.yShift = -hBoundary;
                wall = true;
            }
        }

        else if (map.maze[p.x+1][p.y].getWall()) {

            p.yShift = -hBoundary;
            wall = true;
        }
    }

    else if (temp1 > (hBoundary)) {

        if (temp2 > hBoundary) {
            if (map.maze[p.x - 1][p.y - 1].getWall()) {

                p.xShift = hBoundary;
                p.yShift = hBoundary;
                wall = true;
            }
        }

        else if (map.maze[p.x][p.y-1].getWall()) {

            p.xShift = hBoundary;
            wall = true;
        }

    }

    else if (temp1 < (-hBoundary)) {

        if (temp2 > hBoundary) {

            if (map.maze[p.x - 1][p.y + 1].getWall()) {
                p.xShift = -hBoundary;
                p.yShift = hBoundary;
                wall = true;
            }
        }

        else if (map.maze[p.x][p.y+1].getWall()) {
            p.xShift = -hBoundary;
            wall = true;
        }
    }

    else if (temp2 > (hBoundary)) {

        if (map.maze[p.x-1][p.y].getWall()) {
            p.yShift = hBoundary;
            wall = true;
        }
    }

    if (!wall) {
        p.xShift = temp1;
        p.yShift = temp2;
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

                e.draw(x,y,p.xShift,p.yShift,angleR);
            }

            if ((e2.x == i) && (e2.y == j)) {

                e2.draw(x,y,p.xShift,p.yShift,angleR);
            }

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


void follow_path(Enemy &E) {

    if (map.maze[E.x][E.y].getCorrectPath()) {
        map.maze[E.x][E.y].setCorrectPath(false);
    }

    if (map.maze[E.x+1][E.y].getCorrectPath()) {
        E.yShift -= E.getSpeed();
    }

    else if (map.maze[E.x-1][E.y].getCorrectPath()) {
        E.yShift += E.getSpeed();
    }

    else if (map.maze[E.x][E.y+1].getCorrectPath()) {
        E.xShift -= E.getSpeed();
    }

    else if (map.maze[E.x][E.y-1].getCorrectPath()) {
        E.xShift += E.getSpeed();
    }

        //When in the players square, different movement behavior
    else if ((p.x == E.x) && (p.y == E.y)) {

        if (p.yShift > E.yShift) {
            E.yShift += E.getSpeed();
        }

        if (p.yShift < E.yShift) {
            E.yShift -= E.getSpeed();
        }

        if (p.xShift < E.xShift) {
            E.xShift -= E.getSpeed();
        }

        if (p.xShift > E.xShift) {
            E.xShift += E.getSpeed();
        }
    }


    E.update();
}


void timer(int extra) {
    //slowly rotate for fun
    //angle = (angle + 1) % 360;
    //angleR = angle * (M_PI /180);

    if (!rState) {

        if (keys[GLUT_KEY_DOWN]) {
            calcShift(0, -p.getSpeed());
        }

        if (keys[GLUT_KEY_LEFT]) {
            calcShift(p.getSpeed(), 0);
        }

        if (keys[GLUT_KEY_UP]) {
            calcShift(0, p.getSpeed());
        }

        if (keys[GLUT_KEY_RIGHT]) {
            calcShift(-p.getSpeed(), 0);
        }

        p.update();

        map.solve_maze(e.x, e.y, p.x, p.y);
        follow_path(e);

        map.solve_maze(e2.x, e2.y, p.x, p.y);
        follow_path(e2);

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