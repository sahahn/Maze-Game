#include "graphics.h"

#include <iostream>
#include "Maze.h"
#include "Character.h"
#include <math.h>
#include "MapEditor.h"
#include "Text.h"

using namespace std;

enum GameState {StartMenu, Game, Editor, EditorMenu, Login, LevelMenu};

int songState = 0;
bool audio = false;

//Declare the main game object, maze, player, enemies, game
Maze m;
Player p;
vector<Enemy> enemies(MAX_ENEMIES);
GameInfo game;

MapEditor e;

Box startBox;
Box endBox;
Box editorBox;

InputBox inputBox;
Text textInBox;

Text menu1;
Text menu2;
Text gameMenu;

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

//Stores the highest level reached for that user
int levelReached;

//Global flag for rotation
bool rState;

int miniTick;
GameState state;
int previewScale;

//Used in editorMenu
int mark;

/**
 * @param name - string name of song with file extension
 * @param val - which song is playing - so you don't restart song when it is already playing
 */
void playAudio(string name, int val) {
    if (val != songState && audio) {
        string path = "start ..\\";
        system((path + name).c_str());
        songState = val;
    }
}

void init() {
    state = Login;

    for (bool &key : keys) {
        key = false;
    }

    menu1 = Text();
    menu1.setTextAndLoc("Enter your name and press enter to continue ",
                        {(SCREEN_WIDTH / 2) - 220, (SCREEN_HEIGHT/2) - 125});

    //First params are x and y, then width and height, and finally color
    inputBox = InputBox((SCREEN_WIDTH / 2) - 100, (SCREEN_HEIGHT/2) - 100, 200, 24, 1,1,1);
    textInBox = Text();
    playAudio("soundtrackStart.mp3", 5);


}


//Initialize
void menuInit() {
    playAudio("soundtrackStart.mp3", 5);

    state = StartMenu;
    startBox = Box(300,300,50,0,1,0);
    endBox = Box(5,5,25,1,0,0);
    editorBox = Box(500,500,50,0,0,1);

    menu1 = Text();
    menu2 = Text();

    //Start menu text
    menu1.setTextAndLoc("Hello, and welcome to the game or something, go ahead an click the green square.", {50,50});
    menu2.setTextAndLoc("The game might start if you do, oh and red to quit, blue for editor menu...", {50,100});

    currentLevel = 1;
}

void gameInit() {

    game = GameInfo();
    m = Maze(currentLevel, true); //True for load live game

    gameMenu = Text();
    gameMenu.setTextAndLoc("Level: " + to_string(currentLevel), {(SCREEN_WIDTH/2) - 35, 20});

    //Display a different tip/message for some levels
    switch(currentLevel) {
        case (1) : {
            menu1.setTextAndLoc("So you know, use the arrow keys or wasd to move, and try and find the red block", {50,50});
            playAudio("soundtrack0.mp3", 0);
            break;
        }
        case (2) : {
            menu1.setTextAndLoc("Oh uh... did I mention enemies? Yeah... try not to get him by them...", {50,50});
            playAudio("soundtrack1.mp3", 1);
            break;
        }

        case (3) : {
            menu1.setTextAndLoc("Turns out there are different kinds of enemies... who knew!", {50,50});
            playAudio("soundtrack2.mp3", 2);
            break;
        }
        case (4) : {
            menu1.setTextAndLoc("",{50,50});
            playAudio("soundtrack3.mp3", 3);
            break;
        }
        case (5) : {
            menu1.setTextAndLoc("",{50,50});
            playAudio("soundtrack4.mp3", 4);
            break;
        }

        default:
            menu1.setTextAndLoc("",{50,50});
            playAudio("soundtrack4.mp3", 4);
    }

    p = Player(m.getStartX(),m.getStartY());

    numEnemies = static_cast<int>(m.enemyInfo.size());

    //If over the max enemies, just use up to the max
    if (numEnemies > MAX_ENEMIES) {
        numEnemies = MAX_ENEMIES;
    }

    //Load the enemies into the enemies vector, w/ info from loading
    for (int i = 0; i < numEnemies; i++) {
        enemies[i] = Enemy(m.enemyInfo[i].first.first,
                           m.enemyInfo[i].first.second, eType(m.enemyInfo[i].second));
    }

    //Init the angles of the map
    angle = 0;
    angleR = 0;

    //rState is used when the player is hit by the Flipper
    rState = false;

    //miniTicks are used for more percise enemy movement
    miniTick = 0;

    //End box is static in game
    endBox.x = 5;
    endBox.y = 5;

    //Lastly set the state to game
    state = Game;
}

