//
// Created by sage on 11/10/17.
//

#include "Character.h"



/*
 * Character Abstract Class
 */


int Character::getSize() const {
    return size;
}

int Character::getSpeed() const {
    return speed;
}

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
    speed = 20;
    location.x = (SCREEN_WIDTH / 2) - (size/2);
    location.y = (SCREEN_HEIGHT / 2) - (size/2);

    x = START_X;  //Starting location in the maze array
    y = START_Y;

    xShift = 0;
    yShift = 0;


}

Player::Player(int X, int Y) {
    size = 20;
    speed = 20;
    location.x = (SCREEN_WIDTH / 2) - (size/2);
    location.y = (SCREEN_HEIGHT / 2) - (size/2);

    x = X;  //Starting location in the maze array
    y = Y;

    xShift = 0;
    yShift = 0;
}


void Player::draw(int X, int Y) const {

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
}

/*
 * Enemy Class
 */

Enemy::Enemy() {
    size = 20;
    speed = 10;

    location.x = 1;
    location.y = 1;

    x = 60;
    y = 60;

    xShift = 0;
    yShift = 0;

}


Enemy::Enemy(int X, int Y) {
    size = 20;
    speed = 10;

    location.x = 1;
    location.y = 1;

    x = X;
    y = Y;

    xShift = 0;
    yShift = 0;
}


void Enemy::draw(int X, int Y) const {

    X = (X * SCALE) - yShift;
    Y = (Y * SCALE) - xShift;

    X -= (size/2);
    Y -= (size/2);

    glBegin(GL_QUADS);
    glColor3f(0, 1, .5);
    // top left corner
    glVertex2i(Y, X);
    // top right corner
    glVertex2i((Y + size), X);
    // bottom right corner
    glVertex2i((Y + size), (X + size));
    // bottom left corner
    glVertex2i(Y, (X + size));
    glEnd();


    glEnd();
}





