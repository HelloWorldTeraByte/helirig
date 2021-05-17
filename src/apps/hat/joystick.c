#include "joystick.h"
#include "adc.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define ADC_CLOCK_FREQ 24000000
#define adc_max_reading 4095
#define center_val 1920
#define center_dead_zone 100
#define outer_dead_zone 100
#define spin_threshold 300

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

//forward positive and backwards negative
int get_joystick_x(void){
    return data[1] - center_val;
}



//left positive, right negative.
int get_joystick_y(void){
    return data[0] - center_val;
}

bool is_in_deadzone(int val){
    if (abs(val) < center_dead_zone){
        return true;
    }
    return false;
}

bool is_in_spinzone(int val){
    if (is_in_deadzone(val)){
        return false;
    }else if(abs(val) < spin_threshold +  center_dead_zone){
        return true;
    }
    return false;
}

void get_left_speed(int speed_buffer[]){
    double forward_reading = get_joystick_x();
    double side_reading = get_joystick_y();
    if (is_in_spinzone(forward_reading)){
        speed_buffer[0] = side_reading/(adc_max_reading/2.0) * 100;
        speed_buffer[1] = -side_reading/(adc_max_reading/2.0) * 100;
    }
}