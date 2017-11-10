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
Enemy e;
int scale; //Scale is how big each item in the array is displayed
int scope;


bool keys[128]; //Holds value of key presses and releases

int angle; //angle of screen tilt in degrees
double angleR; //angle in radians

int person_size; //Size of the "player"

//Used in keeping track of playing location in array and checking for collisions
int sBoundary;
int hBoundary;

bool wall; //flag value for collisions

bool r_state;

int endx, endy;

void init() {
    map = Maze();
    p = Player();

    scale = GameInfo::scale;
    scope = GameInfo::scope;

    for (int i =0; i < 128; i++) {
        keys[i] = false;
    }

    angle = 0;

    hBoundary = ((scale-p.get_size()) / 2);

    r_state = false;

    cout << map.maze[10][10].get_wall() << endl;

    e = Enemy(10, 10);

    map.solve_maze(e.x, e.y, p.x, p.y);

    ////////////////////
}



/*
 * Calculate collisions
 */
void calcShift(int x, int y) {

    int temp1, temp2;

    wall = false;

    temp1 = rint(p.x_shift + ((x * cos(-angleR)) - (y * sin(-angleR))));
    temp2 = rint(p.y_shift + ((y * cos(-angleR)) + (x * sin(-angleR))));

    if (temp2 < (-hBoundary)) {

        if (temp1 > hBoundary) {
            if (map.maze[p.x + 1][p.y - 1].get_wall()) {

                p.x_shift = hBoundary;
                p.y_shift = -hBoundary;
                wall = true;
            }
        }

        else if (temp1 < (-hBoundary)) {
            if (map.maze[p.x + 1][p.y + 1].get_wall()) {

                p.x_shift = -hBoundary;
                p.y_shift = -hBoundary;
                wall = true;
            }
        }

        else if (map.maze[p.x+1][p.y].get_wall()) {

            p.y_shift = -hBoundary;
            wall = true;
        }
    }

    else if (temp1 > (hBoundary)) {

        if (temp2 > hBoundary) {
            if (map.maze[p.x - 1][p.y - 1].get_wall()) {

                p.x_shift = hBoundary;
                p.y_shift = hBoundary;
                wall = true;
            }
        }

        else if (map.maze[p.x][p.y-1].get_wall()) {

            p.x_shift = hBoundary;
            wall = true;
        }
    }

    else if (temp1 < (-hBoundary)) {

        if (temp2 > hBoundary) {
            if (map.maze[p.x - 1][p.y + 1].get_wall()) {
                p.x_shift = -hBoundary;
                p.y_shift = hBoundary;
                wall = true;
            }
        }

        else if (map.maze[p.x][p.y+1].get_wall()) {
            p.x_shift = -hBoundary;
            wall = true;
        }
    }

    else if (temp2 > (hBoundary)) {

        if (map.maze[p.x-1][p.y].get_wall()) {
            p.y_shift = hBoundary;
            wall = true;
        }
    }

    if (wall == false) {
        p.x_shift = temp1;
        p.y_shift = temp2;
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
    glViewport(0, 0, GameInfo::screen_width, GameInfo::screen_height);

    // do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, GameInfo::screen_width, GameInfo::screen_height, 0.0, -1.f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
            map.maze[i][j].draw(x,y,p.x_shift,p.y_shift,angleR);

            if ((e.x == i) && (e.y == j)) {

                e.draw(x,y);
            }

        }
    }

    p.draw(1,1); //Draw the player


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
        r_state = true; //Animated transition
    }

    glutPostRedisplay();

    return;
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


//make_unique<FiremansDaughter>(fire1)

void follow_path() {

    if (map.maze[e.x][e.y].get_correct_path()) {
        map.maze[e.x][e.y].set_correct_path(false);
    }

    if (map.maze[e.x+1][e.y].get_correct_path()) {
        e.y_shift -= e.get_speed();
    }

    else if (map.maze[e.x-1][e.y].get_correct_path()) {
        e.y_shift += e.get_speed();
    }

    else if (map.maze[e.x][e.y+1].get_correct_path()) {
        e.x_shift -= e.get_speed();
    }

    else if (map.maze[e.x][e.y-1].get_correct_path()) {
        e.x_shift += e.get_speed();
    }

    //else ()

    e.update();


}


void timer(int extra) {
    //slowly rotate for fun
    //angle = (angle + 1) % 360;
    //angleR = angle * (M_PI /180);

    if (r_state == false) {

        if (keys[GLUT_KEY_DOWN]) {
            calcShift(0, -p.get_speed());
        }

        if (keys[GLUT_KEY_LEFT]) {
            calcShift(p.get_speed(), 0);
        }

        if (keys[GLUT_KEY_UP]) {
            calcShift(0, p.get_speed());
        }

        if (keys[GLUT_KEY_RIGHT]) {
            calcShift(-p.get_speed(), 0);
        }

        p.update();

        map.solve_maze(e.x, e.y, p.x, p.y);
        follow_path();


    }

    else {

        angle = (angle + 3) % 360;
        angleR = angle * (M_PI /180);

        if (angle % 90 == 0) {
            r_state = 0;
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