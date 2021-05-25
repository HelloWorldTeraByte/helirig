#include "joystick.h"
#include "target.h"
#include "adc.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "button.h"
#include "power.h"
#include "command.h"

#define ADC_CLOCK_FREQ 24000000

#define adc_max_reading 4095
#define center_val 1920
#define center_dead_zone 100
#define outer_dead_zone 100
#define spin_threshold 300

#define JOYSTICK_SENSITIVITY 0.5

static adc_t adc;
//static volatile uint16_t data[3];
static volatile uint16_t data[2];
static button_t button1;
static button_t button;
static button_t button_sleep;



static const button_cfg_t button1_cfg =
{
    .pio = JOYSTICK_BUTTON_PIO
};

static const button_cfg_t button_cfg =
{
    .pio = BUTTON_PIO
};

static const button_cfg_t button_sleep_cfg =
{
    .pio = BUTTON_SLEEP_PIO
};



/*
static const adc_cfg_t adc_cfg =
{
    .bits = 12,
    .channels = BIT (ADC_CHANNEL_0) | BIT (ADC_CHANNEL_1) | BIT (ADC_CHANNEL_3),
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = ADC_CLOCK_FREQ / 1000
};
*/

static const adc_cfg_t adc_cfg =
{
    .bits = 12,
    .channels = BIT (ADC_CHANNEL_0) | BIT (ADC_CHANNEL_1),
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = ADC_CLOCK_FREQ / 1000
};



void joystick_power_sense_init(int pacer_rate){
    adc = adc_init (&adc_cfg);
    button1 = button_init (&button1_cfg);
    button = button_init (&button_cfg);
    button_sleep = button_init (&button_sleep_cfg);
    button_poll_count_set (BUTTON_POLL_COUNT (pacer_rate));
}

void update_adc(void){
    uint16_t my_data[3];
    //adc_sync(adc);
    adc_read(adc, my_data, sizeof (data));
    data[0] = my_data[0];
    data[1] = my_data[1];
    //data[2] = my_data[2];
}

void update_button(void){
    button_poll (button1);
    button_poll (button);
    button_poll (button_sleep);
}


bool go_sleep(void){
    if (button_pushed_p (button_sleep)){
        //pio_output_toggle(LED_ERROR);
        return true;
    }
    return false;
}

bool joystick_button_pushed(void){
    if (button_pushed_p (button1)){
        return true;
    }
    return false;
}

bool is_debug(void){
    static bool debug = false;
    if (button_pushed_p (button)){
        debug = !debug;
    }
    return debug;
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

struct Command joystick_get_speed_command(void){
    int speed_buffer[2];
    double forward_reading = get_joystick_x();
    double side_reading = get_joystick_y();
    if (is_in_spinzone(forward_reading, side_reading)){
        speed_buffer[0] = side_reading/(adc_max_reading/2.0) * 100;
        speed_buffer[1] = -side_reading/(adc_max_reading/2.0) * 100;
    }else if(!is_in_deadzone(forward_reading)){
        speed_buffer[0] = (forward_reading+(side_reading*JOYSTICK_SENSITIVITY))/(adc_max_reading/2.0) * 100;
        speed_buffer[1] = (forward_reading-(side_reading*JOYSTICK_SENSITIVITY))/(adc_max_reading/2.0) * 100;
    }else{
        speed_buffer[0] = 0;
        speed_buffer[1] = 0;
    }

    if (speed_buffer[0] > 100){
        speed_buffer[0] = 100;
    }

    if (speed_buffer[1] > 100){
        speed_buffer[1] = 100;
    }
    return create_command(MOTOR_SPEED, speed_buffer[0], speed_buffer[1]);
}


/*
float read_bat_voltage(void){
    int v_out =  data[2] + POWER_SENSE_CORRECTION;
    float v_in = (v_out * ( (double) POWER_SENSE_R1 + (double) POWER_SENSE_R2) / (double) POWER_SENSE_R2) * (3.3/4095.0);
    return v_in;
}
*/
float read_bat_voltage(void){
    return 5.0;
}

bool is_low_bat(void){
    float v_in = read_bat_voltage();
    if (v_in < LOW_POWER_THRESH){
        return true;
    }
    return false;
}