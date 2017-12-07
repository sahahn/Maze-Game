//
// Created by sage on 11/30/17.
//

#include "Box.h"

Box::Box() {
    x = -1;
    y = -1;
    size = 1;
    
    hover = false;
    
    r,g,b = 0;

    empty = false;
}


Box::Box(int X, int Y, int s, double R, double G, double B) {
    x = X;
    y = Y;
    size = s;

    hover = false;
    
    r = R;
    g = G;
    b = B;

    empty = false;
}

bool Box::getE() const {
    return empty;
}

void Box::setE(bool b) {
    empty = b;
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
        glColor3f(static_cast<GLfloat>(r / 2), static_cast<GLfloat>(g / 2), static_cast<GLfloat>(b / 2));
    } else {
        glColor3f(static_cast<GLfloat>(r), static_cast<GLfloat>(g), static_cast<GLfloat>(b));
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


InputBox::InputBox() {

    x = -1;
    y = -1;

    width = 1;
    height = 1;

    hover = false;

    r,g,b = 0;

    empty = false;

    index = 0;

    ticks = 150;

}

InputBox::InputBox(int X, int Y, int w, int h, double R, double G, double B) {
    x = X;
    y = Y;

    width = w;
    height = h;

    hover = false;

    r = R;
    g = G;
    b = B;

    empty = false;

    index = -1;

    ticks = 150;

    //Start in active
    setActive(true);
}

bool InputBox::getActive() const {
    return active;
}

bool InputBox::getThere() const {
    return there;
}

void InputBox::setActive(bool b) {

    if ((b) && (!active)) {
        content = content + "|";
        index += 1;
        there = true;

    } else if ((!b) && (active)) {
        if (index != -1) {
            content.erase(static_cast<unsigned long>(index), 1);
            index -= 1;
        }
    }

    active = b;

}

void InputBox::addChar(char c) {
    if (index != -1) {
        content.erase(static_cast<unsigned long>(index), 1);
        content = content + c + "|";
    } else {
        content = content + c;
    }

    index += 1;
}

void InputBox::delChar() {
    if (index > 0) {
        content.erase(static_cast<unsigned long>(index - 1), 1);
        index -= 1;
    }
}

bool InputBox::checkHover(int X, int Y) {
    if ((X > x) && (X < x + width) && (Y > y) && (Y < y + height)) {

        hover = true;
        return true;

    } else {

        hover = false;
        return false;
    }
}

void InputBox::blinkCursor() {
    if (ticks > 0) {
        ticks--;
    } else {

        there = !there;

        ticks = 150;
    }
}


void InputBox::draw() const {

    glBegin(GL_QUADS);

    glColor3f(static_cast<GLfloat>(r), static_cast<GLfloat>(g), static_cast<GLfloat>(b));

    // top left corner
    glVertex2i(x, y);
    // top right corner
    glVertex2i(x + width, y);
    // bottom right corner
    glVertex2i(x + width, y + height);
    // bottom left corner
    glVertex2i(x, y + height);

    glEnd();
}


