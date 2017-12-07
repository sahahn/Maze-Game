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

void Character::setSize(int sizeIn) {
    size = (sizeIn > 0) ? sizeIn : size;

    hBoundary = ((SCALE - size) / 2);
}

void Character::setSpeed(int speedIn) {
    speed = (speedIn > 0) ? speedIn : speed;
}

//Sets the characters location on the screen, used mostly for enemy,
//but could be used for the Player for some sort of end game animation.
void Character::setLocation(int xL, int yL) {
    location.x = xL;
    location.y = yL;
}

Point Character::getLocation() {
    return location;
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

//Do move contains the important game logic for the player and enemies, for movement and collisions,
//calcMove must be called beforehand which sets the temp values to the characters intended move.
//doMove returns True if the move was valid, and False if there was a wall.
bool Character::doMove(Maze &m) {

    //There are eight possible collisions to check for in a grid based system.
    //The following logic checks all eight, and reacts accordingly to which one was triggered
    if (temp2 < (-hBoundary)) {
        if (temp1 > hBoundary) {
            if (m.maze[x + 1][y - 1].getWall()) {

                xShift = hBoundary;
                yShift = -hBoundary;
                return false;
            }
        }

        if (temp1 < (-hBoundary)) {
            if (m.maze[x + 1][y + 1].getWall()) {

                xShift = -hBoundary;
                yShift = -hBoundary;
                return false;
            }
        }

        if (m.maze[x + 1][y].getWall()) {

            yShift = -hBoundary;
            return false;
        }
    }

    if (temp1 > (hBoundary)) {

        if (temp2 > hBoundary) {
            if (m.maze[x - 1][y - 1].getWall()) {

                xShift = hBoundary;
                yShift = hBoundary;
                return false;
            }
        }

        if (m.maze[x][y - 1].getWall()) {

            xShift = hBoundary;
            return false;
        }

    }

    if (temp1 < (-hBoundary)) {

        if (temp2 > hBoundary) {

            if (m.maze[x - 1][y + 1].getWall()) {
                xShift = -hBoundary;
                yShift = hBoundary;
                return false;
            }
        }

        if (m.maze[x][y + 1].getWall()) {
            xShift = -hBoundary;
            return false;
        }
    }

    if (temp2 > (hBoundary)) {

        if (m.maze[x - 1][y].getWall()) {
            yShift = hBoundary;
            return false;
        }
    }

    //If it reaches this point, there is no wall, all cases were checked for,
    //proceed with the valid movement
    xShift = temp1;
    yShift = temp2;

    return true;
}

void Character::updateVelocity(int x, int y) {
    currentVelocity.x = x;
    currentVelocity.y = y;
}

DoublePoint Character::getVelocity() const {
    return currentVelocity;
}

DoublePoint Character::getMovementBuffer() const {
    return velocityMovementBuffer;
}

void Character::taperXVelocity() {
    currentVelocity.x /= 1.02;
    if (velocityMovementBuffer.x > 1 || velocityMovementBuffer.x < -1) {
        velocityMovementBuffer.x = 0;
    }
    velocityMovementBuffer.x += currentVelocity.x;
}

void Character::taperYVelocity() {
    currentVelocity.y /= 1.02;
    if (velocityMovementBuffer.y > 1 || velocityMovementBuffer.y < -1) {
        velocityMovementBuffer.y = 0;
    }
    velocityMovementBuffer.y += currentVelocity.y;
}

void Character::flipVelocity() {
    currentVelocity.x = -currentVelocity.x;
    currentVelocity.y = -currentVelocity.y;
    velocityMovementBuffer.x = -velocityMovementBuffer.x;
    velocityMovementBuffer.y = -velocityMovementBuffer.y;
}

/*
 * Player Class
 */

Player::Player() {

}
Player::Player(int sX, int sY) {
    size = 30;
    speed = 2;
    playerLight = Light();

    defaultLightRadius = 300;
    lightRadius = defaultLightRadius;
    playerLight.setRadius(lightRadius);

    // constantly updated speed of player.
    currentVelocity = DoublePoint{0, 0};
    velocityMovementBuffer = DoublePoint{0, 0};

    //Set the player to be centered on the screen
    location.x = (SCREEN_WIDTH / 2);
    location.y = (SCREEN_HEIGHT / 2);

    //Set the starting location in the maze array, as defined in GameInfo
    x = sX;
    y = sY;

    xShift = 0;
    yShift = 0;

    //hBoundary, or Hard Boundary is used in doMove, and reflects the size of the Character
    hBoundary = ((SCALE - size) / 2);
    playerRotation = 0;
}

//Draws the player, NOTE: although enemy also has a draw function, it requires different parameters
//so I did not think there was a use in creating a virtual function to override.
void Player::draw() const {

    int X, Y;

    X = location.x - (size / 2);
    Y = location.y - (size / 2);


    glBegin(GL_QUADS);
    glColor3f(1, 1, 0);
    // top left corner
    glVertex2i(X, Y);
    // top right corner
    glVertex2i(X + size, Y);
    // bottom right corner
    glVertex2i(X + size, Y + size);
    // bottom left corner
    glVertex2i(X, Y + size);

    glEnd();


    //If we want to make the player rotate  uncomment this code!
    /*
    glBegin(GL_QUADS);
    glColor3f(1, 1, 0);

    Point p1,p2,p3,p4;

    p1 = rotate(X, Y, playerRotation);
    p2 = rotate((X+size),Y, playerRotation);
    p3 = rotate((X+size),(Y+size), playerRotation);
    p4 = rotate(X, (Y+size), playerRotation);

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

    temp1 = (int) rint(xShift + ((xDelta * cos(-angleR)) - (yDelta * sin(-angleR))));
    temp2 = (int) rint(yShift + ((yDelta * cos(-angleR)) + (xDelta * sin(-angleR))));
}

double Player::getPlayerRotation() const {
    return playerRotation;
}

int Player::getDLightRadius() const {
    return defaultLightRadius;
}

void Player::setPlayerRotation(double angR) {
    playerRotation = angR;
}

void Player::setDLightRadius(int l) {
    defaultLightRadius = l;

    calcNewLight(defaultLightRadius);
}

//Change player light radius accordingly
void Player::updateLight() {

    if (lightRadius < (defaultLightRadius * 2)) {
        playerLight.setRadius(lightRadius);
    }

    //Reset lightRadius back to default
    lightRadius = defaultLightRadius;
}

//Called before light radius, e.g. if there are multiple scary things
void Player::calcNewLight(int d) {

    //If d is less then the current lightRadius, set the lightRadius to d
    if (d < lightRadius) {
        lightRadius = d;
    }
}

/*
 * Enemy Class
 */

Enemy::Enemy() {
    //Default constructor isn't used
}

Enemy::Enemy(int X, int Y, eType e) {
    spawnBuffer = 1;

    spawnX = X;
    spawnY = Y;

    x = spawnX;
    y = spawnY;

    xShift = 0;
    yShift = 0;

    //hBoundary, same as with Player, must be calculated depending on size.
    hBoundary = ((SCALE - size) / 2);

    type = e;

    //Could have these dep. on type
    size = 20; //Default size
    speed = 1; //Default speed

    //Update rate is determined by type
    switch (type) {

        case (Sizer) : {
            updateRate = 0;
            break;
        }

        case (Flipper) : {
            updateRate = 1;
            break;
        }

        case (ScaryThing) : {
            updateRate = 2;
            size = 50;
            break;
        }
    }
}

eType Enemy::getType() const {
    return type;
}

int Enemy::getSpawnX() const {
    return spawnX;
}

int Enemy::getSpawnY() const {
    return spawnY;
}

int Enemy::getUpdateRate() const {
    return updateRate;
}

int Enemy::getSpawnBuffer() const {
    return spawnBuffer;
}

void Enemy::setType(eType e) {
    type = e;
}

void Enemy::setSpawnX(int x) {
    spawnX = x;
}

void Enemy::setSpawnY(int y) {
    spawnY = y;
}

void Enemy::setUpdateRate(int u) {
    updateRate = u;
}

void Enemy::setSpawnBuffer(int s) {
    spawnBuffer = s;
}

bool Enemy::doBuffer() {
    if (spawnBuffer > 0) {
        spawnBuffer--;
        return false;
    } else {
        return true;
    }
}

double Enemy::getDistance(Point center) const {
    return sqrt(pow((center.x - (SCREEN_WIDTH / 2)), 2) + pow((center.y - (SCREEN_HEIGHT / 2)), 2));
}


//Note: The draw function for the Enemy, and Tile piece are quite close.
//In order to draw the Enemy relative to the Player, the players xShift and yShift,
//along with the current angle in radians is needed.
void Enemy::draw(Player &p, double angleR) const {

    //Only draw if not buffering
    if (!spawnBuffer > 0) {

        int pXShift = p.xShift;
        int pYShift = p.yShift;
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
        X += (SCALE / 2) - (size / 2);
        Y += (SCALE / 2) - (size / 2);

        glBegin(GL_QUADS);

        Point p1, p2, p3, p4, center;

        p1 = rotate(Y, X, angleR);
        p2 = rotate((Y + size), X, angleR);
        p3 = rotate((Y + size), (X + size), angleR);
        p4 = rotate(Y, (X + size), angleR);

        center.x = (p1.x + p2.x) / 2;
        center.y = (p1.y + p4.y) / 2;

        double distance = getDistance(center);

        int playerLightRadius = p.playerLight.getRadius() + 10; // +10 so that you can see it a little better

        //Color is based on enemy type
        switch (type) {

            case (Flipper):
                glColor3f(.3 - (distance / playerLightRadius), .7 - (distance / playerLightRadius),
                          .4 - (distance / playerLightRadius));
                break;

            case (Sizer):
                glColor3f(.7 - (distance / playerLightRadius), .4 - (distance / playerLightRadius),
                          .8 - (distance / playerLightRadius));
                break;

            case (ScaryThing) :
                //If scary thing then do the players calcDistance
                p.calcNewLight((int) rint(distance / 2));

                glColor3f(.1, .1, .1);
                break;

            default:
                glColor3f(1, 1, 1);
                break;
        }

        glVertex2i(p1.x, p1.y); //TL
        glVertex2i(p2.x, p2.y); //TR
        glVertex2i(p3.x, p3.y); //BR
        glVertex2i(p4.x, p4.y); //BL


        glEnd();
    }
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


void Enemy::nextCalc(int nX, int nY) {
    //Move Right
    if (nX > x) {
        //First calculate move, then doMove, same for all movement.
        calcMove(0, -speed, 0);
    }

        //Move Left
    else if (nX < x) {
        calcMove(0, speed, 0);
    }

        //Move Up
    else if (nY > y) {
        calcMove(-speed, 0, 0);
    }

        //Move Down
    else if (nY < y) {
        calcMove(speed, 0, 0);
    }
}








