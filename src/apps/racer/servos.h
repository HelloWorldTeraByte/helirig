#ifndef __SERVOS__H
#define __SERVOS__H

#include "stdint.h"

#define SERVO_PWM_FREQ_HZ 10e3

void servos_init(void);
void servo1_duty_set(uint8_t duty);

#endif