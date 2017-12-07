//
// Created by sage on 11/6/17.
//


#include "GameInfo.h"
#include <fstream>
#include <iostream>
#include <vector>

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

    Point newPoint{};
    newPoint.x = static_cast<int>(rint(xP));
    newPoint.y = static_cast<int>(rint(yP));

    return newPoint;
}

using namespace std;

GameInfo::GameInfo() {
    score.got = 0;
    score.name = "";
    score.time = 0.0;
    score.completed = false;
    startTime = clock();
}

void GameInfo::end() {
    endTimer();
    saveScore();
    exit(0);
}

int GameInfo::loadPlayer(string name) {
    ifstream fileIn;

    fileIn.open("players.txt");

    if (!fileIn) {
        cerr << "Unable to open players.txt";
        exit(1);
    }

    string line;
    int x;

    while (fileIn >> line >> x) {
        if (line == name) {

            fileIn.close();
            return x;
        }
    }

    fileIn.close();
    //Return -1 for not found
    return -1;
}

void GameInfo::updatePlayer(string name, int level) {

    vector<string> names(MAX_PLAYERS);
    vector<int> levels(MAX_PLAYERS);

    ifstream fileIn;
    fileIn.open("players.txt");

    if (!fileIn) {
        cerr << "Unable to open players.txt";
        exit(1);
    }

    string line;
    int x;

    int size = 0;

    //Read the file into memory and change the players level that needs to be changed
    while (fileIn >> line >> x) {

        names[size] = line;

        if (line == name) {
            levels[size] = level;
        } else {
            levels[size] = x;
        }

        size++;
    }

    fileIn.close();

    //Write a new version with the change
    ofstream fileOut;

    fileOut.open("players.txt", ios::out);

    if (fileOut) {

        for (int i = 0; i < size; i++){
            fileOut << names[i] << " " << levels[i] << '\n';
        }
    }

    fileOut.close();
}

void GameInfo::makePlayer(string name) {
    ofstream fileOut;

    fileOut.open("players.txt", ios::app);

    if (fileOut) {
        fileOut << name << " " << 1 << '\n';
    }

    fileOut.close();
}

void GameInfo::saveScore() {

    cout << "Enter a name ";
    while(!(cin >> score.name)) {
        junk();
        cout << "Enter a name ";
    }

    ofstream fileOut;
    fileOut.open("saves.txt", ios::app);

    if (fileOut) {
        fileOut << score.name << "\n"
                << "Time: " << score.time << " s\n"
                << "gotchas: " << score.got << " gots\n";

        if (score.completed) {
            fileOut << "Completed: Yes!\n";
        } else {
            fileOut << "Completed: Nope\n";
        }

        string list = "=========================\n";

        cout << list;
        cout << score.name << "\n"
             << "Time: " << score.time << " s\n"
             << "gotchas: " << score.got << " gots\n";

        if (score.completed) {
            cout << "Completed: Yes\n";
        } else {
            cout << "Completed: Nope\n";
        }

        cout << list;
        fileOut.close();
    }

    else {
        cout << "Error opening file" << endl;
    }
}

void GameInfo::endTimer() {
    score.time = (clock() - startTime) / (double) CLOCKS_PER_SEC;
    cout << CLOCKS_PER_SEC << endl;
}

/**
 * Clears bad user input from the cin stream.
 */
void GameInfo::junk() {
    cin.clear();
    string junk;
    getline(cin,junk);
}

