//
// Created by sage on 11/6/17.
//


#include "GameInfo.h"

/*
 * Takes in an x and y, and an angle in radians and returns the location of the point rotated around the center
 */
Point cp(int x, int y, double Angle) {

    x-= (SCREEN_WIDTH / 2);
    y-= (SCREEN_HEIGHT / 2);

    double xP = ((x * cos(Angle)) - (y * sin(Angle)));
    double yP = ((y * cos(Angle)) + (x * sin(Angle)));

    xP += (SCREEN_WIDTH / 2);
    yP += (SCREEN_HEIGHT / 2);

    Point newPoint;
    newPoint.x = rint (xP);
    newPoint.y = rint (yP);

    return newPoint;
}
