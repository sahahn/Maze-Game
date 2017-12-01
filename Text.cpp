//
// Created by Joseph Zieg on 12/1/17.
//

#include "Text.h"

Text::Text(){
    text = "default";
    loc = {100, 100};
}

Text::Text(string text, Point loc){
    this->text = text;
    this->loc = loc;
}

void Text::draw(){
    glColor3f(1, 1, 1);
    glRasterPos2d(loc.x, loc.y);
    for (int p = 0; p < text.size(); p++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text.at(p));
    }
}

void Text::setTextAndLoc(string text, Point loc){
    this->text = text;
    this->loc = loc;
}