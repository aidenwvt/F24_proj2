/*
 * logic.h
 *
 *  Created on: Sep 28, 2024
 *      Author: aiden
 */

#ifndef CODE_LOGIC_H_
#define CODE_LOGIC_H_

#include <HAL/HAL.h>
#include <Code/proj2_app.h>
#include <stdlib.h>

// Function for taking an array of ints, and turns it into
// an array of chars
// 7 Lines
void make_6digit_NumString(unsigned int num, char *string)
{
    string[0] = (num / 100000) % 10 + '0';
    string[1] = (num / 10000) % 10 + '0';
    string[2] = (num / 1000) % 10 + '0';
    string[3] = (num / 100) % 10 + '0';
    string[4] = (num / 10) % 10 + '0';
    string[5] = num % 10 + '0';
}

// Function responsible for moving the square in both the X and Y directions
// Also responsible for the jumping logic
// 34 Lines
void moveSquare(GFX *gfx_p, HAL *hal_p, App_proj2 *app_p)
{
    static bool timerStarted = false;
    // Set the graphics to black and draw over the current square
    GFX_setForeground(gfx_p, GRAPHICS_COLOR_BLACK);
    GFX_fillRectangle(gfx_p, app_p->minX, app_p->minY, app_p->maxX,
                      app_p->maxY);

    // Logic for determining which direction to move the player
    if (Joystick_isPressedToRight(&hal_p->joystick) && app_p->maxX < 127)
    {
        float x = return_Joystick_X(&hal_p->joystick);
        app_p->minX += (0.9 / 6000) * (x - 10000) + 0.1;
        app_p->maxX += (0.9 / 6000) * (x - 10000) + 0.1;
    }
    else if (Joystick_isPressedToLeft(&hal_p->joystick) && app_p->minX > 0)
    {
        float x2 = return_Joystick_X(&hal_p->joystick);
        app_p->minX -= (0.9 / 5000) * (5000 - x2) + 0.1;
        app_p->maxX -= (0.9 / 5000) * (5000 - x2) + 0.1;
    }

    // If BB1 is pressed, begin making the player jump, and lock the player from switching lanes
    if (Button_isTapped(&hal_p->boosterpackS1) || !SWTimer_expired(&app_p->timer))
    {
       if (timerStarted == false) {
       app_p->timer = SWTimer_construct(JUMP);
       SWTimer_start(&app_p->timer);
       timerStarted = true;
       }
       if (app_p->minY > 22) {
       app_p->minY = app_p->minY - 0.75;
       app_p->maxY = app_p->maxY - 0.75;
       }
    }
    else {
        if (timerStarted == true) {
            timerStarted = false;
        }
        if (app_p->maxY < 106) {
        app_p->minY = app_p->minY + 0.5;
        app_p->maxY = app_p->maxY + 0.5;
        }
    }
    // Change graphics back to white and draw the player again
    GFX_setForeground(gfx_p, GRAPHICS_COLOR_WHITE);
    GFX_fillRectangle(gfx_p, app_p->minX, app_p->minY, app_p->maxX,
                      app_p->maxY);
}

// Function responsible for spawning the obstacles as
// well as moving them across the screen
void obstacleSpawner(GFX *gfx_p, App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p)
{
    int continueCheck = -1;
    int i;
    static bool wait = false;

    // Iterate through all the obstacles, and stop on the first one that is current not moving
    for (i = 0; i < OBSTACLE_SIZE; i++)
    {
        if (!app_p->obstacles[i].moving)
        {
            continueCheck = i;
            break;
        }
    }

    // If the timer in charge of the delay in between objects is over, start a new timer
    if (!wait)
    {
        app_p->timer2 = SWTimer_construct(OBSTACLE_WAIT);
        SWTimer_start(&app_p->timer2);
        wait = true;
    }

    // If the wait between objects is over, set up a new object and enable it
    if (continueCheck != -1 && SWTimer_expired(&app_p->timer2))
    {
        for (i = 0; i < OBSTACLE_SIZE; i++)
        {
            Obstacle *newObstacle = &app_p->obstacles[continueCheck];
            obstacleTypeVal(app_p, hal_p, obj_p, continueCheck);
            newObstacle->obstacleTimer = SWTimer_construct(OBSTACLE_MOVE);
            newObstacle->moving = true;
            SWTimer_start(&newObstacle->obstacleTimer);
            wait = false;
        }
    }

    // Iterate through every single possible object and update their position and score
    for (i = 0; i < OBSTACLE_SIZE; i++)
    {
        Obstacle *currentObstacle = &app_p->obstacles[i];
        // If currently active, run all of this, if not, immediately move to the next object
        if (currentObstacle->moving)
        {
            double runTime = SWTimer_percentElapsed(
                    &currentObstacle->obstacleTimer);
            runTime = runTime * 1.35;
            currentObstacle->xMin = (1.1 - runTime) * 127.0;
            currentObstacle->xMax = (1.1 - runTime) * 127.0;
            // If off screen, disable the obstacle and add +500 points
            if (currentObstacle->xMax < -13)
            {
                currentObstacle->moving = false;
                app_p->score += 500;
            }
            // If still active, redraw the obstacle
            else
            {
                GFX_setForeground(gfx_p, GRAPHICS_COLOR_BLACK);
                GFX_fillRectangle(gfx_p, (1.19 - runTime) * 127.0,
                                  currentObstacle->yMin,
                                  (1.11 - runTime) * 127.0 + 10,
                                  currentObstacle->yMax);
                GFX_fillRectangle(gfx_p, (1.19 - runTime) * 127.0,
                                  currentObstacle->yMin2,
                                  (1.11 - runTime) * 127.0 + 10,
                                  currentObstacle->yMax2);
                GFX_setForeground(gfx_p, GRAPHICS_COLOR_WHITE);
                GFX_fillRectangle(gfx_p, currentObstacle->xMin,
                                  currentObstacle->yMin, currentObstacle->xMax,
                                  currentObstacle->yMax);
                GFX_fillRectangle(gfx_p, currentObstacle->xMin,
                                  currentObstacle->yMin2, currentObstacle->xMax,
                                  currentObstacle->yMax2);
            }
        }
    }
    // Had an issue with the score, these lines of code fixed it
    if (app_p->reset == false)
    {
        app_p->score = 000000;
        app_p->reset = true;
    }
}

