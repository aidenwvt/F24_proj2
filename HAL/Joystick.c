/*
 * Joystick.c
 *
 * Created on: Mar 25, 2024
 *     Author: Aiden Wiehn
 */

#include <HAL/Joystick.h>

#define PRESSED_LEFT 5000
#define PRESSED_RIGHT 10000
#define PRESSED_UP 12000
#define PRESSED_DOWN 3000

enum _JoystickLeftDebounceState
{
    LEFT, NOT_LEFT
};
typedef enum _JoystickLeftDebounceState JoystickLeftDebounceState;
enum _JoystickUpDebounceState
{
    UP, NOT_UP
};
typedef enum _JoystickUpDebounceState JoystickUpDebounceState;
enum _JoystickDownDebounceState
{
    DOWN, NOT_DOWN
};
typedef enum _JoystickDownDebounceState JoystickDownDebounceState;
enum _JoystickRightDebounceState
{
    RIGHT, NOT_RIGHT
};
typedef enum _JoystickRightDebounceState JoystickRightDebounceState;

void initADC()
{
    ADC14_enableModule();

    ADC14_initModule(ADC_CLOCKSOURCE_SYSOSC,
    ADC_PREDIVIDER_1,
                     ADC_DIVIDER_1, 0);

    // This configures the ADC to store output results
    // in ADC_MEM0 for joystick X.
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);

    // This configures the ADC in manual conversion mode
    // Software will start each conversion.
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
}

void startADC()
{
    // Starts the ADC with the first conversion
    // in repeat-mode, subsequent conversions run automatically
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();
}

// Interfacing the Joystick with ADC (making the proper connections in software)
void initJoyStick()
{

    // This configures ADC_MEM0 to store the result from
    // input channel A15 (Joystick X), in non-differential input mode
    // (non-differential means: only a single input pin)
    // The reference for Vref- and Vref+ are VSS and VCC respectively
    ADC14_configureConversionMemory(ADC_MEM0,
    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A15,                 // joystick X
                                    ADC_NONDIFFERENTIAL_INPUTS);

    // This selects the GPIO as analog input
    // A15 is multiplexed on GPIO port P6 pin PIN0
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
    GPIO_PIN0,
                                               GPIO_TERTIARY_MODULE_FUNCTION);

    ADC14_configureConversionMemory(ADC_MEM1,
    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A9,                 // joystick Y
                                    ADC_NONDIFFERENTIAL_INPUTS);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
    GPIO_PIN4,
                                               GPIO_TERTIARY_MODULE_FUNCTION);

}

Joystick Joystick_construct()
{
    // The Joystick object which will be returned at the end of construction
    Joystick Joystick;

    initADC();
    initJoyStick();
    startADC();

    // Initialize all buffered outputs of the Joystick
//    Joystick.pushState = RELEASED;
//    Joystick.isTapped = false;

    // Return the constructed Joystick object to the user
    return Joystick;
}

// Refreshes the joystick input
// 3 Lines
void Joystick_refresh(Joystick *joystick_p)
{
    joystick_p->x = ADC14_getResult(ADC_MEM0);
    joystick_p->y = ADC14_getResult(ADC_MEM1);
}

// Returns the x position of the joystick
// 2 Lines
int return_Joystick_X(Joystick *joystick_p)
{
    return joystick_p->x;
}

// Returns the y position of the joystick
// 2 Lines
int return_Joystick_Y(Joystick *joystick_p)
{
    return joystick_p->y;
}

// Checks if joystick is pressed left
// 2 Lines
bool Joystick_isPressedToLeft(Joystick *joystick_p)
{
    return (joystick_p->x < PRESSED_LEFT);
}

// Checks if joystick is pressed right
// 2 Lines
bool Joystick_isPressedToRight(Joystick *joystick_p)
{
    return (joystick_p->x > PRESSED_RIGHT);
}

// Checks if joystick is pressed up
// 2 Lines
bool Joystick_isPressedToUp(Joystick *joystick_p)
{
    return (joystick_p->y > PRESSED_UP);
}

// Checks if joystick is pressed down
// 2 Lines
bool Joystick_isPressedToDown(Joystick *joystick_p)
{
    return (joystick_p->y < PRESSED_DOWN);
}

// Checks if joystick is tapped left
// 14 Lines
bool Joystick_isTappedToLeft(Joystick *joystick_p)
{
    static JoystickLeftDebounceState state = NOT_LEFT;
    bool output = false;

    switch (state)
    {

    case NOT_LEFT:
        if (joystick_p->x < PRESSED_LEFT)
        {
            state = LEFT;
            output = true;
        }
        break;

    case LEFT:
        if (joystick_p->x > PRESSED_LEFT)
        {
            state = NOT_LEFT;
            output = false;
        }
    }
    return output;
}

// Checks if joystick is tapped right
// 14 Lines
bool Joystick_isTappedToRight(Joystick *joystick_p)
{
    static JoystickRightDebounceState state = NOT_RIGHT;
    bool output = false;

    switch (state)
    {

    case NOT_RIGHT:
        if (joystick_p->x < PRESSED_RIGHT)
        {
            state = RIGHT;
            output = true;
        }
        break;

    case RIGHT:
        if (joystick_p->x > PRESSED_RIGHT)
        {
            state = NOT_RIGHT;
            output = false;
        }
    }
    return output;
}

// Checks if joystick is tapped up
// 14 Lines
bool Joystick_isTappedToUp(Joystick *joystick_p)
{
    static JoystickUpDebounceState state = NOT_UP;
    bool output = false;

    switch (state)
    {

    case NOT_UP:
        if (joystick_p->y > PRESSED_UP)
        {
            state = UP;
            output = true;
        }
        break;

    case UP:
        if (joystick_p->y < PRESSED_UP)
        {
            state = NOT_UP;
            output = false;
        }
    }
    return output;
}

// Checks if joystick is tapped down
// 14 Lines
bool Joystick_isTappedToDown(Joystick *joystick_p)
{
    static JoystickDownDebounceState state = NOT_DOWN;
    bool output = false;

    switch (state)
    {

    case NOT_DOWN:
        if (joystick_p->y < PRESSED_DOWN)
        {
            state = DOWN;
            output = true;
        }
        break;

    case DOWN:
        if (joystick_p->y > PRESSED_DOWN)
        {
            state = NOT_DOWN;
            output = false;
        }
    }
    return output;
}
