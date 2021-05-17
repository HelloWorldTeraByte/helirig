#include "joystick.h"
#include "adc.h"
#include <stdio.h>

#define ADC_CLOCK_FREQ 24000000
#define adc_max_reading 4095

adc_t adc;
uint16_t data[2];

static const adc_cfg_t adc_cfg =
{
    .bits = 12,
    .channels = BIT (ADC_CHANNEL_0) | BIT (ADC_CHANNEL_1),
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = ADC_CLOCK_FREQ / 1000
};


void init_joystick(void){
    adc = adc_init (&adc_cfg);
}

int read_joystick(void){
    adc_read (adc, data, sizeof (data));
}

int get_joystick_x(void){
    return data[1];
}

int get_joystick_y(void){
    return data[1];
}