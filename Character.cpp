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
    yShift = 10;


}


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
}

/*
 * Enemy Class
 */

Enemy::Enemy() {
    size = 20;
    speed = 15;

    x = 10;
    y = 10;

    xShift = 0;
    yShift = 0;

}


Enemy::Enemy(int X, int Y) {
    size = 20;
    speed = 10;

    x = X;
    y = Y;

    xShift = 0;
    yShift = 0;
}

Enemy::Enemy(int X, int Y, int s, int sp) {
    size = s;
    speed = sp;

    x = X;
    y = Y;

    xShift = 0;
    yShift = 0;
}


void Enemy::draw(int X, int Y, int pXShift, int pYShift, double angle) const {

    X = (X * SCALE) - yShift + pYShift;
    Y = (Y * SCALE) - xShift + pXShift;

    X += (SCALE/2) - (size/2);
    Y += (SCALE/2) - (size/2);

    glBegin(GL_QUADS);
    glColor3f(0, 1, .5);

    if (angle == 0) {

        // top left corner
        glVertex2i(Y, X);
        // top right corner
        glVertex2i((Y + size), X);
        // bottom right corner
        glVertex2i((Y + size), (X + size));
        // bottom left corner
        glVertex2i(Y, (X + size));
    }


    else {

        Point p1,p2,p3,p4;
        p1 = cp(Y,X,angle);
        p2 = cp((Y+size),X,angle);
        p3 = cp((Y+size),(X+size),angle);
        p4 = cp(Y,(X+size),angle);

        glVertex2i(p1.x,p1.y);
        glVertex2i(p2.x,p2.y);
        glVertex2i(p3.x,p3.y);
        glVertex2i(p4.x,p4.y);
    }



    glEnd();

}





