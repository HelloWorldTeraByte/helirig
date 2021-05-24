#ifndef JOYSTICK_H_
#define JOYSTICK_H_
#include <stdbool.h>
void joystick_power_sense_init(int pacer_rate);
struct Command joystick_get_speed_command(void);
void update_adc_and_button(void);
bool is_debug(void);
bool go_sleep(void);
bool joystick_button_pushed(void);


#endif /*JOYSTICK_H_*/