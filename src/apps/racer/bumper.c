#include "mcu.h"
#include "pio.h"
#include "bumper.h"
#include "button.h"

static button_t bumper;
static uint8_t bumper_stat;
static uint16_t bumper_timeout;

static uint8_t been_hit = 0;
static uint8_t exit_hit_mode;
static uint8_t go_hit_mode;

static const button_cfg_t bumper_cfg =
{
    .pio = BUMPER_PIO
};

void bumper_init(void)
{
    bumper = button_init(&bumper_cfg);
    bumper_timeout = 0;
}

void bumper_update(void)
{
    button_poll(bumper);

    if(button_pushed_p(bumper)) {
        bumper_timeout = BUMPER_TIMEOUT;
        go_hit_mode = 1;
        been_hit = 1;
        bumper_stat = true;     
    }else {
        bumper_stat = false;
    }

    bumper_timeout_update();
}

uint8_t b_go_hit_mode(void)
{
    uint8_t ret = 0;
    if(go_hit_mode == 1) {
        ret = 1;
        go_hit_mode = 0;
    }
    return ret;
}

uint8_t b_exit_hit_mode(void)
{
    uint8_t ret = 0;
    if(exit_hit_mode == 1) {
        ret = 1;
        exit_hit_mode = 0;
    }
    return ret;
}

void bumper_timeout_update(void)
{
    if(bumper_timeout > 0) {
        bumper_timeout--;
    }
    if(been_hit == 1 && bumper_timeout <= 0) {
        been_hit = 0;
        exit_hit_mode = 1;
    }
}

void bumper_set_timeout(uint8_t val)
{
    bumper_timeout = val;
}

uint8_t is_bumper_in_timeout(void)
{
    if(bumper_timeout > 0)
        return 1;
    else
        return 0;
}
//
//uint8_t bumper_status_get(void)
//{
//    return bumper_stat;
//}