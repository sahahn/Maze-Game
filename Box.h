//
// Created by sage on 11/30/17.
//

#ifndef GRAPHICS_BOX_H
#define GRAPHICS_BOX_H

#include "graphics.h"
#include "GameInfo.h"


class Box {
public:

    //Default constructor, not used
    Box();

    /**
     *
     * @param X - int starting x value of top left corner
     * @param Y - int starting y value of top left corner
     * @param s - int size value
     * @param R - red value
     * @param G - green value
     * @param B - blue value
     */
    Box(int X, int Y, int s, double R, double G, double B);

    //Box fields
    int x, y, size;
    double r,g,b;

    //Stores if the mouse is over the box or not
    bool hover;

    //Getters+Setters for empty
    bool getE() const;
    void setE(bool b);

    /**
     * Check if the mouse is over the box
     * @param X - int of x loc
     * @param Y - int of y loc
     * @return - returns true if mouse is within box
     */
    virtual bool checkHover(int X, int Y);

    //Draws the box, overriden in InputBox
    virtual void draw() const;

    void randomMenuMovement();


protected:
    bool empty;

};

class InputBox : public Box {
public:

    //Default constructor for inputBox, not used
    InputBox();

    /**
     * Non-default constructor
     * @param X - int x location of top left corner
     * @param Y - int y location of top left corner
     * @param w - int width of input box
     * @param h - int height of input box
     * @param R - double red value
     * @param G - double green value
     * @param B - double blue value
     */
    InputBox(int X, int Y, int w, int h, double R, double G, double B);

    //Input box uses width and height instead of size
    int width,height;
    int index;
    string content;

    /**
     * Checks InputBox to see if mouse is over it
     * @param X - int x location of mouse
     * @param Y - int y location of mouse
     * @return - bool if over input box or not
     */
    bool checkHover(int X, int Y) override;

    void draw() const override;

    bool getActive() const;
    bool getThere() const;

    /**
     * Sets InputBox to active/ready for input
     * @param b - bool for active or not
     */
    void setActive(bool b);

    /**
     * Adds a character to the InputBox's string content
     * @param c - char c to add
     */
    void addChar(char c);

    /*
     * Deletes a character from the content, triggered w/ backspace
     */
    void delChar();

    /*
     * An aesthetic function to make the cursor blink
     */
    void blinkCursor();


private:
    bool active;
    int ticks;
    bool there;
};


#endif //GRAPHICS_BOX_H
