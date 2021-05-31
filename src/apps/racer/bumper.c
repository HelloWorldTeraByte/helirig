#include "mcu.h"
#include "pio.h"
#include "bumper.h"
#include "button.h"

static button_t bumper;
static uint8_t bumper_stat;
static uint16_t bumper_timeout;

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
        bumper_stat = true;     
    }else {
        bumper_stat = false;
    }

    bumper_timeout_update();
}

void bumper_timeout_update(void)
{
    if(bumper_timeout > 0)
        bumper_timeout--;
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

uint8_t bumper_status_get(void)
{
    return bumper_stat;
}