/**
 * Helper function for levelMenu and editorMenu to generate preview/level boxes
 * @param yShift - int change in yShift for all boxes
 */
void initPreviewBoxes(int yShift) {

    //To make a dif # per rows, change 3 w/ something
    int W = (SCREEN_WIDTH / 3);
    int H = (SCREEN_HEIGHT / 3);

    int sz = (H - (H/3)) + (HEIGHT - ((H - (H/3)) % HEIGHT));

    //Set the previewScale
    previewScale = sz / HEIGHT;

    //Init the levelBoxes, position pre-determined to fill in 3 per row, w/ 3 visible columns
    for (int i = 0; i < NUM_LEVELS; i++){
        levelBoxes[i] = Box(((W/6) + ((i % 3) * W)), ((H/6) + ((i/3) * H)), sz, .2, .2, .2);
        levelBoxes[i].y += yShift;
    }

}

void levelMenuInit() {
    state = LevelMenu;

    endBox = Box(5,5,25,1,0,0);

    initPreviewBoxes(100);

    menu1.setTextAndLoc("Select a level to play!", {50,50});
    menu2.setTextAndLoc("Use the up and down arrow to navigate additional levels!", {50,100});

    //Used to flag a box
    mark = 0;

}

void editorMenuInit() {
    playAudio("ElevatorMusic.mp3", 10);
    state = EditorMenu;

    endBox = Box(5,5,25,1,0,0);

    initPreviewBoxes(150);

    menu1.setTextAndLoc("Select a map to edit, or drag and drop to change the level order.", {50,50});
    menu2.setTextAndLoc("Use the up and down arrow to navigate additional levels!", {50,100});

    //Used to flag a box
    mark = 0;
}

/**
 * Initialize the variables for the editor, and load in/create a maze
 * @param l - int level that should be loaded into editor
 * @param emp - bool for if that level is empty
 */
void editorInit(int l, bool emp) {
    state = Editor;

    e = MapEditor();
    m = !emp ? Maze(l, false) : Maze();

    e.setLevel(l);

    //Start and end box in bottom for save/not save in Editor
    startBox.x = (SCREEN_WIDTH / 2) - startBox.size;
    startBox.y = SCREEN_HEIGHT - startBox.size;

    //Change size back
    endBox.size = 50;
    endBox.x = (SCREEN_WIDTH / 2) + endBox.size;
    endBox.y = SCREEN_HEIGHT - endBox.size;

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


    //Check for backspace key if on Login screen
    if (state == Login) {

       if (key == 13) {
            if (inputBox.index > 0) {

                //Erase the |
                inputBox.content.erase(static_cast<unsigned long>(inputBox.index), 1);


                //Load the players level
                levelReached = game.loadPlayer(inputBox.content);

                //If player doesn't exist yet
                if (levelReached == -1) {
                    game.makePlayer(inputBox.content);
                    levelReached = 1;
                }

                menuInit();

            } else {
                inputBox.g = .6;
                inputBox.b = .6;
            }

       }  else if ((inputBox.getActive()) && (key == 8)) {
            inputBox.delChar();
        }
    }

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

            case '6':
                m.maze[e.mX][e.mY].setStati(ScaryThingSpawn);
                break;

            case 'f':
                m.fillFromSpot(e.mX,e.mY);
                break;
        }

    } else if (state == Login) {

        //Add char if in login and inputBox selected, and key not backspace or enter
        if (inputBox.getActive() && (key != 8) && (key != 13)) {
            inputBox.addChar(key);
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
        default :
            break;
    }
}

void cursor(int x, int y) {

    switch (state) {


        case (Game) : {

            //Unused right now but code for player rotation
            //double ang;
            //ang = atan2(y - (SCREEN_HEIGHT / 2), x - (SCREEN_WIDTH / 2));

            //p.setPlayerRotation(ang);
            //glutPostRedisplay();

            endBox.checkHover(x,y);
            break;
        }

        case (StartMenu) : {

            startBox.checkHover(x,y);
            endBox.checkHover(x,y);
            editorBox.checkHover(x,y);

            glutPostRedisplay();
            break;
        }

        case (LevelMenu) : {
            for (int i = 0; i < NUM_LEVELS; i++) {
                levelBoxes[i].checkHover(x,y);
            }

            endBox.checkHover(x,y);

            glutPostRedisplay();
            break;
        }

        case (Editor) : {

            e.mX = (e.loc.x - EDITOR_SCOPE + 1) + ((y - e.yShift)/EDITOR_SCALE);
            e.mY = (e.loc.y - EDITOR_SCOPE + 1) + ((x - e.xShift)/EDITOR_SCALE);

            startBox.checkHover(x,y);
            endBox.checkHover(x,y);

            glutPostRedisplay();
            break;
        }
        
        case (EditorMenu) : {

            for (int i = 0; i < NUM_LEVELS; i++) {
                levelBoxes[i].checkHover(x,y);
            }

            endBox.checkHover(x,y);

            glutPostRedisplay();
            break;
        }

        default: {
            break;
        }
    }
}

