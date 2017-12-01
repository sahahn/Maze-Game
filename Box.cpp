//
// Created by sage on 11/30/17.
//

#include "Box.h"

Box::Box() {
    x = 300;
    y = 300;
    size = 50;
    
    hover = false;
    
    r,g,b = 0;
}

Box::Box(int X, int Y, int s, double R, double G, double B) {
    x = X;
    y = Y;
    size = s;

    hover = false;
    
    r = R;
    g = G;
    b = B;
}

bool Box::checkHover(int X, int Y) {
    if ((X > x) && (X < x + size) && (Y > y) && (Y < y + size)) {
        
        hover = true;
        return true;

    } else {
        
        hover = false;
        return false;
    }
}

void Box::draw() const {
    glBegin(GL_QUADS);
    
    if (hover) {
        glColor3f(r/2, g/2, b/2);
    } else {
        glColor3f(r, g, b);
    }

    // top left corner
    glVertex2i(x, y);
    // top right corner
    glVertex2i(x + size, y);
    // bottom right corner
    glVertex2i(x + size, y + size);
    // bottom left corner
    glVertex2i(x, y + size);

    glEnd();
    
}

void Box::randomMenuMovement() {
    int randMoveX = rand() % 2;
    int randMoveY = rand() % 2;

    int randN = rand() % 2;

    if (((x + randMoveX) < SCREEN_WIDTH) && (randN == 1)) {
        x += randMoveX;
    }
    else if (((x - randMoveX) > 0) && (randN == 0)) {
        x -= randMoveX;
    }

    if (((y + randMoveY) < SCREEN_HEIGHT) && (randN == 1)) {
        y += randMoveY;
    }

    else if (((y - randMoveY) > 0) && (randN == 0)) {
        y -= randMoveY;
    }
}


