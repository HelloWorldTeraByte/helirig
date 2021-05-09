// *******************************************************
// 
// buttons4.c
//
// Support for a set of FOUR specific buttons on the Tiva/Orbit.
// ENCE361 sample code.
// The buttons are:  UP and DOWN (on the Orbit daughterboard) plus
// LEFT and RIGHT on the Tiva.
//
// Note that pin PF0 (the pin for the RIGHT pushbutton - SW2 on
//  the Tiva board) needs special treatment - See PhilsNotesOnTiva.rtf.
//
// P.J. Bones UCECE
// Last modified:  7.2.2018
//
// Modified by Hank Wu at 12.05.2020
// - implement long press. It will return HOLD by the checkButton function if detected a long press.
// - configurable long press threshold.
// - added Switches functions: call updateSwitch function to update the state of switches.
// 
// *******************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "inc/tm4c123gh6pm.h"  // Board specific defines (for PF0)
#include "buttons4.h"
#include "system.h"

// *******************************************************
// Globals to module
// *******************************************************
static bool but_state[NUM_BUTS];	// Corresponds to the electrical state
static uint8_t but_count[NUM_BUTS];
static bool but_flag[NUM_BUTS];
static bool but_long_press_flag[NUM_BUTS];
static bool but_normal[NUM_BUTS];   // Corresponds to the electrical state
static long time_stamps[NUM_BUTS];
static int time_diff[NUM_BUTS];
static long max_count;
static long threshold_count;

static bool sw_state[NUM_SWS];    // Corresponds to the electrical state
static uint8_t sw_count[NUM_SWS];


// *******************************************************
// initThresholdCount: convert the long press threshold in ms(0.001 second) to counts.
static void initThresholdCount(void){
    threshold_count = SystemTimerTickPeriodGet()*long_press_threshold;
}

// *******************************************************
// initButtons: Initialise the variables associated with the set of buttons
// defined by the constants in the buttons2.h header file.
void
initButtons (void)
{
    initThresholdCount();
	int i;

	// UP button (active HIGH)
    SysCtlPeripheralEnable (UP_BUT_PERIPH);
    GPIOPinTypeGPIOInput (UP_BUT_PORT_BASE, UP_BUT_PIN);
    GPIOPadConfigSet (UP_BUT_PORT_BASE, UP_BUT_PIN, GPIO_STRENGTH_2MA,
       GPIO_PIN_TYPE_STD_WPD);
    but_normal[UP] = UP_BUT_NORMAL;
	// DOWN button (active HIGH)
    SysCtlPeripheralEnable (DOWN_BUT_PERIPH);
    GPIOPinTypeGPIOInput (DOWN_BUT_PORT_BASE, DOWN_BUT_PIN);
    GPIOPadConfigSet (DOWN_BUT_PORT_BASE, DOWN_BUT_PIN, GPIO_STRENGTH_2MA,
       GPIO_PIN_TYPE_STD_WPD);
    but_normal[DOWN] = DOWN_BUT_NORMAL;
    // LEFT button (active LOW)
    SysCtlPeripheralEnable (LEFT_BUT_PERIPH);
    GPIOPinTypeGPIOInput (LEFT_BUT_PORT_BASE, LEFT_BUT_PIN);
    GPIOPadConfigSet (LEFT_BUT_PORT_BASE, LEFT_BUT_PIN, GPIO_STRENGTH_2MA,
       GPIO_PIN_TYPE_STD_WPU);
    but_normal[LEFT] = LEFT_BUT_NORMAL;
    // RIGHT button (active LOW)
      // Note that PF0 is one of a handful of GPIO pins that need to be
      // "unlocked" before they can be reconfigured.  This also requires
      //      #include "inc/tm4c123gh6pm.h"
    SysCtlPeripheralEnable (RIGHT_BUT_PERIPH);
    //---Unlock PF0 for the right button:
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTF_CR_R |= GPIO_PIN_0; //PF0 unlocked
    GPIO_PORTF_LOCK_R = GPIO_LOCK_M;
    GPIOPinTypeGPIOInput (RIGHT_BUT_PORT_BASE, RIGHT_BUT_PIN);
    GPIOPadConfigSet (RIGHT_BUT_PORT_BASE, RIGHT_BUT_PIN, GPIO_STRENGTH_2MA,
       GPIO_PIN_TYPE_STD_WPU);
    but_normal[RIGHT] = RIGHT_BUT_NORMAL;

    max_count = SystemTimerMaxValueGet();

	for (i = 0; i < NUM_BUTS; i++)
	{
		but_state[i] = but_normal[i];
		but_count[i] = 0;
		but_flag[i] = false;
		but_long_press_flag[i] = false;
		time_stamps[i] = -1;
		time_diff[i] = 0;
	}
}

