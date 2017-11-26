//
// Created by Luke on 11/22/2017.
//

#include "Light.h"
#include "GameInfo.h"

bool wallCheck(double x, double y, Maze &m) {
    //cout << (int)(x/SCALE) << " - " << (int)(y/SCALE) << endl;
    return !(m.maze[(int) (y / SCALE + 0.5)][(int) (x / SCALE + 0.5)].getWall());
}

void Light::renderLight(int x, int y, Maze &m) const {
    double rad = 450.0;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1, 1, 1);
    glVertex2i(500, 500);
    glColor3f(0, 0, 0);
//    cout << x << " - " << y << endl;
    vector<int> tempRadArray;
    for (double i = 0; i <= 2.0 * 3.14159265 + 0.05; i += 2.0 * 3.14159265 / 360.0) {
        int distance = 0;
        while (wallCheck(x + distance * cos(i), y + distance * sin(i), m) && distance < rad) {
            distance += 8;
        }
        tempRadArray.push_back(distance);
    }
//    for (int i = 1; i < tempRadArray.size() - 1; i++) {
//        if (tempRadArray[i] > (tempRadArray[i - 1] + tempRadArray[i + 1]) / 2) {
//            tempRadArray[i] = (tempRadArray[i - 1] + tempRadArray[i + 1]) / 2;
//        }
//    }
    int j = 0;
    for (double i = 0; i <= 2.0 * 3.14159265 + 0.05; i += 2.0 * 3.14159265 / 360.0) {
        int distance = tempRadArray[j];
        glColor3f(1 - (distance / rad), 1 - (distance / rad), 1 - (distance / rad));
        glVertex2i(500 + distance * cos(i), 500 + distance * sin(i));
        j++;
    }
    glEnd();
}


Light::Light() {

}