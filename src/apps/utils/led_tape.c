#include "pio.h"
#include "target.h"
#include "ledbuffer.h"
#include "led_tape.h"

static ledbuffer_t* ledsr;
static uint16_t count = 0;

static enum ledt_state state_curr = LEDT_NORMAL;

void ledt_init(void)
{
    ledsr = ledbuffer_init(LEDTAPE_PIO, NUM_LED_TAPE);
    ledt_setmode(LEDT_NORMAL);
}

void ledt_junglejam(void)
{
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
}

void ledt_apemode(void)
{
    const uint8_t length = 18;
    uint8_t i;
    for(i = 0; i < NUM_LED_TAPE; i++) {
        if(i <= length)
            ledbuffer_set(ledsr, i, 200, i, 0);
        else
            ledbuffer_set(ledsr, i, 200, (NUM_LED_TAPE)-(i-length), 0);
    }
}

void ledt_hit(void)
{
    uint8_t i;
    for(i = 0; i < NUM_LED_TAPE; i++) {
        ledbuffer_set(ledsr, i, 100, 0, 0);
    }
}

void ledt_setmode(enum ledt_state st)
{
    state_curr = st;

    ledbuffer_clear(ledsr);
    if(state_curr == LEDT_NORMAL)
        ledt_junglejam();
    else if(state_curr == LEDT_APE)
        ledt_apemode();
    else if(state_curr == LEDT_HIT)
        ledt_hit();
}
void ledt_run(void)
{
    ledbuffer_write(ledsr);
    ledbuffer_advance(ledsr, 1);
}