//initSwitchs: initialise GPIO of SW1 and SW2
void initSwitchs(void){
    // LSW slide switch
    SysCtlPeripheralEnable (LSW_PERIPH);
    GPIOPinTypeGPIOInput (LSW_PORT_BASE, LSW_PIN);
    GPIOPadConfigSet (LSW_PORT_BASE, LSW_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);

    // RSW slide switch
    SysCtlPeripheralEnable (RSW_PERIPH);
    GPIOPinTypeGPIOInput (RSW_PORT_BASE, RSW_PIN);
    GPIOPadConfigSet (RSW_PORT_BASE, RSW_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);

}

//updateSwitchs: a background task, periodically updates what the value for the switches are.
void
updateSwitchs (void)
{
    bool sw_value[NUM_SWS];
    int i;

    // Read the pins; true means HIGH, false means LOW
    sw_value[RSW] = (GPIOPinRead (LSW_PORT_BASE, LSW_PIN) == LSW_PIN);
    sw_value[LSW] = (GPIOPinRead (RSW_PORT_BASE, RSW_PIN) == RSW_PIN);
    // Iterate through the buttons, updating button variables as required
    for (i = 0; i < NUM_SWS; i++)
    {
        if (sw_value[i] != sw_state[i])
        {
            sw_count[i]++;
            if (sw_count[i] >= NUM_BUT_POLLS)
            {
                sw_state[i] = sw_value[i];
                sw_count[i] = 0;
            }
        } else{
            sw_count[i] = 0;
        }
    }
}

//checkSwitchs: returns what the state for the specified switch is. UP or DOWN
uint8_t
checkSwitchs (uint8_t swName)
{
    if (sw_state[swName])
        return UP;
    else
        return DOWN;
}

// *******************************************************
// updateButtons: Function designed to be called regularly. It polls all
// buttons once and updates variables associated with the buttons if
// necessary.  It is efficient enough to be part of an ISR, e.g. from
// a SysTick interrupt.
// Debounce algorithm: A state machine is associated with each button.
// A state change occurs only after NUM_BUT_POLLS consecutive polls have
// read the pin in the opposite condition, before the state changes and
// a flag is set.  Set NUM_BUT_POLLS according to the polling rate.
void
updateButtons (void)
{
	bool but_value[NUM_BUTS];
	int i;
	
	// Read the pins; true means HIGH, false means LOW
	but_value[UP] = (GPIOPinRead (UP_BUT_PORT_BASE, UP_BUT_PIN) == UP_BUT_PIN);
	but_value[DOWN] = (GPIOPinRead (DOWN_BUT_PORT_BASE, DOWN_BUT_PIN) == DOWN_BUT_PIN);
    but_value[LEFT] = (GPIOPinRead (LEFT_BUT_PORT_BASE, LEFT_BUT_PIN) == LEFT_BUT_PIN);
    but_value[RIGHT] = (GPIOPinRead (RIGHT_BUT_PORT_BASE, RIGHT_BUT_PIN) == RIGHT_BUT_PIN);
	// Iterate through the buttons, updating button variables as required
	for (i = 0; i < NUM_BUTS; i++)
	{
        if (but_value[i] != but_state[i])
        {
        	but_count[i]++;
        	if (but_count[i] >= NUM_BUT_POLLS)
        	{
        		but_state[i] = but_value[i];
        		but_flag[i] = true;	   // Reset by call to checkButton()
        		but_count[i] = 0;

        		if(but_state[i] != but_normal[i]){  //when the button has been pressed, record the time stamp
        		    time_stamps[i] = SystemTimerValueGet();
        		}else{ //when the button released, reset the time stamp.
        		    time_stamps[i] = -1;
        		}
        	}
        }
        else{
        	but_count[i] = 0;
        	if (time_stamps[i] != -1){ // if the time stamp is not -1, calculate the time time difference.
        	    time_diff[i] = time_stamps[i] - SystemTimerValueGet();
        	    if (time_diff[i] < 0){ //overflow case
        	        if((max_count + time_diff[i])> threshold_count){
        	            but_long_press_flag[i] = true; //long press flag reset by calling check button.
        	        }

        	    }else{ //normal case
        	        if(time_diff[i] > threshold_count){
                        but_long_press_flag[i] = true;
                    }

        	    }
        	}
        }
	}
}



// *******************************************************
// checkButton: Function returns the new button logical state if the button
// logical state (PUSHED or RELEASED) has changed since the last call,
// return HOLD if the button is been pressed for longer than the long press threshold.
// otherwise returns NO_CHANGE.
uint8_t
checkButton (uint8_t butName)
{
	if (but_flag[butName])
	{
		but_flag[butName] = false;
		if (but_state[butName] == but_normal[butName])
			return RELEASED;
		else
			return PUSHED;
	}else if (but_long_press_flag[butName]){
	    but_long_press_flag[butName] = false;
	    return HOLD;
	}
	return NO_CHANGE;
}

