#include "stdint.h"
#include "state_manager.h"
#include "led_tape.h"
#include "servos.h"
#include "bumper.h"

enum states racer_state = NORMAL;
enum states state_bf_hit = NORMAL;

static uint16_t ape_mode_t = 0;

void state_manager_init(void)
{
    go_normal_mode();
}

void go_ape_mode(void)
{
    racer_state = APE;
    servo_drop_bananas();
    ledt_setmode(LEDT_APE);
}

void go_normal_mode(void)
{
    racer_state = NORMAL;
    ledt_setmode(LEDT_NORMAL);
}

void go_hit_mode(void)
{
    state_bf_hit = racer_state;
    racer_state = HIT;
    ledt_setmode(LEDT_HIT);
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

enum states get_current_state(void)
{
    return racer_state;
}
