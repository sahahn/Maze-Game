#include "graphics.h"

#include <iostream>
#include "Maze.h"
#include "GameInfo.h"
#include "Character.h"
#include <ctime>
#include <math.h>

using namespace std;

//Declare the main game object, maze, player, enemies, game
Maze m;
Player p;
vector<Enemy> enemies(MAX_ENEMIES);
GameInfo game;

int numEnemies;

int wd;

//Holds value of key presses and releases
bool keys[128];

//angle of screen tilt in degrees
int angle;
//angle in radians
double angleR;

//Global flag for rotation
bool rState;

int miniTick;

//Initialize
void init() {
    game = GameInfo();
    m = Maze(1, true); //True for load live game
    p = Player(m.getStartX(),m.getStartY());

    numEnemies = m.enemyInfo.size();

    for (int i = 0; i < numEnemies; i++) {
        enemies[i] = Enemy(m.enemyInfo[i].first.first,
                                    m.enemyInfo[i].first.second, eType(m.enemyInfo[i].second));
    }

    //enemies[0] = Enemy(4, 4,eType(5));
    //enemies[1] = Enemy(4, 4, eType(5));

    for (int i = 0; i < 128; i++) {
        keys[i] = false;
    }

    angle = 0;
    angleR = 0;

    rState = false;

    miniTick = 0;
}

// Initialize OpenGL Graphics
void initGL() {
    // Set "clearing" or background color to black
    glClearColor(.02,.02,.02,.02);
}

/**********************************************************************
 * Input Handling for keystrokes, mouse ect...
 *********************************************************************/
void kbd(unsigned char key, int x, int y) {

    //Escape key
    if (key == 27) {
        cout << "its ova actually tho" << endl;
        glutDestroyWindow(wd);

        game.end();
    }

    //key 32 is space, I just have that rotate the maze here
    if (key == 32) {
        rState = true; //Animated transition
    }

    switch (key) {

        case 's':
            keys[GLUT_KEY_DOWN] = true;
            break;

        case 'a':
            keys[GLUT_KEY_LEFT] = true;
            break;

        case 'd':
            keys[GLUT_KEY_RIGHT] = true;
            break;

        case 'w':
            keys[GLUT_KEY_UP] = true;
            break;
    }

    glutPostRedisplay();
}

void kbu(unsigned char key, int x, int y) {

    switch (key) {

        case 's':
            keys[GLUT_KEY_DOWN] = false;
            break;

        case 'a':
            keys[GLUT_KEY_LEFT] = false;
            break;

        case 'd':
            keys[GLUT_KEY_RIGHT] = false;
            break;

        case 'w':
            keys[GLUT_KEY_UP] = false;
            break;
    }
}


void kbdS(int key, int x, int y) {

    switch (key) {

        case GLUT_KEY_DOWN:
            keys[GLUT_KEY_DOWN] = true;
            break;

        case GLUT_KEY_LEFT:
            keys[GLUT_KEY_LEFT] = true;
            break;

        case GLUT_KEY_RIGHT:
            keys[GLUT_KEY_RIGHT] = true;
            break;

        case GLUT_KEY_UP:
            keys[GLUT_KEY_UP] = true;
            break;
    }
    glutPostRedisplay();
}


void keyUp(int key, int x, int y) {


    switch (key) {

        case GLUT_KEY_DOWN:
            keys[GLUT_KEY_DOWN] = false;
            break;

        case GLUT_KEY_LEFT:
            keys[GLUT_KEY_LEFT] = false;
            break;

        case GLUT_KEY_RIGHT:
            keys[GLUT_KEY_RIGHT] = false;
            break;

        case GLUT_KEY_UP:
            keys[GLUT_KEY_UP] = false;
            break;
    }
}

void cursor(int x, int y) {
    double ang;
    ang = atan2(y - (SCREEN_HEIGHT / 2), x - (SCREEN_WIDTH / 2));

    p.setPlayerRotation(ang);
    //glutPostRedisplay();
}

