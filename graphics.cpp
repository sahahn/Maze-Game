#include "graphics.h"

#include <iostream>
#include "Maze.h"
#include "GameInfo.h"
#include "Character.h"
#include <ctime>
#include <math.h>
#include "MapEditor.h"
#include "Box.h"
#include "Text.h"

using namespace std;

enum GameState {StartMenu, Game, EndMenu, Editor, EditorMenu};

//Declare the main game object, maze, player, enemies, game
Maze m;
Player p;
vector<Enemy> enemies(MAX_ENEMIES);
GameInfo game;

MapEditor e;

Box startBox;
Box endBox;
Box editorBox;

Text menu1;
Text menu2;

vector<Box> levelBoxes(NUM_LEVELS);

int numEnemies;

int wd;

//Holds value of key presses and releases
bool keys[128];

//angle of screen tilt in degrees
int angle;
//angle in radians
double angleR;

//Stores what level the player is in
int currentLevel;

//Global flag for rotation
bool rState;

int miniTick;
GameState state;
int previewScale;

//Initialize
void init() {
    for (int i = 0; i < 128; i++) {
        keys[i] = false;
    }

    state = StartMenu;
    startBox = Box(300,300,50,0,1,0);
    endBox = Box(350,350,50,1,0,0);
    editorBox = Box(500,500,50,0,0,1);

    menu1 = Text();
    menu2 = Text();

    currentLevel = 1;
}

void gameInit() {

    game = GameInfo();
    m = Maze(currentLevel, true); //True for load live game
    currentLevel++;

    p = Player(m.getStartX(),m.getStartY());

    numEnemies = m.enemyInfo.size();

    for (int i = 0; i < numEnemies; i++) {
        enemies[i] = Enemy(m.enemyInfo[i].first.first,
                           m.enemyInfo[i].first.second, eType(m.enemyInfo[i].second));
    }

    angle = 0;
    angleR = 0;

    rState = false;

    miniTick = 0;

    //Lastly put the state into game
    state = Game;

}

void editorMenuInit() {

    state = EditorMenu;

    //To make a dif # per rows, change 3 w/ something
    int W = (SCREEN_WIDTH / 3);
    int H = (SCREEN_HEIGHT / 3);

    int sz = (H - (H/3)) + (HEIGHT - ((H - (H/3)) % HEIGHT));

    //Set the previewScale
    previewScale = sz / HEIGHT;

    cout << previewScale << endl;

    for (int i = 0; i < NUM_LEVELS; i++){
        levelBoxes[i] = Box(((W/6) + ((i % 3) * W)), ((H/6) + ((i/3) * H)), sz, .5, .5, .5);
    }
}

void editorInit(int l) {
    state = Editor;

    e = MapEditor();
    if (l != -1) {
        m = Maze(l, false);

    } else {
        m = Maze(3,3);
    }

    e.level = l;
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
        glutDestroyWindow(wd);
        game.end();
    }

    //key 32 is space, I just have that rotate the maze here
    //if (key == 32) {
    //    rState = true; //Animated transition
    //}

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

    if (state == Editor) {
        switch(key) {

            case '1':
                m.maze[e.mX][e.mY].setStati(None);
            break;

            case '2':
                m.clearStart();
                m.maze[e.mX][e.mY].setStati(Start);
            break;

            case '3':
                m.maze[e.mX][e.mY].setStati(End);
            break;

            case '4':
                m.maze[e.mX][e.mY].setStati(FlipperSpawn);
            break;

            case '5':
                m.maze[e.mX][e.mY].setStati(SizerSpawn);
            break;
        }
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

    switch (state) {

        //Unused right now but for player rotation
        case (Game) : {

            //double ang;
            //ang = atan2(y - (SCREEN_HEIGHT / 2), x - (SCREEN_WIDTH / 2));

            //p.setPlayerRotation(ang);
            //glutPostRedisplay();
            break;
        }

        case (StartMenu) : {

            startBox.checkHover(x,y);
            endBox.checkHover(x,y);
            editorBox.checkHover(x,y);

            glutPostRedisplay();
            break;
        }

        case (EndMenu) : {

            startBox.checkHover(x,y);
            endBox.checkHover(x,y);

            glutPostRedisplay();
            break;
        }

        case (Editor) : {

            e.mX = (e.loc.x - EDITOR_SCOPE + 1) + ((y - e.yShift)/EDITOR_SCALE);
            e.mY = (e.loc.y - EDITOR_SCOPE + 1) + ((x - e.xShift)/EDITOR_SCALE);
            glutPostRedisplay();
        }
        
        case (EditorMenu) : {

            for (int i = 0; i < NUM_LEVELS; i++) {
                levelBoxes[i].checkHover(x,y);
            }
        }
    }
}

