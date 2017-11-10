//
// Created by sage on 11/10/17.
//

#include "Character.h"



/*
 * Character Abstract Class
 */


int Character::get_size() const {
    return size;
}

int Character::get_speed() const {
    return speed;
}

void Character::update() {
    if (y_shift < (-sBoundary)) {
        x += 1;
        y_shift += GameInfo::scale;
    }

    if (x_shift > (sBoundary)) {
        y -= 1;
        x_shift -= GameInfo::scale;
    }

    if (x_shift < (-sBoundary)) {
        y += 1;
        x_shift += GameInfo::scale;
    }

    if (y_shift > (sBoundary)) {
        x -= 1;
        y_shift -= GameInfo::scale;
    }
}

/*
 * Player Class
 */
Player::Player() {
    size = 20;
    speed = 20;
    location.x = (GameInfo::screen_width / 2) - (size/2);
    location.y = (GameInfo::screen_height / 2) - (size/2);

    x = GameInfo::start_x;  //Starting location in the maze array
    y = GameInfo::start_y;

    x_shift = 0;
    y_shift = 0;


}

Player::Player(int X, int Y) {
    size = 20;
    speed = 20;
    location.x = (GameInfo::screen_width / 2) - (size/2);
    location.y = (GameInfo::screen_height / 2) - (size/2);

    x = X;  //Starting location in the maze array
    y = Y;

    x_shift = 0;
    y_shift = 0;
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

    x_shift = 0;
    y_shift = 0;

}


Enemy::Enemy(int X, int Y) {
    size = 20;
    speed = 10;

    location.x = 1;
    location.y = 1;

    x = X;
    y = Y;

    x_shift = 0;
    y_shift = 0;
}


void Enemy::draw(int X, int Y) const {

    X = (X * GameInfo::scale) - y_shift;
    Y = (Y * GameInfo::scale) - x_shift;

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





