/*
 * proj2_app.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 *      Edited by: Aiden Weiehn
 */

#ifndef PROJ2_APP_H_
#define PROJ2_APP_H_

#include <HAL/Timer.h>

// All definitions responsible for timers
#define TITLE_SCREEN_TIMER 3000
#define OBSTACLE_WAIT 2000 // 2 seconds
#define OBSTACLE_MOVE 5000 // 5 seconds
#define IFRAMES 500 // 0.5 seconds
#define JUMP 250 // 1 second

// Defines responsible for game logic
#define LIVES 3
#define SCORE 000000
#define OBSTACLE_SIZE 6

// Defines for position of the player
#define MIN_X 10.0
#define MAX_X 15.0
#define MIN_Y 69.0
#define MAX_Y 74.0

#define LOWERBOUND -1
#define UPPERBOUND 1

/**
 * The top-level Application object, initialized in the main() entry point and
 * passed around to basically every other function. Holds state variables
 * which YOU define as part of your application's state.
 */
enum _GameState
{
    TITLE_SCREEN,
    MENU_SCREEN,
    INSTRUCTIONS_SCREEN,
    GAME_SCREEN,
    FINAL_SCREEN,
    RESULTS_SCREEN
};
typedef enum _GameState GameState;

enum _Cursor
{
    CURSOR_0,
    CURSOR_1,
    CURSOR_2
};

typedef enum _Cursor Cursor;

struct _Obstacles
{
    double xMin;
    double xMax;
    double yMin;
    double yMax;
    double yMin2;
    double yMax2;
    bool moving;
    SWTimer obstacleTimer;
};
typedef struct _Obstacles Obstacle;

struct _App_proj2
{
    SWTimer timer, timer2, timer3, iFrames; // General-purpose timer for when screens must disappear
    GameState state;
    Cursor cursor;
    int highScores[3];
    int lives;
    bool reset;
    unsigned int score;
    char currentScore[7], highScoreOne[7], highScoreTwo[7], highScoreThree[7];
    float minX, maxX, minY, maxY;
    Obstacle obstacles[OBSTACLE_SIZE];
};
typedef struct _App_proj2 App_proj2;

// Boilerplate constructor function for the Application
App_proj2 App_proj2_construct(HAL *hal_p);
Obstacle App_obstacle_construct(HAL *hal_p);
void App_proj2_loop(App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p);

void App_proj2_handleTitleScreen(App_proj2 *app, HAL *hal_p);
void App_proj2_handleMenuScreen(App_proj2 *app, HAL *hal_p, GFX *gfx_p);
void App_proj2_handleInstructionsScreen(App_proj2 *app, HAL *hal_p);
void App_proj2_handleGameScreen(App_proj2 *app, HAL *hal_p, Obstacle *obj_p);
void App_proj2_handleResultScreen(App_proj2 *app, HAL *hal_p);
void App_proj2_handleFinalScreen(App_proj2 *app, HAL *hal_p, Obstacle *obj_p);

// Helper functions which clear the screen and draw the text for each state
void App_proj2_showTitleScreen(GFX *gfx_p, App_proj2 *app_p);
void App_proj2_showMenuScreen(App_proj2 *app_p, GFX *gfx_p);
void App_proj2_showInstructionsScreen(App_proj2 *app_p, GFX *gfx_p);
void App_proj2_showGameScreen(App_proj2 *app_p, GFX *gfx_p);
void App_proj2_showResultScreen(App_proj2 *app_p, GFX *gfx_p);
void App_proj2_showFinalScreen(App_proj2 *app, HAL *hal_p, Obstacle *obj_p);

// Game logic functions
void moveSquare(GFX *gfx_p, HAL *hal_p, App_proj2 *app_p);
void obstacleSpawner(GFX *gfx_p, App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p);
void obstacleTypeVal(App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p, int i);
void damageCheck(GFX *gfx_p, App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p);

// Misc functions
void make_6digit_NumString(unsigned int num, char *string);
void calculateHighScore(App_proj2 *app_p, HAL *hal_p);

#endif /* PROJ2_APP_H_ */
