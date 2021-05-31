#include <pio.h>
#include "target.h"
#include "pacer.h"
#include "ledbuffer.h"

#define NUM_LEDS 28

ledbuffer_t* ledt_init(void)
{
    ledbuffer_t* ledsr = ledbuffer_init(LEDTAPE_PIO, NUM_LEDS);
    ledbuffer_clear(ledsr);
    ledbuffer_set(ledsr, 0, 0, 100, 0);
    ledbuffer_set(ledsr, 1, 14, 94, 0);
    ledbuffer_set(ledsr, 2, 29, 87, 0);
    ledbuffer_set(ledsr, 3, 43, 80, 0);
    ledbuffer_set(ledsr, 4, 58, 72, 0);
    ledbuffer_set(ledsr, 5, 72, 65, 0);
    ledbuffer_set(ledsr, 6, 87, 58, 0);
    ledbuffer_set(ledsr, 7, 102, 51, 0);
    ledbuffer_set(ledsr, 8, 87, 58, 0);
    ledbuffer_set(ledsr, 9, 72, 65, 0);
    ledbuffer_set(ledsr, 10, 58, 72, 0);
    ledbuffer_set(ledsr, 11, 43, 80, 0);
    ledbuffer_set(ledsr, 12, 29, 87, 0);
    ledbuffer_set(ledsr, 13, 14, 94, 0);
    ledbuffer_set(ledsr, 14, 0, 100, 0);
    ledbuffer_set(ledsr, 15, 14, 94, 0);
    ledbuffer_set(ledsr, 16, 29, 87, 0);
    ledbuffer_set(ledsr, 17, 43, 80, 0);
    ledbuffer_set(ledsr, 18, 58, 72, 0);
    ledbuffer_set(ledsr, 19, 72, 65, 0);
    ledbuffer_set(ledsr, 20, 87, 58, 0);
    ledbuffer_set(ledsr, 21, 102, 51, 0);
    ledbuffer_set(ledsr, 22, 87, 58, 0);
    ledbuffer_set(ledsr, 23, 72, 65, 0);
    ledbuffer_set(ledsr, 24, 58, 72, 0);
    ledbuffer_set(ledsr, 25, 43, 80, 0);
    ledbuffer_set(ledsr, 26, 29, 87, 0);
    ledbuffer_set(ledsr, 27, 14, 94, 0);
    return(ledsr);
    
}



void ledt_run(ledbuffer_t* ledsr)
{
    ledbuffer_advance(ledsr, 1);
    ledbuffer_write(ledsr);
}