void mousemov(int x, int y) {
    //glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {
//glutPostRedisplay();

}

/**********************************************************************
 * Display - which contains the main screen rendering logic
 *********************************************************************/
void display() {
    // tell OpenGL to use the whole window for drawing
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, -1.f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //lb and up refer to lower bound and upper bound, and are there as error checks to
    //make sure the program doesn't attempt to render outside of the maze array.
    int lb1 = p.x - SCOPE;
    int upb1 = p.x + SCOPE;
    int lb2 = p.y - SCOPE;
    int upb2 = p.y + SCOPE;

    if (lb1 < 0) {
        lb1 = 0;
    }
    if (lb2 < 0) {
        lb2 = 0;
    }

    if (upb1 > HEIGHT) {
        upb1 = HEIGHT;
    }

    if (upb2 > HEIGHT) {
        upb2 = HEIGHT;
    }

    // render the players light
    //p.playerLight.renderLight((p.y)*SCALE+(-p.xShift), (p.x)*SCALE+(-p.yShift), m);

    p.playerLight.renderLight(p.x, p.y, p.xShift, p.yShift, m, angle);


    //x and y here are the relative locations to be rendered on the screen,
    //so by starting with -1, the maze generates from 1 square off the screen
    int x = -1;

    for (int i = lb1; i < upb1 + 1; i++, x++) {

        int y = -1;

        for (int j = lb2; j < upb2 + 1; j++, y++) {

            //For each position in the array being drawn, i and j, tell the
            //tile piece to draw it at relative location x and y, according to
            //also the players x and yShift and the current angle of the screen.
            m.maze[i][j].draw(x, y, p.xShift, p.yShift, angleR);

            //If any enemies are in one of the squares,
            //store the location of that square in there location field.
            for (int z = 0; z < numEnemies; z++) {

                if ((enemies[z].x == i) && (enemies[z].y == j)) {
                    enemies[z].setLocation(x,y);
                }
            }
        }
    }

    //Render enemies, specifically after the maze tiles are displayed.
    for (int z = 0; z < numEnemies; z++) {

        enemies[z].draw(p, angleR);
        enemies[z].resetLoc();  //Fixes bug
    }


    //Draw the player
    p.draw();

    //Render now
    glFlush();
}

/**********************************************************************
 * Main Player and Character movement logic
 *********************************************************************/

//Implements the logic for an Enemy to follow the correct path to the player.
void follow_path(Enemy &E) {

    //Check if in players square
    if (!(m.getNextX() == -2)) {

        E.nextCalc(m.getNextX(),m.getNextY());
        E.doMove(m);
    }

        //When in the player's tile, behave differently
        //Specifically, compensate between the difference in player and enemy x and yShift.
    else {

        //First check for collision with the player, if no collision...
        if (p.xShift + p.getSize() < E.xShift || E.xShift + E.getSize() < p.xShift ||
            p.yShift + p.getSize() < E.yShift || E.yShift + E.getSize() < p.yShift) {

            //Get closer to the player,
            if (p.yShift > E.yShift) {

                E.calcMove(0, E.getSpeed(), 0);
                E.doMove(m);

            } else if (p.yShift < E.yShift) {

                E.calcMove(0, -E.getSpeed(), 0);
                E.doMove(m);

            } else if (p.xShift < E.xShift) {

                E.calcMove(-E.getSpeed(), 0, 0);
                E.doMove(m);

            } else if (p.xShift > E.xShift) {

                E.calcMove(E.getSpeed(), 0, 0);
                E.doMove(m);
            }
        }

            //If a collision then,
        else {

            if (E.getType() == Flipper) {

                //Flip the screen 90 degrees
                rState = true;
            } else if (E.getType() == Sizer) {

                //Increase the Players size
                p.setSize(p.getSize() + 1);
            }

            //Reset the location of the Enemy
            E.x = E.getSpawnX();
            E.y = E.getSpawnY();

            E.xShift = 0;
            E.yShift = 0;

            //Update score
            game.score.got++;
            cout << "You got Got!" << endl;

        }
    }

    //After movement, the Enemy must call update, which handles updating the Characters location
    //within the array.
    E.update();
}