void mousemov(int x, int y) {

    if (state == Editor) {

        int tempX = (e.loc.x - EDITOR_SCOPE + 1) + ((y - e.yShift) / EDITOR_SCALE);
        int tempY = (e.loc.y - EDITOR_SCOPE + 1) + ((x - e.xShift) / EDITOR_SCALE);

        if (!((e.mX == tempX) && (e.mY == tempY))) {
            keys[GLUT_LEFT_BUTTON] = true;
            e.mX = tempX;
            e.mY = tempY;
        }

        glutPostRedisplay();
    }
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int stateB, int x, int y) {

        if ((state == StartMenu) || (state == EndMenu)) {

            if (stateB == GLUT_LEFT_BUTTON) {
                if (startBox.checkHover(x, y)) {
                    gameInit();
                } else if (endBox.checkHover(x, y)) {
                    //Mark as completed and end game.
                    game.score.completed = true;

                    glutDestroyWindow(wd);
                    game.end();
                } else if (editorBox.checkHover(x,y)) {
                    editorMenuInit();
                }

            }
        }

        else if ((state == EditorMenu) && (stateB == GLUT_LEFT_BUTTON)) {
            for (int i = 0; i < NUM_LEVELS; i++){
                if (levelBoxes[i].checkHover(x, y)) {
                    editorInit(i+1);
                }
            }

        }

        else if (state == Editor) {

            if ((button == GLUT_LEFT_BUTTON) && (stateB == GLUT_DOWN)) {

                //To avoid erroneous double clicks
                if (!(e.clickTimer)) {

                    e.clickTimer = true;
                    keys[GLUT_LEFT_BUTTON] = true;

                    //For filling, only fill with the start value,
                    //You fill with dragging cursorE w/ mouseE held down
                    if (m.maze[e.mX][e.mY].getWall()) {
                        e.fillVal = true;
                    } else {
                        e.fillVal = false;
                    }
                }
            }
        }

        glutPostRedisplay();

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

    switch (state) {
        case (Game): {

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
                            enemies[z].setLocation(x, y);
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
            break;
        }

        case (StartMenu): {
            menu1.setTextAndLoc("Hello, and welcome to our very low quality menu!", {50,50});
            menu1.draw();

            menu2.setTextAndLoc("Go ahead and click that square, the game might start also if you do.", {50,100});
            menu2.draw();

            startBox.draw();
            endBox.draw();
            editorBox.draw();

            break;
        }

        case (EndMenu): {
            menu1.setTextAndLoc("Hey, that was okay.", {50,50});
            menu1.draw();

            menu2.setTextAndLoc("Ready for the next level?", {50,100});
            menu2.draw();

            startBox.draw();
            endBox.draw();
            break;
        }

        case (EditorMenu) : {
            menu1.setTextAndLoc("Keys: 1 to carve wall, 2 to place start, ", {50,300});
            menu1.draw();

            menu2.setTextAndLoc("3 to place end, 4 and 5 to spawn enemies", {50, 350});
            menu2.draw();
            for (int i = 0; i < NUM_LEVELS; i++){


                Maze lev = Maze(i+1,false);

                for (int t = 0; t < WIDTH; t++) {
                    for (int s = 0; s < HEIGHT; s++) {
                        lev.maze[t][s].draw((levelBoxes[i].x + (s * previewScale)),
                                            (levelBoxes[i].y + (t * previewScale)), previewScale, levelBoxes[i].hover);
                    }
                }
            }
        }

            break;

        case (Editor) : {

                    int lb1 = e.loc.x - EDITOR_SCOPE;
                    int upb1 = e.loc.x + EDITOR_SCOPE;
                    int lb2 = e.loc.y - EDITOR_SCOPE;
                    int upb2 = e.loc.y + EDITOR_SCOPE;

                    int x = -1;

                    for (int i = lb1; i <= upb1 + 1; i++, x++) {

                        int y = -1;

                        for (int j = lb2; j <= upb2 + 1; j++, y++) {

                            m.maze[i][j].draw(x , y, e.xShift, e.yShift);
                        }
                    }
                }
            }

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


    switch (state) {

        case(Game) : {

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

                    state = EndMenu;
                }


                for (int z = 0; z < numEnemies; z++) {

                    if (miniTick < enemies[z].getUpdateRate()) {
                        m.aStarSearch(enemies[z].x, enemies[z].y, p.x, p.y);
                        follow_path(enemies[z]);
                    }

                }

            } else {

                angle = (angle + 1) % 360;
                angleR = angle * (LUKE_M_PI / 180);

                if (angle % 90 == 0) {
                    rState = 0;
                }
            }
        }

        case (StartMenu)  : {
            startBox.randomMenuMovement();
            endBox.randomMenuMovement();
            editorBox.randomMenuMovement();
        }

        case (EndMenu)  : {
            startBox.randomMenuMovement();
            endBox.randomMenuMovement();
        }

        case (EditorMenu) : {
            if (keys[GLUT_KEY_UP]) {
                for (int i = 0; i < NUM_LEVELS; i++){
                    levelBoxes[i].y += 5;
                }
            }

            if (keys[GLUT_KEY_DOWN]) {
                for (int i = 0; i < NUM_LEVELS; i++){
                    levelBoxes[i].y -= 5;
                }
            }

        }

        case (Editor) : {

            //If click Timer, then increment
            //until timerE is out, used to prevent accidental double clicks
            if (e.clickTimer) {
                e.ticks++;

                if (e.ticks == 20) {

                    e.ticks = 0;
                    e.clickTimer = false;
                }

            } else {

                if ((keys[GLUT_LEFT_BUTTON]) && (m.maze[e.mX][e.mY].getWall() == e.fillVal)) {
                    m.maze[e.mX][e.mY].flipWall();
                    keys[GLUT_LEFT_BUTTON] = false;
                }
            }

            if (keys[GLUT_KEY_DOWN]) {

                if ((e.loc.x + EDITOR_SCOPE  < HEIGHT)) {
                    e.yShift -= e.moveSpeed;
                }
            }

            if (keys[GLUT_KEY_LEFT]) {

                if ((e.loc.y - EDITOR_SCOPE) > 0) {
                    e.xShift += e.moveSpeed;
                }

            }

            if (keys[GLUT_KEY_UP]) {

                if ((e.loc.x - EDITOR_SCOPE) > 0) {
                    e.yShift += e.moveSpeed;
                }

            }

            if (keys[GLUT_KEY_RIGHT]) {

                if ((e.loc.y + EDITOR_SCOPE < WIDTH)) {
                    e.xShift -= e.moveSpeed;
                }
            }

            if (e.yShift < (-e.sBoundary)) {
                e.loc.x +=1;
                e.yShift += EDITOR_SCALE;
            }

            if (e.xShift > (e.sBoundary)) {
                e.loc.y -= 1;
                e.xShift -= EDITOR_SCALE;
            }

            if (e.xShift < (-e.sBoundary)) {
                e.loc.y += 1;
                e.xShift += EDITOR_SCALE;
            }

            if (e.yShift > (e.sBoundary)) {
                e.loc.x -= 1;
                e.yShift -= EDITOR_SCALE;
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(3, timer, 0);
}


/* Main function: GLUT runs as a console application starting at main()  */
int graphicsPlay(int argc, char **argv) {

    init();
    //gameInit(1);
    //editorInit(1);

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