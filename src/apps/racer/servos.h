#ifndef __SERVOS__H
#define __SERVOS__H

#include "stdint.h"

#define SERVO_PWM_FREQ_HZ 50
#define DROP_DURATION 3*LOOP_RATE_U
#define SERVO_OPEN_POS 95
#define SERVO_CLOSE_POS 90 

void servos_init(void);
void servo1_duty_set(uint8_t duty);
void servo_update(void);
void servo_drop_bananas(void);

#endif