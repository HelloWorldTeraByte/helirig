#ifndef __BUMPER__H
#define __BUMPER__H

#include "mcu.h"

#define BUMPER_TIMEOUT 10*LOOP_RATE_U

void bumper_init(void);
void bumper_update(void);
void bumper_timeout_update(void);
uint8_t bumper_status_get(void);
uint8_t bumper_is_hit(void);

#endif