// Randomly spawns in the obstacles with a gap in between each of them
void obstacleTypeVal(App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p, int i)
{
    Obstacle *newObstacle = &app_p->obstacles[i];

    // Define the gap size and the bounds for random gap position
    double gapHeight = 30.0; // Fixed gap size between the pipes
    double minGapY = 20.0;   // The minimum allowed y position (lower bound)
    double maxGapY = 107.0 - gapHeight; // The maximum allowed y position for the top of the gap

    // Randomize the vertical position of the gap
    double randomGapY = minGapY + (rand() / (double)RAND_MAX) * (maxGapY - minGapY); // Random position between 20 and 87 (since gapHeight is 20)

    // Set the bottom pipe's y values (lower obstacle)
    newObstacle->yMin = 21.0;
    newObstacle->yMax = randomGapY;

    // Set the top pipe's y values (upper obstacle)
    newObstacle->yMin2 = randomGapY + gapHeight;
    newObstacle->yMax2 = 106.0;

    // Set the horizontal position for both obstacles (they move together)
    newObstacle->xMin = MAX_X;         // Start from the rightmost part of the screen
    newObstacle->xMax = MAX_X + 5.0;   // Define the width of the obstacles

    // Initialize the obstacle movement and timer logic
    newObstacle->moving = true;
}

// Checks if the player has taken damage
void damageCheck(GFX *gfx_p, App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p)
{
    // Define player’s bounding box
       double playerXMin = app_p->minX;
       double playerXMax = app_p->maxX;
       double playerYMin = app_p->minY;
       double playerYMax = app_p->maxY;
       int i;

       // Iterate through each obstacle in the array
       for (i = 0; i < OBSTACLE_SIZE; i++)
       {
           Obstacle *obstacle = &app_p->obstacles[i];

           // Check if player collides with the bottom pipe
           bool collidesWithBottom =
               (playerXMax >= obstacle->xMin && playerXMin <= obstacle->xMax) &&  // Overlapping on the X-axis
               (playerYMax >= obstacle->yMin && playerYMin <= obstacle->yMax);    // Overlapping on the Y-axis (bottom pipe)

           // Check if player collides with the top pipe
           bool collidesWithTop =
               (playerXMax >= obstacle->xMin && playerXMin <= obstacle->xMax) &&  // Overlapping on the X-axis
               (playerYMax >= obstacle->yMin2 && playerYMin <= obstacle->yMax2);  // Overlapping on the Y-axis (top pipe)

           // If either collision is true, the player has collided with an obstacle
           if ((collidesWithBottom || collidesWithTop) && SWTimer_expired(&app_p->iFrames))
           {
               app_p->iFrames = SWTimer_construct(IFRAMES);
               SWTimer_start(&app_p->iFrames);
               GFX_setForeground(gfx_p, GRAPHICS_COLOR_BLACK);
               GFX_fillRectangle(gfx_p, 95 + (10 * (app_p->lives - 3)), 112, 100 + (10 * (app_p->lives - 3)), 117);
               GFX_setForeground(gfx_p, GRAPHICS_COLOR_WHITE);
               app_p->lives--;
               break; // Exit the loop after detecting collision with one obstacle
           }
       }

       if (app_p->lives == 0)
       {
           GFX_clear(gfx_p);
           app_p->state = FINAL_SCREEN;
       }
}

// Calculates if the score in the current round is higher than
// any previous scores
void calculateHighScore(App_proj2 *app_p, HAL *hal_p)
{
    if (app_p->score > app_p->highScores[0])
    {
        app_p->highScores[2] = app_p->highScores[1];
        app_p->highScores[1] = app_p->highScores[0];
        app_p->highScores[0] = app_p->score;
    }
    else if (app_p->score > app_p->highScores[1])
    {
        app_p->highScores[2] = app_p->highScores[1];
        app_p->highScores[1] = app_p->score;
    }
    else if (app_p->score > app_p->highScores[2])
    {
        app_p->highScores[2] = app_p->score;
    }

    make_6digit_NumString(app_p->highScores[0], app_p->highScoreOne);
    make_6digit_NumString(app_p->highScores[1], app_p->highScoreTwo);
    make_6digit_NumString(app_p->highScores[2], app_p->highScoreThree);
}


#endif /* CODE_LOGIC_H_ */
