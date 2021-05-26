#ifndef __BUTTONS__H
#define __BUTTONS__H

#include "stdint.h"

void btns_init(void);
void btns_update(void);
uint8_t is_btn0_pressed(void);

#endif