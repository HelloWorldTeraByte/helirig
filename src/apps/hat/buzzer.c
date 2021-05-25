#include "piezo.h"
#include "piezo_beep.h"
#include "target.h"
#include "mcu.h"

const piezo_cfg_t piezo_cfg = 
{
    .pio = PIEZO_PIO,
};

static piezo_t buzzer;



void buzzer_init(void){
    mcu_jtag_disable();
    buzzer = piezo_init(&piezo_cfg);
}

void buzzer_beep(uint16_t ms){
    piezo_beep(buzzer, ms);
}
