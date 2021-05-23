#include "joystick.h"
#include "target.h"
#include "adc.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "button.h"

#define ADC_CLOCK_FREQ 24000000

#define adc_max_reading 4095
#define center_val 1920
#define center_dead_zone 100
#define outer_dead_zone 100
#define spin_threshold 300

adc_t adc;
uint16_t data[2];
button_t button1;



static const button_cfg_t button1_cfg =
{
    .pio = JOYSTICK_BUTTON_PIO
};


static const adc_cfg_t adc_cfg =
{
    .bits = 12,
    .channels = BIT (ADC_CHANNEL_0) | BIT (ADC_CHANNEL_1),
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = ADC_CLOCK_FREQ / 1000
};


void joystick_init(void){
    adc = adc_init (&adc_cfg);
    button1 = button_init (&button1_cfg);
}

void read_joystick(void){
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

bool is_in_spinzone(int xval, int yval){
    if (is_in_deadzone(yval)){
        return false;
    }else if(abs(xval) < spin_threshold +  center_dead_zone){
        return true;
    }
    return false;
}

void get_left_speed(int speed_buffer[]){
    read_joystick();
    double forward_reading = get_joystick_x();
    double side_reading = get_joystick_y();
    if (is_in_spinzone(forward_reading, side_reading)){
        speed_buffer[0] = side_reading/(adc_max_reading/2.0) * 100;
        speed_buffer[1] = -side_reading/(adc_max_reading/2.0) * 100;
    }else if(!is_in_deadzone(forward_reading)){
        speed_buffer[0] = forward_reading/(adc_max_reading/2.0) * 100;
        speed_buffer[1] = forward_reading/(adc_max_reading/2.0) * 100;
    }else{
        speed_buffer[0] = 0;
        speed_buffer[1] = 0;
    }
}