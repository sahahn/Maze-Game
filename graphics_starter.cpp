//
// Created by sage on 10/30/17.
//

#include "graphics.h"
#include <iostream>

#include <iostream>
#include "graphics.h"
#include "Tile.h"
#include <vector>
#include <random>
#include <memory>
#include <algorithm>
#include "Maze.h"
#include "Player.h"
using namespace std;



//NOTE EVERYTHING IN THIS FILE IS STILL VERY MESSY! SORRY!
//There are a few very half implimented/thought out ideas, like holding key values

Maze map; //Im just calling the maze object
int wd;
Player p;
int scale; //Scale is how big each item in the array is displayed
int scope; //Scope is how much of the array/map is rendered each time

//I use the x_shift and true x_shifts in calculating movement
int x_shift, y_shift;
double true_x_shift, true_y_shift;

bool keys[128]; //not used yet

int angle; //angle of screen tilt in degrees
double angleR; //angle in radians

double temp1; //Used in calculating collisions
double temp2;

int person_size; //Size of the "player"

//Used in keeping track of playing location in array and checking for collisions
int sBoundary;
int hBoundary;

bool wall; //flag value for collisions


void init() {
    map = Maze();
    p = Player();

    scope = 4;
    scale = GameInfo::scale;

    x_shift = 0;
    y_shift = 0;

    for (int i =0; i < 128; i++) {
        keys[i] = false;
    }

    angle = 0;

    true_x_shift = 0;
    true_y_shift = 0;

    hBoundary = ((scale-person_size) / 2);
    sBoundary = (scale/2);

}



/*
 * My rough first attempt at calculating collisions, will probally want to re-work this.
 * NOTE: as currently implemented still misses certain edge cases, I'll try and fix this soon
 */
void calcShift(int x, int y) {

    wall = false;

    temp1 = true_x_shift + ((x * cos(-angleR)) - (y * sin(-angleR)));
    temp2 = true_y_shift + ((y * cos(-angleR)) + (x * sin(-angleR)));


    if (temp2 < (-hBoundary)) {

        if (temp1 > hBoundary) {
            if (map.maze[p.x + 1][p.y - 1].get_wall()) {
                wall = true;
            }
        }

        else if (temp1 < (-hBoundary)) {
            if (map.maze[p.x + 1][p.y + 1].get_wall()) {
                wall = true;
            }
        }

        else if (map.maze[p.x+1][p.y].get_wall()) {
            wall = true;
        }
    }

    if (temp1 > (hBoundary)) {

        if (temp2 > hBoundary) {
            if (map.maze[p.x - 1][p.y - 1].get_wall()) {
                wall = true;
            }
        }

        else if (map.maze[p.x][p.y-1].get_wall()) {
            wall = true;
        }
    }

    if (temp1 < (-hBoundary)) {

        if (temp2 > hBoundary) {
            if (map.maze[p.x - 1][p.y + 1].get_wall()) {
                wall = true;
            }
        }

        else if (map.maze[p.x][p.y+1].get_wall()) {
            wall = true;
        }
    }

    if (temp2 > (hBoundary)) {

        if (map.maze[p.x-1][p.y].get_wall()) {
            wall = true;
        }
    }

    if (wall == false) {
        true_x_shift = temp1;
        true_y_shift = temp2;
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
    glViewport(0, 0, GameInfo::screen_width, GameInfo::screen_height);

    // do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, GameInfo::screen_width, GameInfo::screen_height, 0.0, -1.f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    //Okay so the way I'm doing this right now, is calculating the shifts from their "true" double values
    x_shift = ((int) true_x_shift+.5);
    y_shift = ((int) true_y_shift+.5);

    //This part is a little sketchy too, but basically the lower boundaries and upper boundaries
    //lb1 and up1 ect.. are used to keep what is displayed from trying to render parts that are not in the array
    int lb1 = p.x - scope;
    int upb1 = p.x + scope;
    int lb2 = p.y - scope;
    int upb2 = p.y + scope;

    if (lb1 < 0) {
        lb1 = 0;
    }
    if (lb2 < 0) {
        lb2 = 0;
    }

    if (upb1 > GameInfo::height) {
        upb1 = GameInfo::height;
    }

    if (upb2 > GameInfo::width) {
        upb2 = GameInfo::width;
    }

    //x and y here are the relative locations to be rendered on the screen,
    //so by starting with -1, the maze generates from 1 square off the screen
    int x = -1;

    for (int i = lb1; i < upb1+1; i++, x++) {

        int y = -1;

        for (int j = lb2; j < upb2+1; j++, y++) {
            map.maze[i][j].draw(x,y,x_shift,y_shift,angleR);

        }
    }

    p.draw();

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
        angle = (angle + 3) % 360;
        angleR = angle * (M_PI /180);
    }

    glutPostRedisplay();

    return;
}

void betterKeys() {

    switch (keys[GLUT_KEY_DOWN] == true) {
        //
    }

}

void kbdS(int key, int x, int y) {

    /*
     * This whole chunk could be reworked, or atleast redesigned so that we keep track of
     * key up and key down, and do the actual movement somewhere else.
     */
    switch(key) {

        case GLUT_KEY_DOWN:
            //loc.x += 1;
            //y_shift -= 10;

            calcShift(0,-20);

            break;
        case GLUT_KEY_LEFT:
            //loc.y -= 1;
           // x_shift += 10;

            calcShift(20,0);

            break;
        case GLUT_KEY_RIGHT:
            //loc.y += 1;
            //x_shift -= 10;

            calcShift(-20,0);
            break;

        case GLUT_KEY_UP:
            //loc.x -= 1;
            //y_shift += 10;

            calcShift(0,20);

            break;
    }


    //So what I'm doing here, is with the true_x and true_y shifts.
    //I determine the place where loc.x and y ends up, so basically
    //keeping track of the players location in the maze array. The exact location/ what appears
    //on screen a mix of loc.x and loc.y, with the x and y shift handling all the small movement between array chunks.
    if (true_y_shift < (-sBoundary)) {
        p.x +=1;
        true_y_shift += scale;
    }

    if (true_x_shift > (sBoundary)) {
        p.y -= 1;
        true_x_shift -= scale;
    }

    if (true_x_shift < (-sBoundary)) {
        p.y += 1;
        true_x_shift += scale;
    }

    if (true_y_shift > (sBoundary)) {
        p.x -= 1;
        true_y_shift -=scale;
    }

    //std::cout << true_x_shift << " " << true_y_shift << std::endl;

    glutPostRedisplay();

    return;
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
    //This is where I have the map slowly rotate for fun
    angle = (angle + 1) % 360;
    angleR = angle * (M_PI /180);

    glutPostRedisplay();
    glutTimerFunc(50, timer, 0);
}



/* Main function: GLUT runs as a console application starting at main()  */
int graphicsPlay(int argc, char** argv) {

    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize(GameInfo::screen_width, GameInfo::screen_height);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-GameInfo::screen_width)/2,
                           (glutGet(GLUT_SCREEN_HEIGHT)-GameInfo::screen_height)/2);
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