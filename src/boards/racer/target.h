/** @file   target.h
    @author M. P. Hayes, UCECE
    @date   12 February 2018
    @brief 
*/
#ifndef TARGET_H
#define TARGET_H

#include "mat91lib.h"

/* This is for the carhat (chart) board configured as a racer!  */

/* System clocks  */
#define F_XTAL 12.00e6
#define MCU_PLL_MUL 16
#define MCU_PLL_DIV 1

#define MCU_USB_DIV 2
/* 192 MHz  */
#define F_PLL (F_XTAL / MCU_PLL_DIV * MCU_PLL_MUL)
/* 96 MHz  */
#define F_CPU (F_PLL / 2)

/* Define how fast ticks occur.  This must be faster than
   TICK_RATE_MIN.  */
#define LOOP_POLL_RATE  200

/* Define Motor speed update rate in Hz.  */
#define MOTOR_UPDATE_RATE 10
#define RADIO_POLL_RATE 1

#define LOOP_RATE_M 1
#define LOOP_RATE_U 10
#define LOOP_RATE_N 100

#define LOOP_RATE_RF_RX 20
#define LOOP_RATE_RF_TX 10

#define SPEED_RX_TIMEOUT 20

/* TWI  */
#define TWI_TIMEOUT_US_DEFAULT 10000

/* USB  */
#define USB_VBUS_PIO PA5_PIO
#define USB_CURRENT_MA 500

/* LEDs  */
#define LED_STAT0 PA4_PIO
#define LED1_PIO PA4_PIO
#define LED_STAT1 PA3_PIO
#define LED_LOW_BATT PA2_PIO

/* General  */
#define NUM_LED_TAPE 28

//#define SERVO_PWM_PIO PA25_PIO
#define LEDTAPE_PIO PA11_PIO 
//#define APPENDAGE_PIO PA1_PIO
#define SERVO_PWM_PIO PA17_PIO

/* Button  */
#define BUTTON_WAKEUP_PIO PA8_PIO

#define BUMPER_PIO PA31_PIO

/* H-bridges   */
#define MOTOR_LEFT_PWM_PIO_0 PA23_PIO
#define MOTOR_LEFT_PWM_PIO_1 PA0_PIO
#define MOTOR_RIGHT_PWM_PIO_0 PB1_PIO
#define MOTOR_RIGHT_PWM_PIO_1 PA1_PIO
#define MOTOR_SLEEP_PIO PA7_PIO

/* Radio  */
#define RADIO_CS_PIO PA24_PIO
#define RADIO_CE_PIO PA26_PIO
#define RADIO_IRQ_PIO PA15_PIO

/* Radio Jumper Channels */
#define RADIO_JUMPER1 PA30_PIO
#define RADIO_JUMPER2 PA29_PIO
#define RADIO_JUMPER3 PA28_PIO
#define RADIO_JUMPER4 PA27_PIO

#endif /* TARGET_H  */