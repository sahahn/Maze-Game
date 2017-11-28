//
// Created by Luke on 11/22/2017.
//

#include "Light.h"
#include "GameInfo.h"

bool wallCheck(double x, double y, Maze &m, int pX, int pY) {
    //cout << (int)(x/SCALE) << " - " << (int)(y/SCALE) << endl;

    int X = rint(y / SCALE);
    int Y = rint(x / SCALE);


    //If the block is out of sight, automatically treat it as a wall
    //Fixes bug with seeing one ray through a corner.
    if ((X < pX) && (Y < pY)) {
        if (m.maze[X+1][Y].getWall() && m.maze[X][Y+1].getWall()) {
            return false;
        }
    }

    else if ((X < pX) && (Y > pY)) {
        if (m.maze[X+1][Y].getWall() && m.maze[X][Y-1].getWall()) {
            return false;
        }
    }

    else if ((X > pX) && (Y > pY)) {

        if (m.maze[X-1][Y].getWall() && m.maze[X][Y-1].getWall()) {
            return false;
        }
    }

    else if ((X > pX) && (Y < pY)) {
        if (m.maze[X-1][Y].getWall() && m.maze[X][Y+1].getWall()) {
            return false;
        }
    }

    return !(m.maze[X][Y].getWall());
}

void Light::renderLight(int pX, int pY, int pXShift, int pYShift, Maze &m) const {
    double x, y;
    x = (pY) * SCALE + (-pXShift);
    y = (pX) * SCALE + (-pYShift);

    double rad = 350.0;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1, 1, 1);
    glVertex2i((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2));
    glColor3f(.05, .05, .05);
//    cout << x << " - " << y << endl;
    vector<int> tempRadArray;
    for (double i = 0; i <= 2.0 * 3.14159265 + 0.05; i += 2.0 * 3.14159265 / 360.0) {
        int distance = 0;
        while (wallCheck(x + distance * cos(i), y + distance * sin(i), m, pX, pY) && distance < rad) {
            distance += 10;
        }
        tempRadArray.push_back(distance);
    }

    //for (int i = 1; i < tempRadArray.size() - 1; i++) {
    //    if (tempRadArray[i] > (tempRadArray[i - 1] + tempRadArray[i + 1]) / 2) {
    //        tempRadArray[i] = (tempRadArray[i - 1] + tempRadArray[i + 1]) / 2;
    //    }
    //}

    int j = 0;
    for (double i = 0; i <= 2.0 * 3.14159265 + 0.05; i += 2.0 * 3.14159265 / 360.0) {
        int distance = tempRadArray[j];

        double color = 1 - (distance / rad);

        glColor3f(color,color,color);
        glVertex2i((SCREEN_WIDTH / 2) + distance * cos(i), (SCREEN_HEIGHT / 2) + distance * sin(i));
        j++;
    }
    glEnd();
}


Light::Light() {

}