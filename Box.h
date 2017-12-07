//
// Created by sage on 11/30/17.
//

#ifndef GRAPHICS_BOX_H
#define GRAPHICS_BOX_H

#include "graphics.h"
#include "GameInfo.h"


class Box {
public:
    Box();
    Box(int X, int Y, int s, double R, double G, double B);
    int x, y, size;
    double r,g,b;

    //Getters+Setters for empty
    bool getE() const;
    void setE(bool b);

    bool hover;

    virtual bool checkHover(int X, int Y);

    virtual void draw() const;

    void randomMenuMovement();


protected:

    bool empty;

};

class InputBox : public Box {
public:
    InputBox();
    InputBox(int X, int Y, int w, int h, double R, double G, double B);
    bool checkHover(int X, int Y) override;
    void draw() const override;

    bool getActive() const;
    bool getThere() const;



    void setActive(bool b);

    void addChar(char c);
    void delChar();

    int width,height;
    int index;
    string content;

    void blinkCursor();


private:
    bool active;
    int ticks;
    bool there;
};


#endif //GRAPHICS_BOX_H
