#include "mcu.h"
#include "button.h"

static button_t btn0;
static uint8_t btn0_stat;

static const button_cfg_t btn0_cfg =
{
    .pio = BUTTON_WAKEUP_PIO
};

void btns_init(void)
{
    btn0 = button_init(&btn0_cfg);
}

void btns_update(void)
{
    button_poll(btn0);
}

uint8_t is_btn0_pressed(void)
{
    if (button_pushed_p(btn0))
    {
        btn0_stat = 1;
        return 1;
    }
    else
    {
        btn0_stat = 0;
        return 0;
    }
}