#ifndef graphics_h
#define graphics_h
#include "Tile.h"
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

struct Point {   // Declare PERSON struct type
    int x;   // Declare member types
    int y;
};


void calcShift(int x, int y);

// Program initialization NOT OpenGL/GLUT dependent,
// as we haven't created a GLUT window yet
void init(void);

// Initialize OpenGL Graphics
void InitGL(void);

// Callback functions for GLUT */

// Draw the window - this is where all the GL actions are
void display(void);

// Trap and process alphanumeric keyboard events
void kbd(unsigned char key, int x, int y);

// Trap and process special keyboard events
void kbdS(int key, int x, int y);

// Handle "mouse cursor moved" events
void cursor(int x, int y);

//To handle drag function
void mousemov(int x, int y);

// Calls itself after a specified time
void timer(int extra);

// Handle mouse button pressed and released events
void mouse(int button, int state, int x, int y);

int graphicsPlay(int argc, char *argv[]);

void betterKeys();

//Internal function used in isInTriangle
float sign (Point p1, Point p2, Point p3);

//Check if a point x,y is within the global triangle points
bool isInTriangle (int x, int y);

#endif /* graphics_h */