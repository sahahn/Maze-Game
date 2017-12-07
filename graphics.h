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


// Program initialization NOT OpenGL/GLUT dependent,
// as we haven't created a GLUT window yet
void init(void);

// Initialize OpenGL Graphics
void InitGL();

void kbd(unsigned char key, int x, int);

void kbu(unsigned char key, int x, int y);

void kbdS(int key, int x, int y);

void keyUp(int key, int x, int y);

void cursor(int x, int y);

void mousemov(int x, int y);

void mouse(int button, int state, int x, int y);

void display();

void timer(int extra);

int graphicsPlay(int argc, char *argv[]);

#endif /* graphics_h */