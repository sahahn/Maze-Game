//
// Created by Luke on 11/22/2017.
//

#ifndef PROJECT6_CS120_LIGHT_H
#define PROJECT6_CS120_LIGHT_H

#include "graphics.h"
#include "Maze.h"

class Light {
public:

    /**
     *
     * This function takes in data about the maze and player and renders light around the player,
     * ray casting to walls.
     * TODO: this function could use optimization, and refactoring to use more variables passed down instead of hard coded values
     *
     * @param pX - players x tile in maze
     * @param pY - players y tile in maze
     * @param pXShift - x shift in pixels from the center of the maze tile
     * @param pYShift - y shift in pixels from the center of the maze tile
     * @param m - maze object
     * @param angle - angle to render the maze at
     */
    void renderLight(int pX, int pY, int pXShift, int pYShift, Maze &m, int angle) const;

    /**
     * radius setter
     * @param r - desired radius
     */
    void setRadius(int r);

    /**
     * radius getter
     * @return current radius
     */
    int getRadius() const;

    Light();

private:
    int radius;
};
#endif //PROJECT6_CS120_LIGHT_H
