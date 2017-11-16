//
// Created by sage on 11/10/17.
//

#include "Character.h"
#include <iostream>

/*
 * Character Abstract Class
 */


int Character::getSize() const {
    return size;
}

int Character::getSpeed() const {
    return speed;
}

//Sets the characters location on the screen, used mostly for enemy,
//but could be used for the Player for some sort of end game animation.
void Character::setLocation(int xL, int yL) {
    location.x = xL;
    location.y = yL;
}

//The update function is used by Player and Enemies, and acts by checking to see if
//after a movement, the Character has moved into a different x,y location within the map,
//and subsequently updates either the x or y, along with the xShift and yShift to reflect this change.
void Character::update() {
    if (yShift < (-sBoundary)) {
        x += 1;
        yShift += SCALE;
    }

    if (xShift > (sBoundary)) {
        y -= 1;
        xShift -= SCALE;
    }

    if (xShift < (-sBoundary)) {
        y += 1;
        xShift += SCALE;
    }

    if (yShift > (sBoundary)) {
        x -= 1;
        yShift -= SCALE;
    }
}

/*
 * Player Class
 */
Player::Player() {
    size = 20;
    speed = 2;

    //Set the player to be centered on the screen
    location.x = (SCREEN_WIDTH / 2) - (size/2);
    location.y = (SCREEN_HEIGHT / 2) - (size/2);

    //Set the starting location in the maze array, as defined in GameInfo
    x = START_X;
    y = START_Y;

    xShift = 0;
    yShift = 0;

    //hBoundary, or Hard Boundary is used in doMove, and reflects the size of the Character
    hBoundary = ((SCALE-size) / 2);
    playerRotation = 0;
}

//Draws the player, NOTE: although enemy also has a draw function, it requires different parameters
//so I did not think there was a use in creating a virtual function to override.
void Player::draw() const {


    glBegin(GL_QUADS);
    glColor3f(1, 1, 0);
    // top left corner
    glVertex2i(location.x, location.y);
    // top right corner
    glVertex2i((location.x+size),location.y);
    // bottom right corner
    glVertex2i((location.x+size),(location.y+size));
    // bottom left corner
    glVertex2i(location.x, (location.y+size));

    glEnd();

    //If we want to make the player rotate  uncomment this code!
    /*
    glBegin(GL_QUADS);
    glColor3f(1, 1, 0);

    Point p1,p2,p3,p4;

    p1 = rotate(location.x, location.y, playerRotation);
    p2 = rotate((location.x+size),location.y, playerRotation);
    p3 = rotate((location.x+size),(location.y+size), playerRotation);
    p4 = rotate(location.x, (location.y+size), playerRotation);

    glVertex2i(p1.x, p1.y); //TL
    glVertex2i(p2.x, p2.y); //TR
    glVertex2i(p3.x, p3.y); //BR
    glVertex2i(p4.x, p4.y); //BL

    glEnd(); */
}

//calcMove takes in an xDelta, and yDelta along with the current angle in radians.
//The purpose of the function is to calculate the corrected movement for the player,
//such that even if the map is rotated the up arrow, for example, will always make the player move up.
//So instead of xDelta 10, yDelta 0, for up, it adds xDelta 10 shifted in the opposite direction,
//to compensate for the later applied rotation.
void Player::calcMove(int xDelta, int yDelta, double angleR) {

    temp1 = rint(xShift + ((xDelta * cos(-angleR)) - (yDelta * sin(-angleR))));
    temp2 = rint(yShift + ((yDelta * cos(-angleR)) + (xDelta * sin(-angleR))));
}

void Player::setPlayerRotation(double angR) {
    playerRotation = angR;
}

/*
 * Enemy Class
 */

Enemy::Enemy() {
    size = 20;
    speed = 1;

    //The default "spawning location" is set to 10,10 for now, as it is easier to test
    x = 10;
    y = 10;

    xShift = 0;
    yShift = 0;

    //hBoundary, same as with Player, must be calculated depending on size.
    hBoundary = ((SCALE-size) / 2);

}

Enemy::Enemy(int X, int Y, int s, int sp) {
    size = s;
    speed = sp;

    x = X;
    y = Y;

    xShift = 0;
    yShift = 0;

    //hBoundary, same as with Player, must be calculated depending on size.
    hBoundary = ((SCALE-size) / 2);
}


//Note: The draw function for the Enemy, and Tile piece are quite close.
//In order to draw the Enemy relative to the Player, the players xShift and yShift,
//along with the current angle in radians is needed.
void Enemy::draw(int pXShift, int pYShift, double angleR) const {

    //Temp values used in calculation
    int X, Y;

    //Note here, the location.x and location.y must be set prior to draw being called,
    //within the main graphics function. Where location.x and location.y refer to
    //which grid on the screen the enemy will be drawn.
    //Further the location must be brought to scale, and set to reflect
    //both the Enemy being drawns x and y shifts and the players.
    X = (location.x * SCALE) - yShift + pYShift;
    Y = (location.y * SCALE) - xShift + pXShift;

    //Next set X and Y to reflect the top left corner of the Enemy.
    X += (SCALE/2) - (size/2);
    Y += (SCALE/2) - (size/2);

    glBegin(GL_QUADS);

    //This is uh some sort of weird green, I've just been putting random colors to be honest...
    glColor3f(0, 1, .5);

    //If no rotation, no use in calculating shifted locations...
    //though this optimization most likely is not needed, we can delete it, I don't care.
    if (angleR == 0) {

        //Note Y is called as x, and X as y, this is due to the conversion between array x and y, and pixel x and y.
        //It's annoying, until you just make the swap here and don't think about it any more.


        glVertex2i(Y, X);                   //TL
        glVertex2i((Y + size), X);          //TR
        glVertex2i((Y + size), (X + size)); //BR
        glVertex2i(Y, (X + size));          //BL
    }

    else {

        //If the map is currently rotated, call the global rotate function, which returns rotated points
        Point p1,p2,p3,p4;

        p1 = rotate(Y, X, angleR);
        p2 = rotate((Y + size), X, angleR);
        p3 = rotate((Y + size),(X + size), angleR);
        p4 = rotate(Y ,(X + size), angleR);

        glVertex2i(p1.x, p1.y); //TL
        glVertex2i(p2.x, p2.y); //TR
        glVertex2i(p3.x, p3.y); //BR
        glVertex2i(p4.x, p4.y); //BL
    }

    glEnd();
}

//Like the player calcMove, but no need to adjust for rotation,
//simply sets the temp values.
void Enemy::calcMove(int xDelta, int yDelta, double angleR) {
    temp1 = xShift + xDelta;
    temp2 = yShift + yDelta;
}

void Enemy::resetLoc() {
    location.x = -2;
    location.y = -2;
}






