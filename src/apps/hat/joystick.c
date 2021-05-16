#include "joystick.h"
#include "adc.h"
#include <stdio.h>

#define ADC_CLOCK_FREQ 24000000

adc_t adcx;
adc_t adcy;

static const adc_cfg_t adcx_cfg =
{
    .bits = 12,
    .channel = ADC_CHANNEL_0,
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = ADC_CLOCK_FREQ / 1000
};

static const adc_cfg_t adcy_cfg =
{
    .bits = 12,
    .channel = ADC_CHANNEL_1,
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = ADC_CLOCK_FREQ / 1000
};


void init_joystick(void){
    adcx = adc_init (&adcx_cfg);
    adcy = adc_init (&adcy_cfg);
}

int read_joystick_x(void){
    uint16_t data[1];
    adc_read (adcx, data, sizeof (data));
    return &data;
}

int read_joystick_y(void){
    uint16_t data[1];
    adc_read (adcx, data, sizeof (data));
    return &data;
}