//
// Created by sage on 11/6/17.
//


#include "GameInfo.h"
#include <fstream>
#include <iostream>

/*
 * Takes in an x and y, and an angle in radians and returns the location of the point rotated around the center
 */
Point rotate(int x, int y, double Angle) {

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

using namespace std;

GameInfo::GameInfo(){
    score.got = 0;
    score.name = "";
    score.time = 0.0;
//    start = clock();
}

/**
 * Timing currently fails to compile
 */
void GameInfo::saveScore(){

    cout << "Enter a name ";
    while(!(cin >> score.name)){
        junk();
        cout << "Enter a name ";
    }

    ofstream fileOut;
    fileOut.open("saves.txt", ios::app);

    if (fileOut) {
        fileOut << score.name << "\n"
                << "Time: " << score.time << " s\n"
                << "gotchas: " << score.time << " gots\n";
        string list = "=========================\n";

        cout << list;
        cout << score.name << "\n"
             << "Time: " << score.time << " s\n"
             << "gotchas: " << score.time << " gots\n";
        fileOut.close();
    }
    else{
        cout << "Error opening file" << endl;
    }
}

//void GameInfo::endTimer(){
//    score.time = (clock() - start) / (double) CLOCKS_PER_SEC;
//}

/**
 * Clears bad user input from the cin stream.
 */
void GameInfo::junk(){
    cin.clear();
    string junk;
    getline(cin,junk);
}

