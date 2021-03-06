#include "adc.h"
#include "power.h"
#include <stdbool.h>

#define ADC_CLOCK_FREQ 24000000
adc_t adc;
uint16_t data[1];

static const adc_cfg_t adc_cfg =
{
    .bits = 12,
    .channels = BIT (ADC_CHANNEL_1),
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = ADC_CLOCK_FREQ / 1000
};

void power_sense_init(void){
    adc = adc_init (&adc_cfg);
}

float power_batt_read(void){
    adc_read (adc, data, sizeof (data));
    int v_out =  data[0] + POWER_SENSE_CORRECTION;
    float v_in = (v_out * ( (double) POWER_SENSE_R1 + (double) POWER_SENSE_R2) / (double) POWER_SENSE_R2) * (3.3/4095.0);
    return v_in;
}

bool power_is_batt_low(void){
    float v_in = power_batt_read();
    if (v_in < LOW_POWER_THRESH){
        return true;
    }
    return false;
}