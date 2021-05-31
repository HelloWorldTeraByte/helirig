#include "stdint.h"
#include "state_manager.h"
#include "led_tape.h"

enum states racer_state = NORMAL;

static uint16_t ape_mode_t = 0;

void state_manager_init(void)
{
    go_normal_mode();
}

void go_ape_mode(void)
{
    racer_state = APE;
    servo_drop_bananas();
    bumper_set_timeout(0);
    ledt_setmode(LEDT_APE);
}

void go_normal_mode(void)
{
    racer_state = NORMAL;
    ledt_setmode(LEDT_NORMAL);
}

void state_manage(void)
{
    if(racer_state == APE) {
        ape_mode_t++;
    }
    /*
    if(racer_state == NORMAL) {

    }
    */

    if(ape_mode_t > TIME_IN_APE_MODE) {
        ape_mode_t = 0;
        go_normal_mode();
    }
}