void mousemov(int x, int y) {

    switch (state) {

        //Check for hover even if mouse held down
        case (EditorMenu) : {
            for (int i = 0; i < NUM_LEVELS; i++) {
                levelBoxes[i].checkHover(x, y);
            }
        }

        //In the editor, mouse movement is used for dragging blocks over an area
        case (Editor) : {

            int tempX = (e.loc.x - EDITOR_SCOPE + 1) + ((y - e.yShift) / EDITOR_SCALE);
            int tempY = (e.loc.y - EDITOR_SCOPE + 1) + ((x - e.xShift) / EDITOR_SCALE);

            if (!((e.mX == tempX) && (e.mY == tempY))) {
                keys[GLUT_LEFT_BUTTON] = true;
                e.mX = tempX;
                e.mY = tempY;
            }
        }

        default :
            break;
    }

    glutPostRedisplay();

}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int stateB, int x, int y) {

    switch (state) {
        case (StartMenu) : {

            if ((button == GLUT_LEFT_BUTTON) && (stateB == GLUT_DOWN)) {

                if (startBox.checkHover(x, y)) {
                    levelMenuInit();

                } else if (endBox.checkHover(x, y)) {

                    glutDestroyWindow(wd);
                    exit(0);

                } else if (editorBox.checkHover(x, y)) {
                    editorMenuInit();
                }
            }
            break;
        }

        case (Login) : {

            if (inputBox.checkHover(x, y)) {
                inputBox.setActive(true);
            } else if (inputBox.getActive()) {
                inputBox.setActive(false);
            }
            break;
        }

        case (LevelMenu) : {

            if (stateB == GLUT_DOWN) {
                for (int i = 0; i < levelReached; i++) {
                    if (levelBoxes[i].checkHover(x, y)) {
                        currentLevel = i + 1;
                        gameInit();
                    }
                }

                if (endBox.checkHover(x, y)) {
                    menuInit();
                }
            }
        }

        case (EditorMenu) : {
            if (stateB == GLUT_DOWN) {
                for (int i = 0; i < NUM_LEVELS; i++) {
                    if (levelBoxes[i].checkHover(x, y)) {
                        mark = i;
                    }
                }

                if (endBox.checkHover(x, y)) {
                    menuInit();
                }

            } else if (stateB == GLUT_UP) {

                for (int i = 0; i < NUM_LEVELS; i++) {

                    //If same box as left click, load editor
                    if ((levelBoxes[i].hover) && (i == mark)) {
                        editorInit(i + 1, levelBoxes[i].getE());
                    }

                        //If a different box then left click, attempt to switch them
                    else if ((levelBoxes[i].hover) && (i != mark)) {

                        string L = std::to_string((mark + 1));
                        string oldName = L + ".txt";
                        string tempName = "temp.txt";

                        //Try renaming marked box to temp;
                        rename(oldName.c_str(), tempName.c_str());

                        L = std::to_string((i + 1));
                        string newName = L + ".txt";

                        //Try renaming the end square w/ the first square
                        rename(newName.c_str(), oldName.c_str());

                        //Lastly rename temp name to new name's spot
                        rename(tempName.c_str(), newName.c_str());
                    }
                }
            }

            break;
        }

        case (Editor) : {

            if ((button == GLUT_LEFT_BUTTON) && (stateB == GLUT_DOWN)) {

                //Check the start and end box, start box to save level
                //Red to return to main menu
                if (startBox.checkHover(x, y)) {
                    m.saveLevel(e.getLevel());
                    menuInit();
                } else if (endBox.checkHover(x, y)) {
                    editorMenuInit();
                }

                //To avoid erroneous double clicks
                if (!(e.clickTimer)) {

                    e.clickTimer = true;
                    keys[GLUT_LEFT_BUTTON] = true;

                    //For filling, only fill with the start value,
                    //You fill with dragging cursorE w/ mouseE held down

                    e.fillVal = m.maze[e.mX][e.mY].getWall();

                }
            }
            break;
        }

        case (Game) : {

            if ((button == GLUT_LEFT_BUTTON) && (stateB == GLUT_DOWN) && (endBox.checkHover(x, y))) {

                //If end Box is pressed within the game decrement level
                currentLevel--;

                //and go to EndMenu
                levelMenuInit();

                //Change the text also,
                menu2.setTextAndLoc("You can try again... if you want", {50, 100});

            }

            break;
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

            p.updateLight();
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

            //Draw the endBox
            endBox.draw();

            //Draw the thing that says what level you're on.. you know
            gameMenu.draw();

            menu1.draw();


            break;
        }

        case (StartMenu): {

            menu1.draw();
            menu2.draw();

            startBox.draw();
            endBox.draw();
            editorBox.draw();

            break;
        }

        case (Login) : {

            //Draw the prompt
            menu1.draw();

            //Draw the input box
            inputBox.draw();

            //Set the text to whats saved in the inputBox
            textInBox.setTextAndLoc(inputBox.content, {inputBox.x+2, inputBox.y+20});

            //calc the blink for the cursor thing, the '|'
            if (inputBox.getActive()) {
                inputBox.blinkCursor();
            }

            //Sets the color of text to black, and getThere tells it to display the | marker or not
            textInBox.draw(0, 0, 0, inputBox.getThere());
        }

        case (LevelMenu) : {
            menu1.draw();
            menu2.draw();

            //For each level loads either a preview, or blank space.
            for (int i = 0; i < levelReached; i++){

                Maze lev = Maze(i+1,false);

                //If there isn't a file for the level, set it as empty
                //or if it is the next level to complete
                if ((lev.getEmpty()) || (i == levelReached - 1)) {
                    levelBoxes[i].setE(true);
                    levelBoxes[i].draw();

                } else {

                    //Otherwise, if the level exists and is completed
                    for (int t = 0; t < WIDTH; t++) {
                        for (int s = 0; s < HEIGHT; s++) {
                            lev.maze[t][s].draw((levelBoxes[i].x + (s * previewScale)),
                                                (levelBoxes[i].y + (t * previewScale)), previewScale,
                                                levelBoxes[i].hover);
                        }
                    }
                }

                //Display the level number above each map preview
                Text levNum(to_string(i+1), {levelBoxes[i].x,levelBoxes[i].y - 5});
                levNum.draw();

                endBox.draw();
            }

            break;
        }


        case (EditorMenu) : {

            //Draw the menus
            menu1.draw();
            menu2.draw();

            //For each level loads either a preview, or blank space.
            for (int i = 0; i < NUM_LEVELS; i++){

                Maze lev = Maze(i+1,false);

                //If there isn't a file for the level, set it as empty
                //and instead draw a blank square
                if (lev.getEmpty()) {
                    levelBoxes[i].setE(true);
                    levelBoxes[i].draw();


                } else {

                    //Set to not empty
                    levelBoxes[i].setE(false);

                    //Otherwise, if the level exists, display a preview
                    for (int t = 0; t < WIDTH; t++) {
                        for (int s = 0; s < HEIGHT; s++) {
                            lev.maze[t][s].draw((levelBoxes[i].x + (s * previewScale)),
                                                (levelBoxes[i].y + (t * previewScale)), previewScale,
                                                levelBoxes[i].hover);
                        }
                    }
                }

                //Display the level number above each map preview
                Text levNum(to_string(i+1), {levelBoxes[i].x,levelBoxes[i].y - 5});
                levNum.draw();
            }

            endBox.draw();
            break;
        }

        case (Editor) : {

            //The editor screen is a modified verison of the game function,
            //Just responsible for keeping the editor within the bounds
            int lb1 = e.loc.x - EDITOR_SCOPE;
            int upb1 = e.loc.x + EDITOR_SCOPE;
            int lb2 = e.loc.y - EDITOR_SCOPE;
            int upb2 = e.loc.y + EDITOR_SCOPE;

            int x = -1;

            for (int i = lb1; i <= upb1 + 1; i++, x++) {

                int y = -1;

                for (int j = lb2; j <= upb2 + 1; j++, y++) {

                    //Draw the tile
                    m.maze[i][j].draw(x, y, e.xShift, e.yShift);
                }
            }

            //Draw the start and end Boxes which act as save or don't save here
            startBox.draw();
            endBox.draw();

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
    if (m.getNextX() != -2) {

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

            //Check buffer
            if (E.doBuffer()) {

                switch (E.getType()) {

                    case (Flipper) : {
                        //Flip the screen 90 degrees
                        rState = true;
                        break;
                    }

                    case (Sizer) : {
                        //Increase the Players size
                        p.setSize(p.getSize() + 1);
                        break;
                    }

                    case (ScaryThing) : {

                        //If hit by scary thing, light radius smaller for rest of level
                        if (p.getLightRadius() > 50) {
                            p.setLightRadius(p.getLightRadius() - 50);
                        }
                        break;
                    }
                }

                E.setSpawnBuffer(500);

                //Reset the location of the Enemy
                E.x = E.getSpawnX();
                E.y = E.getSpawnY();

                E.xShift = 0;
                E.yShift = 0;

                //Update score
                game.score.got++;
                //cout << "You got Got!" << endl;
            }
        }
    }

    //After movement, the Enemy must call update, which handles updating the Characters location
    //within the array.
    E.update();
}

void timer(int extra) {

    switch (state) {

        case(Game) : {

            //If in rState/rotate state, don't allow other movement/updates
            if (!rState) {

                miniTick++;
                if (miniTick == 3) {
                    miniTick = 0;
                }

                //The movement logic for the player is defined here,
                //Where the move is calculated, and then done.
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

                //Additional movement logic for velocity
                if (!keys[GLUT_KEY_DOWN] && !keys[GLUT_KEY_UP]) {
                    p.calcMove((int) p.getMovementBuffer().x, 0, angleR);

                    if (p.doMove(m)) {
                        p.taperXVelocity();
                    } else {
                        p.flipVelocity();
                    }
                }

                if (!keys[GLUT_KEY_LEFT] && !keys[GLUT_KEY_RIGHT]) {
                    p.calcMove(0, (int) p.getMovementBuffer().y, angleR);

                    if (p.doMove(m)) {
                        p.taperYVelocity();
                    } else {
                        p.flipVelocity();
                    }
                }

                //Update the player loc after move info
                p.update();

                //If the tile the player is in is an end tile
                if (m.maze[p.x][p.y].getStati() == End) {
                    if (currentLevel == levelReached) {
                        levelReached++;
                        game.updatePlayer(inputBox.content, levelReached);
                    }

                    levelMenuInit();
                }

                //Call the enemies search + movement function
                for (int z = 0; z < numEnemies; z++) {

                    //Only call if miniTick is >= to enemies update rate, e.g.
                    //sizer gets called every time, but Flipper gets called every 2/3
                    //Also check the enemies respawn buffer
                    if ((miniTick >= enemies[z].getUpdateRate()) && (enemies[z].doBuffer())) {
                        m.aStarSearch(enemies[z].x, enemies[z].y, p.x, p.y);
                        follow_path(enemies[z]);
                    }
                }

            } else {

                //If rState, then rotate the map 90 degrees
                angle = (angle + 1) % 360;
                angleR = angle * (LUKE_M_PI / 180);

                //Stop at intervals of 90 degrees
                if (angle % 90 == 0) {
                    rState = false;
                }
            }
            break;
        }

        case (StartMenu)  : {

            //Have the menu boxes move around randomly idk why
            startBox.randomMenuMovement();
            endBox.randomMenuMovement();
            editorBox.randomMenuMovement();
            break;
        }

        case (LevelMenu) : {

            //Logic to move up and down in the editorMenu
            if (keys[GLUT_KEY_UP]) {
                for (int i = 0; i < levelReached; i++){
                    levelBoxes[i].y += 5;
                }
            }

            if (keys[GLUT_KEY_DOWN]) {
                for (int i = 0; i < levelReached; i++){
                    levelBoxes[i].y -= 5;
                }
            }

            endBox.randomMenuMovement();

            break;
        }

        case (EditorMenu) : {

            //Logic to move up and down in the editorMenu
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

            endBox.randomMenuMovement();
            break;

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

                //When a tile is clicked in editor, flip from wall to not, or vice-versa
                if ((keys[GLUT_LEFT_BUTTON]) && (m.maze[e.mX][e.mY].getWall() == e.fillVal)) {
                    m.maze[e.mX][e.mY].flipWall();

                    //Flip flag off
                    keys[GLUT_LEFT_BUTTON] = false;
                }
            }

            //The logic for moving around the camera
            if (keys[GLUT_KEY_DOWN]) {

                if ((e.loc.x + EDITOR_SCOPE <= HEIGHT)) {
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

                if ((e.loc.y + EDITOR_SCOPE <= WIDTH)) {
                    e.xShift -= e.moveSpeed;
                }
            }

            //Update the map locations
            e.update();
            break;
        }

        default : {
            break;
        }

    }

    glutPostRedisplay();
    glutTimerFunc(3, timer, 0);
}


/* Main function: GLUT runs as a console application starting at main()  */
int graphicsPlay(int argc, char **argv) {

    //Call the first init when the game starts
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
    glutMotionFunc(mousemov);
    glutMouseFunc(mouse);

    //Timer
    glutTimerFunc(0, timer, 0);

    //Enter the event-processing loop
    glutMainLoop();

    return 0;
}