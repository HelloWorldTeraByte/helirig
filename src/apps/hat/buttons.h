#ifndef BUTTONS_H_
#define BUTTONS_H_

// *******************************************************
//
// buttons4.h
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

//*****************************************************************************
// Constants
//*****************************************************************************
enum butNames {UP = 0, DOWN, LEFT, RIGHT, NUM_BUTS};
enum swNames {RSW = 0, LSW, NUM_SWS};
enum butStates {RELEASED = 0, PUSHED, NO_CHANGE, HOLD};
// UP button
#define UP_BUT_PERIPH  SYSCTL_PERIPH_GPIOE
#define UP_BUT_PORT_BASE  GPIO_PORTE_BASE
#define UP_BUT_PIN  GPIO_PIN_0
#define UP_BUT_NORMAL  false
// DOWN button
#define DOWN_BUT_PERIPH  SYSCTL_PERIPH_GPIOD
#define DOWN_BUT_PORT_BASE  GPIO_PORTD_BASE
#define DOWN_BUT_PIN  GPIO_PIN_2
#define DOWN_BUT_NORMAL  false
// LEFT button
#define LEFT_BUT_PERIPH  SYSCTL_PERIPH_GPIOF
#define LEFT_BUT_PORT_BASE  GPIO_PORTF_BASE
#define LEFT_BUT_PIN  GPIO_PIN_4
#define LEFT_BUT_NORMAL  true
// RIGHT button
#define RIGHT_BUT_PERIPH  SYSCTL_PERIPH_GPIOF
#define RIGHT_BUT_PORT_BASE  GPIO_PORTF_BASE
#define RIGHT_BUT_PIN  GPIO_PIN_0
#define RIGHT_BUT_NORMAL  true
//LSW
#define LSW_PERIPH  SYSCTL_PERIPH_GPIOA
#define LSW_PORT_BASE  GPIO_PORTA_BASE
#define LSW_PIN  GPIO_PIN_7

//RSW
#define RSW_PERIPH  SYSCTL_PERIPH_GPIOA
#define RSW_PORT_BASE  GPIO_PORTA_BASE
#define RSW_PIN  GPIO_PIN_6

//timer
#define BUT_TIMER_PERIPH SYSCTL_PERIPH_WTIMER2
#define BUT_TIMER_BASE WTIMER2_BASE
#define BUT_TIMER TIMER_A


#define NUM_BUT_POLLS 3
// Debounce algorithm: A state machine is associated with each button.
// A state change occurs only after NUM_BUT_POLLS consecutive polls have
// read the pin in the opposite condition, before the state changes and
// a flag is set.  Set NUM_BUT_POLLS according to the polling rate.


#define long_press_threshold 500 //ms
// change this to configure the long press threshold in ms.


// *******************************************************
// initButtons: Initialise the variables associated with the set of buttons
// defined by the constants above.
void
initButtons (void);


// *******************************************************
// updateButtons: Function designed to be called regularly. It polls all
// buttons once and updates variables associated with the buttons if
// necessary.  It is efficient enough to be part of an ISR, e.g. from
// a SysTick interrupt.
void
updateButtons (void);


// *******************************************************
// checkButton: Function returns the new button state if the button state
// (PUSHED or RELEASED) has changed since the last call, otherwise returns
// NO_CHANGE.  The argument butName should be one of constants in the
// enumeration butStates, excluding 'NUM_BUTS'. Safe under interrupt.
uint8_t
checkButton (uint8_t butName);


// *******************************************************
// initSwitchs: Initialise the variables associated with the set of switches
// defined by the constants above.
void initSwitchs(void);


// *******************************************************
// updateSwitchs: Function designed to be called regularly. It polls all
// switches once and updates variables associated with the switches if
// necessary.  It is efficient enough to be part of an ISR, e.g. from
// a SysTick interrupt.
void
updateSwitchs (void);


// *******************************************************
// checkSwitchs: Function returns the new switch state if the switch state
// (UP or DOWN) has changed since the last call.
// The argument swName should be one of constants in the
// enumeration swStates, excluding 'NUM_SWS'. Safe under interrupt.
uint8_t
checkSwitchs (uint8_t swName);


#endif /*BUTTONS_H_*/
