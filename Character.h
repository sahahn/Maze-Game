//
// Created by sage on 11/10/17.
//

#ifndef GRAPHICS_CHARACTER_H
#define GRAPHICS_CHARACTER_H

#include "graphics.h"
#include "GameInfo.h"
#include "Light.h"

enum eType {
    Flipper = 4, Sizer = 5
};

class Character {
public:

    //Location in the maze array
    int x;
    int y;

    //Shifted position within the location
    int xShift;
    int yShift;

    // Getters
    int getSize() const;

    int getSpeed() const;

    // Getters
    void setSize(int sizeIn);

    void setSpeed(int speedIn);


    //Sets the characters location on the screen, used mostly for enemy,
    //but could be used for the Player for some sort of end game animation.
    void setLocation(int xL, int yL);

    Point getLocation();

    //The update function is used by Player and Enemies, and acts by checking to see if
    //after a movement, the Character has moved into a different x,y location within the map,
    //and subsequently updates either the x or y, along with the xShift and yShift to reflect this change.
    void update();

    // Our abstract method that is overriden in both player and enemy child classes
    virtual void calcMove(int xDelta, int yDelta, double angleR) = 0;

    bool doMove(Maze &m);

    void updateVelocity(int x, int y);

    DoublePoint getVelocity() const;

    DoublePoint getMovementBuffer() const;

    void taperXVelocity();

    void taperYVelocity();

    void flipVelocity();

protected:

    int size;
    int speed;

    //Temp values for intended movement, i.e. only
    //updated to xShift, yShift if a valid movement.
    int temp1;
    int temp2;

    //The "Hard Boundary" specific to each character, used in collision detection.
    int hBoundary;

    static const int sBoundary = (SCALE / 2);
    Point location; //Location on screen

    // These values are used when calculating velocity of the player.
    // Velocity movement buffer allow us to move the player less than a pixel
    // every time the time function is called in the graphics.cpp file, by remembering
    // the last time you moved and how much you should have moved since then.
    DoublePoint currentVelocity;
    DoublePoint velocityMovementBuffer;

};


class Player : public Character {
public:
    Light playerLight;

    // Constructor
    Player();
    Player(int sX, int sY);

    //Draws the player, NOTE: although enemy also has a draw function, it requires different parameters
    //so I did not think there was a use in creating a virtual function to override.
    void draw() const;

    //calcMove takes in an xDelta, and yDelta along with the current angle in radians.
    //The purpose of the function is to calculate the corrected movement for the player,
    //such that even if the map is rotated the up arrow, for example, will always make the player move up.
    //So instead of xDelta 10, yDelta 0, for up, it adds xDelta 10 shifted in the opposite direction,
    //to compensate for the later applied rotation.
    void calcMove(int xDelta, int yDelta, double angleR) override;

    // getter and setter for rotation
    double getPlayerRotation() const;

    void setPlayerRotation(double angR);


private:
    double playerRotation;
};

class Enemy : public Character {
public:
    // Constructors
    Enemy();

    Enemy(int X, int Y, eType e);

    // getter
    eType getType() const;
    int getSpawnX() const;
    int getSpawnY() const;
    int getUpdateRate() const;

    // setter
    void setType(eType e);
    void setSpawnX(int x);
    void setSpawnY(int y);
    void setUpdateRate(int u);

    //Note: The draw function for the Enemy, and Tile piece are quite close.
    //In order to draw the Enemy relative to the Player, the players xShift and yShift,
    //along with the current angle in radians is needed.
    void draw(Player &p, double angleR) const;

    //Like the player calcMove, but no need to adjust for rotation,
    //simply sets the temp values.
    void calcMove(int xDelta, int yDelta, double angleR) override;


    void nextCalc(int nX, int nY);

    // Sets location to a default
    void resetLoc();


private:
    int spawnX, spawnY;
    int updateRate;

    eType type;


};


#endif //GRAPHICS_CHARACTER_H
