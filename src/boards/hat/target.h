/** @file   target.h
    @author M. P. Hayes, UCECE
    @date   12 February 2018
    @brief
*/
#ifndef TARGET_H
#define TARGET_H

#include "mat91lib.h"

/* This is for the carhat (chart) board configured as a hat!  */

/* System clocks  */
#define F_XTAL 12.00e6
#define MCU_PLL_MUL 16
#define MCU_PLL_DIV 1

#define MCU_USB_DIV 2
/* 192 MHz  */
#define F_PLL (F_XTAL / MCU_PLL_DIV * MCU_PLL_MUL)
/* 96 MHz  */
#define F_CPU (F_PLL / 2)

/* TWI  */
#define TWI_TIMEOUT_US_DEFAULT 10000
#define MPU_ADDRESS 0x68

/* USB  */
//#define USB_VBUS_PIO PA5_PIO
#define USB_CURRENT_MA 500

/* ADC  */
#define ADC_BATTERY PA20_PIO
#define ADC_JOYSTICK_X PA18_PIO
#define ADC_JOYSTICK_Y PA17_PIO

/* IMU  */
#define IMU_INT_PIO PA0_PIO

/* LEDs  */
#define LED1_PIO PA0_PIO
#define LED2_PIO PA1_PIO
#define LED3_PIO PA15_PIO
#define LED4_PIO PA16_PIO


#define LED_STATUS PA0_PIO
#define LED_LOW_BAT PA1_PIO
#define LED_ERROR PA15_PIO
#define LED_DEBUG PA16_PIO


/* General  */
#define NUM_LED_TAPE 29

#define APPENDAGE_PIO PA1_PIO
#define SERVO_PWM_PIO PA2_PIO

/* Button  */
#define BUTTON_PIO PA2_PIO
#define JOYSTICK_BUTTON_PIO PA19_PIO
#define BUTTON_SLEEP_PIO PA11_PIO

/* Radio  */
#define RADIO_CS_PIO PA25_PIO
#define RADIO_CE_PIO PA26_PIO
#define RADIO_IRQ_PIO PA24_PIO

#define RADIO_JUMPER1 PA31_PIO
#define RADIO_JUMPER2 PA30_PIO
#define RADIO_JUMPER3 PA29_PIO
#define RADIO_JUMPER4 PA28_PIO

#define PIEZO_PIO PB4_PIO
#define LEDTAPE_PIO PA10_PIO


#endif /* TARGET_H  */
