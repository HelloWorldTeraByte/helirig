#ifndef __LED_TAPE__H
#define __LED_TAPE__H

enum ledt_state {LEDT_NORMAL, LEDT_APE, LEDT_HIT};

void ledt_init(void);
void ledt_run(void);
void ledt_setmode(enum ledt_state st);

#endif