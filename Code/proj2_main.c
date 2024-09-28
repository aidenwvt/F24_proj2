/*
 * Prac.c
 *
 *  Header Created: August 28th, 2024
 *      Author: Aiden Wiehn
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <HAL/HAL.h>
#include <Code/proj2_app.h>
#include <Code/handle.h>
#include <Code/logic.h>
#include <Code/show.h>
#include <stdlib.h>

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void InitNonBlockingLED()
{
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
}

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void PollNonBlockingLED()
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 0)
    {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }
}

int main(void)
{
    // Stop Watchdog Timer - THIS SHOULD ALWAYS BE THE FIRST LINE OF YOUR MAIN
    WDT_A_holdTimer();

    // Initialize the system clock and background hardware timer, used to enable
    // software timers to time their measurements properly.
    InitSystemTiming();
    InitNonBlockingLED();

    // Initialize the main Application object and the HAL.
    HAL hal = HAL_construct();
    App_proj2 app = App_proj2_construct(&hal);
    Obstacle obj = App_obstacle_construct(&hal);

    // Main super-loop! In a polling architecture, this function should call
    // your main FSM function over and over.
    while (true)
    {
        PollNonBlockingLED();
        App_proj2_loop(&app, &hal, &obj); //update my program, application state, output
        HAL_refresh(&hal); // check the inputs
    }
}

App_proj2 App_proj2_construct(HAL *hal_p)
{
    App_proj2 app;
    app.timer = SWTimer_construct(TITLE_SCREEN_TIMER);
    SWTimer_start(&app.timer);
    app.state = TITLE_SCREEN;
    app.cursor = CURSOR_0;
    app.lives = LIVES;
    app.score = SCORE;
    app.minX = MIN_X;
    app.maxX = MAX_X;
    app.minY = MIN_Y;
    app.maxY = MAX_Y;
    app.reset = false;
    app.highScores[0] = app.highScores[1] = app.highScores[2] = 000000;
        int i;
        for (i = 0; i < 7; i++)
        {
            app.currentScore[i] = app.highScoreOne[i] = app.highScoreTwo[i] =
                    app.highScoreThree[i] = '0';
        }
        app.currentScore[6] = app.highScoreOne[6] = app.highScoreTwo[6] =
                app.highScoreThree[6] = '\0';
    return app;
}

// Initializing all obstacle variables here
Obstacle App_obstacle_construct(HAL *hal_p)
{
    Obstacle obj;
    obj.xMin = 127.0;
    obj.xMax = 137.0;
    obj.yMin = 68.0;
    obj.yMax = 74.0;
    obj.yMin2 = 0;
    obj.yMin2 = 0;
    obj.moving = false;
    return obj;
}

// The main super loop FSM in charge of all the game states
void App_proj2_loop(App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p)
{
    switch (app_p->state)
    {
    case TITLE_SCREEN:
        App_proj2_handleTitleScreen(app_p, hal_p);
        break;
    case MENU_SCREEN:
        App_proj2_handleMenuScreen(app_p, hal_p, &hal_p->gfx);
        break;
    case INSTRUCTIONS_SCREEN:
        App_proj2_handleInstructionsScreen(app_p, hal_p);
        break;
    case RESULTS_SCREEN:
        App_proj2_handleResultScreen(app_p, hal_p);
        break;
    case GAME_SCREEN:
        App_proj2_handleGameScreen(app_p, hal_p, obj_p);
        break;
    case FINAL_SCREEN:
        App_proj2_handleFinalScreen(app_p, hal_p, obj_p);
    }
}
