//
// Created by sage on 11/6/17.
//

#include "Player.h"



Player::Player() {
    size = 20;
    speed = 20;
    location.x = (SCREEN_WIDTH / 2) - (size/2);
    location.y = (SCREEN_HEIGHT / 2) - (size/2);

    x = START_X;  //Starting location in the maze array
    y = START_Y;

}

void Player::draw() const {

    glColor3f(1, 1, 0);
    glBegin(GL_QUADS);
    // top left corner
    glVertex2i(location.x, location.y);
    // top right corner
    glVertex2i((location.x+size),location.y);
    // bottom right corner
    glVertex2i((location.x+size),(location.y+size));
    // bottom left corner
    glVertex2i(location.x, (location.y+size));

    glEnd();
}

int Player::getSize() const {
    return size;
}



