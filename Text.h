//
// Created by Joseph Zieg on 12/1/17.
//

#ifndef INC_43U_JH_TEXT_H
#define INC_43U_JH_TEXT_H

#include <string>
#include "GameInfo.h"
#include "graphics.h"

using namespace std;

class Text {

public:
    Text();

    Text(string text, Point loc);

    void draw() const;

    void draw(int r, int g, int b, bool blink) const;

    void setTextAndLoc(string text, Point loc);

private:
    string text;
    Point loc;

};


#endif //INC_43U_JH_TEXT_H
