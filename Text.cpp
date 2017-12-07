//
// Created by Joseph Zieg on 12/1/17.
//

#include "Text.h"

#include <utility>

Text::Text(){
    text = "";
    loc = {-1, -1};
}

Text::Text(string text, Point loc){
    this->text = std::move(text);
    this->loc = loc;
}

void Text::draw() const{
    glColor3f(1, 1, 1);
    glRasterPos2d(loc.x, loc.y);
    for (char p : text) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, p);
    }
}

void Text::draw(int r, int g, int b, bool blink) const {
    glColor3f(r, g, b);
    glRasterPos2d(loc.x, loc.y);

    if (blink) {
        //Don't display the last character if blinking
        if (!text.empty()) {
            for (int p = 0; p < text.size() - 1; p++) {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text.at(static_cast<unsigned long>(p)));
            }
        }

    } else {
        for (char p : text) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, p);
        }
    }
}

void Text::setTextAndLoc(string text, Point loc){
    this->text = std::move(text);
    this->loc = loc;
}