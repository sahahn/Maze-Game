//
// Created by sage on 11/29/17.
//

#ifndef GRAPHICS_EDITORGRAPHICS_H
#define GRAPHICS_EDITORGRAPHICS_H

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

// Program initialization NOT OpenGL/GLUT dependent,
// as we haven't created a GLUT window yet
void initE(void);

// Initialize OpenGL Graphics
void InitGLE(void);

// Callback functions for GLUT */

// Draw the window - this is where all the GL actions are
void displayE(void);

// Trap and process alphanumeric keyboard events
void kbdE(unsigned char key, int x, int y);

// Trap and process special keyboard events
void kbdSE(int key, int x, int y);

// Handle "mouse cursor moved" events
void cursorE(int x, int y);

//To handle drag function
void mousemovE(int x, int y);

// Calls itself after a specified time
void timerE(int extra);

// Handle mouse button pressed and released events
void mouseE(int button, int state, int x, int y);

int graphicsPlayE(int argc, char *argv[]);


#endif //GRAPHICS_EDITORGRAPHICS_H
