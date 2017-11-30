//
// Created by sage on 10/30/17.
//


#include <iostream>
#include "Maze.h"
#include "EditorGraphics.h"

using namespace std;


Maze lev;
int w;
Point loc;
int xShift, yShift;

int moveSpeed;

int sBoundary;

//Holds value of key presses and releases
bool editor_keys[128];

int mX, mY;

bool fillVal;
bool clickTimer;
int ticks;

int level;

void initE() {

    //Call with false to tell it to use map editor
    lev = Maze(3,3);

    //Loads in center of the map
    loc.x = HEIGHT / 2;
    loc.y = WIDTH / 2;

    //Xshift and yShift are just to control the camera in map editor
    xShift = 0;
    yShift = 0;

    sBoundary = (EDITOR_SCALE/2);

    for (int i = 0; i < 128; i++) {
        editor_keys[i] = false;
    }

    //How fast the camera moves
    moveSpeed = 5;

    //Holds mouse position in the map array
    mX = 0;
    mY = 0;

    fillVal = false;
    clickTimer = false;
    ticks = 0;

    level = 1;
}

/* Initialize OpenGL Graphics */
void initGLE() {
    // Set "clearing" or background color
    glClearColor(0.1f, 0.1f, 1.0f, 1.0f); // Changed color to blue
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void displayE() {
    // tell OpenGL to use the whole window for drawing
    glViewport(0, 0, EDITOR_SCREEN_WIDTH, EDITOR_SCREEN_HEIGHT);

    // do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, EDITOR_SCREEN_WIDTH, EDITOR_SCREEN_HEIGHT, 0.0, -1.f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    //Simplified version of what is going on in the main graphics.cpp
    //Basically just keeps the editor within valid ranges, and renders parts of the screen
    int lb1 = loc.x - EDITOR_SCOPE;
    int upb1 = loc.x + EDITOR_SCOPE;
    int lb2 = loc.y - EDITOR_SCOPE;
    int upb2 = loc.y + EDITOR_SCOPE;

    int x = -1;

    for (int i = lb1; i <= upb1 + 1; i++, x++) {

        int y = -1;

        for (int j = lb2; j <= upb2 + 1; j++, y++) {

            lev.maze[i][j].draw(x,y,xShift,yShift);
        }
    }

    glFlush();  // Render now
}


void kbdE(unsigned char key, int x, int y) {


    switch (key) {

        case 's':
            editor_keys[GLUT_KEY_DOWN] = true;
            break;

        case 'a':
            editor_keys[GLUT_KEY_LEFT] = true;
            break;

        case 'd':
            editor_keys[GLUT_KEY_RIGHT] = true;
            break;

        case 'w':
            editor_keys[GLUT_KEY_UP] = true;
            break;
    }

    //Escape key
    if (key == 27) {
        glutDestroyWindow(w);

        if (level == -1) {
            cout << "Enter the level #" << endl;

            //Check for bad input
            while (!(cin >> level)) {
                cin.clear();
                string junk;
                cin >> junk;
                cout << "Invalid input, please re-enter level #" << endl;
            }
        }

        lev.saveLevel(level);

        exit(0);
    }
}

void kbuE(unsigned char key, int x, int y) {

    switch (key) {

        case 's':
            editor_keys[GLUT_KEY_DOWN] = false;
            break;

        case 'a':
            editor_keys[GLUT_KEY_LEFT] = false;
            break;

        case 'd':
            editor_keys[GLUT_KEY_RIGHT] = false;
            break;

        case 'w':
            editor_keys[GLUT_KEY_UP] = false;
            break;

        case '1':
            lev.maze[mX][mY].setStati(None);
            break;

        case '2':
            lev.clearStart();
            lev.maze[mX][mY].setStati(Start);
            break;

        case '3':
            lev.maze[mX][mY].setStati(End);
            break;

        case '4':
            lev.maze[mX][mY].setStati(FlipperSpawn);
            break;

        case '5':
            lev.maze[mX][mY].setStati(SizerSpawn);
            break;
    }
}


void kbdSE(int key, int x, int y) {

    switch (key) {

        case GLUT_KEY_DOWN:
            editor_keys[GLUT_KEY_DOWN] = true;
            break;

        case GLUT_KEY_LEFT:
            editor_keys[GLUT_KEY_LEFT] = true;
            break;

        case GLUT_KEY_RIGHT:
            editor_keys[GLUT_KEY_RIGHT] = true;
            break;

        case GLUT_KEY_UP:
            editor_keys[GLUT_KEY_UP] = true;
            break;
    }
    glutPostRedisplay();
}


void keyUpE(int key, int x, int y) {


    switch (key) {

        case GLUT_KEY_DOWN:
            editor_keys[GLUT_KEY_DOWN] = false;
            break;

        case GLUT_KEY_LEFT:
            editor_keys[GLUT_KEY_LEFT] = false;
            break;

        case GLUT_KEY_RIGHT:
            editor_keys[GLUT_KEY_RIGHT] = false;
            break;

        case GLUT_KEY_UP:
            editor_keys[GLUT_KEY_UP] = false;
            break;
    }
}


void cursorE(int x, int y) {

    mX = (loc.x - EDITOR_SCOPE + 1) + ((y - yShift)/EDITOR_SCALE);
    mY = (loc.y - EDITOR_SCOPE + 1) + ((x - xShift)/EDITOR_SCALE);
    glutPostRedisplay();
}

void mousemovE(int x, int y) {

    int tempX = (loc.x - EDITOR_SCOPE + 1) + ((y - yShift)/EDITOR_SCALE);
    int tempY = (loc.y - EDITOR_SCOPE + 1) + ((x - xShift)/EDITOR_SCALE);

    if (!((mX == tempX) && (mY == tempY))) {
        editor_keys[GLUT_LEFT_BUTTON] = true;
        mX = tempX;
        mY = tempY;
    }

    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouseE(int button, int state, int x, int y) {

    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {

        //To avoid erroneous double clicks
        if (!(clickTimer)) {

            clickTimer = true;
            editor_keys[GLUT_LEFT_BUTTON] = true;

            //For filling, only fill with the start value,
            //You fill with dragging cursorE w/ mouseE held down
            if (lev.maze[mX][mY].getWall()) {
                fillVal = true;
            } else {
                fillVal = false;
            }
        }
    }

    glutPostRedisplay();
}


void timerE(int extra) {


    //If click Timer, then increment
    //until timerE is out, used to prevent accidental double clicks
    if (clickTimer) {
        ticks++;

        if (ticks == 20) {

            ticks = 0;
            clickTimer = false;
        }

    } else {

        if ((editor_keys[GLUT_LEFT_BUTTON]) && (lev.maze[mX][mY].getWall() == fillVal)) {
            lev.maze[mX][mY].flipWall();
            editor_keys[GLUT_LEFT_BUTTON] = false;
        }
    }

    if (editor_keys[GLUT_KEY_DOWN]) {

        if ((loc.x + EDITOR_SCOPE  < HEIGHT)) {
            yShift -= moveSpeed;
        }
    }

    if (editor_keys[GLUT_KEY_LEFT]) {

        if ((loc.y - EDITOR_SCOPE) > 0) {
            xShift += moveSpeed;
        }

    }

    if (editor_keys[GLUT_KEY_UP]) {

        if ((loc.x - EDITOR_SCOPE) > 0) {
            yShift += moveSpeed;
        }

    }

    if (editor_keys[GLUT_KEY_RIGHT]) {

        if ((loc.y + EDITOR_SCOPE < WIDTH)) {
            xShift -= moveSpeed;
        }
    }

    if (yShift < (-sBoundary)) {
        loc.x +=1;
        yShift += EDITOR_SCALE;
    }

    if (xShift > (sBoundary)) {
        loc.y -= 1;
        xShift -= EDITOR_SCALE;
    }

    if (xShift < (-sBoundary)) {
        loc.y += 1;
        xShift += EDITOR_SCALE;
    }

    if (yShift > (sBoundary)) {
        loc.x -= 1;
        yShift -=EDITOR_SCALE;
    }


    glutPostRedisplay();
    glutTimerFunc(5, timerE, 0);
}



/* Main function: GLUT runs as a console application starting at main()  */
int graphicsPlayE(int argc, char** argv) {

    initE();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize(EDITOR_SCREEN_WIDTH, EDITOR_SCREEN_HEIGHT);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-EDITOR_SCREEN_WIDTH)/2,
                           (glutGet(GLUT_SCREEN_HEIGHT)-EDITOR_SCREEN_HEIGHT)/2);

    /* create the window and store the handle to it */
    w = glutCreateWindow("Map Editor");

    // Register callback handler for window re-paint event
    glutDisplayFunc(displayE);

    // Our own OpenGL initialization
    initGLE();

    //Register inputs
    glutKeyboardFunc(kbdE);
    glutKeyboardUpFunc(kbuE);
    glutSpecialFunc(kbdSE);
    glutSpecialUpFunc(keyUpE);

    //Turn key repeat mode to off
    glutSetKeyRepeat(0);

    // handles mouseE movement
    glutPassiveMotionFunc(cursorE);

    glutMotionFunc(mousemovE);

    // handles mouseE click
    glutMouseFunc(mouseE);

    // handles timerE
    glutTimerFunc(0, timerE, 0);

    // Enter the event-processing loop
    glutMainLoop();


    return 0;
}