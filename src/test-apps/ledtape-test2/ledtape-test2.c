/* File:   ledtape-test2.c
   Author: B Mitchell, UCECE
   Date:   14 April 2021
   Descr:  Test ledtape
*/

#include <pio.h>
#include "target.h"
#include "pacer.h"
#include "ledbuffer.h"

#define NUM_LEDS 28

/*
    This is an alternative method for driving the LED tape using the ledbuffer
    module that is included in the ledtape driver.

    The buffer acts like a small framebuffer upon which you can set RGB values
    at specific positions (not GRB, it handles the translation automatically).
    It also makes it easy to make patterns, shuffle them allow the strip, and
    clear it later. See ledbuffer.h for more details (CTRL-Click it in VS Code).
*/

int
main (void)
{
    bool ape = false;
    int count = 0;
    int dpacer = 0;

    ledbuffer_t* leds = ledbuffer_init(LEDTAPE_PIO, NUM_LEDS);

    pacer_init(200);

    while (1)
    {
        pacer_wait();
        if (dpacer++ == 19){
            if (count++ == NUM_LEDS) {

                ledbuffer_clear(leds);
                ledbuffer_set(leds, 0, 0, 100, 0);
                ledbuffer_set(leds, NUM_LEDS / 8, 0, 100, 0);
                ledbuffer_set(leds, NUM_LEDS / 4, 102, 51, 0);
                ledbuffer_set(leds, 3 * NUM_LEDS / 8, 102, 51, 0);
                ledbuffer_set(leds, NUM_LEDS / 2, 0, 100, 0);
                ledbuffer_set(leds, 5 * NUM_LEDS / 8, 0, 100, 0);
                ledbuffer_set(leds, 3 * NUM_LEDS / 4, 102, 51, 0);
                ledbuffer_set(leds, 7 * NUM_LEDS / 8, 102, 51, 0);

                 ape = !ape;
            
                count = 0;
            }

            ledbuffer_write (leds);
            if (ape) {
                ledbuffer_advance (leds, -1);
            } else{
                ledbuffer_advance (leds, 1);
            }
            dpacer = 0;

        }

    }
}