void timer(int extra) {

    if (!rState) {

        miniTick++;
        if (miniTick == 2) {
            miniTick = 0;
        }

        if (keys[GLUT_KEY_DOWN]) {
            p.calcMove(0, -p.getSpeed(), angleR);

            if (p.doMove(m)) {
                p.updateVelocity(0, -p.getSpeed());
            } else {
                p.flipVelocity();
            }
        }

        if (keys[GLUT_KEY_LEFT]) {
            p.calcMove(p.getSpeed(), 0, angleR);

            if (p.doMove(m)) {
                p.updateVelocity(p.getSpeed(), 0);
            } else {
                p.flipVelocity();
            }
        }

        if (keys[GLUT_KEY_UP]) {
            p.calcMove(0, p.getSpeed(), angleR);

            if (p.doMove(m)) {
                p.updateVelocity(0, p.getSpeed());
            } else {
                p.flipVelocity();
            }
        }

        if (keys[GLUT_KEY_RIGHT]) {
            p.calcMove(-p.getSpeed(), 0, angleR);

            if (p.doMove(m)) {
                p.updateVelocity(-p.getSpeed(), 0);
            } else {
                p.flipVelocity();
            }
        }

        if (!keys[GLUT_KEY_DOWN] && !keys[GLUT_KEY_UP]) {
            p.calcMove((int) p.getMovementBuffer().x, 0, angleR);

            if (p.doMove(m)) {
                p.taperXVelocity();
            } else {
                p.flipVelocity();
            }
            //if (p.getMovementBuffer().x != 0) cout << p.getMovementBuffer().x << endl;
        }
        if (!keys[GLUT_KEY_LEFT] && !keys[GLUT_KEY_RIGHT]) {
            p.calcMove(0, (int) p.getMovementBuffer().y, angleR);

            if (p.doMove(m)) {
                p.taperYVelocity();
            } else {
                p.flipVelocity();
            }
            //if (p.getMovementBuffer().y != 0) cout << p.getMovementBuffer().y << endl;
        }
        p.update();



        //If you reach the END_X and END_Y square you win!
        if (m.maze[p.x][p.y].getStati() == End) {
                cout << "You win! " << endl;

            //Mark as completed and end game.
            game.score.completed = true;

            glutDestroyWindow(wd);
            game.end();
        }


        for (int z = 0; z < numEnemies; z++) {


            m.aStarSearch(enemies[z].x, enemies[z].y, p.x, p.y);
            follow_path(enemies[z]);
        }




        } else {

        angle = (angle + 1) % 360;
        angleR = angle * (LUKE_M_PI / 180);

        if (angle % 90 == 0) {
            rState = 0;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(3, timer, 0);
}


/* Main function: GLUT runs as a console application starting at main()  */
int graphicsPlay(int argc, char **argv) {

    init();

    // Initialize GLUT
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - SCREEN_WIDTH) / 2,
                           (glutGet(GLUT_SCREEN_HEIGHT) - SCREEN_HEIGHT) / 2);

    //Create the window
    wd = glutCreateWindow("Some game or something");

    //Register callback handler for window re-paint event
    glutDisplayFunc(display);

    //OpenGL initialization
    initGL();

    //Register inputs
    glutKeyboardFunc(kbd);
    glutKeyboardUpFunc(kbu);
    glutSpecialFunc(kbdS);
    glutSpecialUpFunc(keyUp);

    //Turn key repeat mode to off, very important!
    glutSetKeyRepeat(0);

    //Register mouse
    glutPassiveMotionFunc(cursor);
    //glutMotionFunc(mousemov);
    glutMouseFunc(mouse);

    //Timer
    glutTimerFunc(0, timer, 0);

    //Enter the event-processing loop
    glutMainLoop();

    return 0;
}