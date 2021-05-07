#ifndef __MOTORS__H
#define __MOTORS__H

#include "target.h"
#include "pwm.h"

void motors_init(void);
void motor_left_set(int8_t speed);
void motor_right_set(int8_t speed);

#endif