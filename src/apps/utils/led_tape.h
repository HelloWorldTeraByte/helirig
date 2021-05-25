#ifndef __LED_TAPE__H
#define __LED_TAPE__H

#include "stdint.h"
#include "ledbuffer.h"

ledbuffer_t* ledt_init(void);
void ledt_run(bool ape, ledbuffer_t* ledsr);

#endif