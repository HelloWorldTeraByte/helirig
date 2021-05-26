#ifndef JOYSTICK_H_
#define JOYSTICK_H_
#include <stdbool.h>
void joystick_power_sense_init(int pacer_rate);
struct Command joystick_get_speed_command(void);
void update_adc(void);
void update_button(void);
bool is_debug(void);

bool go_sleep(void);
bool joystick_button_pushed(void);
bool button_pushed(void);


float read_bat_voltage(void);
bool is_low_bat(void);

#endif /*JOYSTICK_H_*/