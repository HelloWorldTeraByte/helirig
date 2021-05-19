#ifndef __RADIO_COMM__H
#define __RADIO_COMM__H

#include "stdint.h"

static void panic(void);
void radio_init(void);

void radio_read(char *buffer, uint8_t len);
int8_t radio_write(char *buffer, uint8_t len);

#endif

