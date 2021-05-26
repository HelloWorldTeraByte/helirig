#ifndef __MOTORS__H
#define __MOTORS__H

#include "stdint.h"

#define MOTOR_PWM_FREQ_HZ 10e3

void motors_init(void);
void motor_left_set(int8_t speed);
void motor_right_set(int8_t speed);

void motor_lock(void);
void motor_unlock(void);

void motors_wakeup(void);
void motors_sleep(void);

#endif