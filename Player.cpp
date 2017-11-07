//
// Created by sage on 11/6/17.
//

#include "Player.h"



Player::Player() {
    size = 20;
    location.x = (GameInfo::screen_width / 2) - (size/2);
    location.y = (GameInfo::screen_height / 2) - (size/2);

    x = 50;
    y = 50;

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



