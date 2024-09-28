/*
 * show.h
 *
 *  Created on: Sep 28, 2024
 *      Author: aiden
 */

#ifndef CODE_SHOW_H_
#define CODE_SHOW_H_

#include <HAL/HAL.h>
#include <Code/proj2_app.h>

// Defines for the lanes
#define BORDER_LEFT 0
#define BORDER_RIGHT 127
#define DIVIDER_TOP 107
#define DIVIDER_BOTTOM 20

// Defines for the lives
#define LIFE_YMIN 112
#define LIFE_YMAX 117
#define LIFEONE_XMIN 75
#define LIFEONE_XMAX 80
#define LIFETWO_XMIN 85
#define LIFETWO_XMAX 90
#define LIFETHREE_XMIN 95
#define LIFETHREE_XMAX 100

// Graphics that show when the code gets reset
// 19 Lines
void App_proj2_showTitleScreen(GFX *gfx_p, App_proj2 *app_p)
{
    static bool isRunning = false;
    if (isRunning == false)
    { // Print out the initial graphics once and the dinosaur
        GFX_clear(gfx_p);
        GFX_print(gfx_p, "Fall 2024 Proj 2", 5, 3);
        GFX_print(gfx_p, "Flappy Bird", 6, 5);
        GFX_print(gfx_p, "Aiden Wiehn", 7, 5);
        isRunning = true;
    }
}

// Graphics that show after the title screen
// 5 Lines
void App_proj2_showMenuScreen(App_proj2 *app_p, GFX *gfx_p)
{
    GFX_clear(gfx_p);
    GFX_print(gfx_p, "Play Flappy Bird", 6, 2);
    GFX_print(gfx_p, "View Instructions", 7, 2);
    GFX_print(gfx_p, "View High Scores", 8, 2);
}

// Graphics for the instructions screen
// 13 Lines
void App_proj2_showInstructionsScreen(App_proj2 *app_p, GFX *gfx_p)
{
    GFX_print(gfx_p, "INSTRUCTIONS", 0, 5);
    GFX_print(gfx_p, "---------------------", 1, 0);
    GFX_print(gfx_p, "Gain 500 points by", 2, 0);
    GFX_print(gfx_p, "avoiding each", 3, 0);
    GFX_print(gfx_p, "obstacle that comes", 4, 0);
    GFX_print(gfx_p, "your way. Getting hit", 5, 0);
    GFX_print(gfx_p, "makes you lose a", 6, 0);
    GFX_print(gfx_p, "life.", 7, 0);
    GFX_print(gfx_p, "Use JOYSTICK to move", 9, 0);
    GFX_print(gfx_p, "Use BB1 to jump", 10, 0);
    GFX_print(gfx_p, "Press JSB to return", 14, 1);
    GFX_print(gfx_p, "to the main menu", 15, 3);
}

// Graphics for the lines, lives, and score for the game
// 10 Lines
void App_proj2_showGameScreen(App_proj2 *app_p, GFX *gfx_p)
{
    GFX_print(gfx_p, "Score", 1, 5);
    Graphics_drawLine(&gfx_p->context, BORDER_LEFT, DIVIDER_TOP, BORDER_RIGHT,
    DIVIDER_TOP);
    Graphics_drawLine(&gfx_p->context, BORDER_LEFT, DIVIDER_BOTTOM, BORDER_RIGHT,
    DIVIDER_BOTTOM);
    GFX_print(gfx_p, "LIVES", 14, 5);
    GFX_fillRectangle(gfx_p, LIFEONE_XMIN, LIFE_YMIN, LIFEONE_XMAX, LIFE_YMAX);
    GFX_fillRectangle(gfx_p, LIFETWO_XMIN, LIFE_YMIN, LIFETWO_XMAX, LIFE_YMAX);
    GFX_fillRectangle(gfx_p, LIFETHREE_XMIN, LIFE_YMIN, LIFETHREE_XMAX,
    LIFE_YMAX);
}

// Graphics for the high score screen
// 10 Lines
void App_proj2_showResultScreen(App_proj2 *app_p, GFX *gfx_p)
{
    GFX_print(gfx_p, "HIGH SCORES", 0, 5);
    GFX_print(gfx_p, "1: ", 3, 7);
    GFX_print(gfx_p, app_p->highScoreOne, 3, 9);
    GFX_print(gfx_p, "2: ", 5, 7);
    GFX_print(gfx_p, app_p->highScoreTwo, 5, 9);
    GFX_print(gfx_p, "3: ", 7, 7);
    GFX_print(gfx_p, app_p->highScoreThree, 7, 9);
    GFX_print(gfx_p, "Press JSB to return", 14, 1);
    GFX_print(gfx_p, "to the main menu", 15, 3);
}

// Graphics that show when all lives are lost in the game
// 6 Lines
void App_proj2_showFinalScreen(App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p)
{
    GFX_print(&hal_p->gfx, "GAME OVER", 1, 6);
    GFX_print(&hal_p->gfx, "Score:", 5, 4);
    GFX_print(&hal_p->gfx, app_p->currentScore, 5, 10);
    GFX_print(&hal_p->gfx, "Press JSB to return", 14, 1);
    GFX_print(&hal_p->gfx, "to the main menu", 15, 3);
}

#endif /* CODE_SHOW_H_ */
