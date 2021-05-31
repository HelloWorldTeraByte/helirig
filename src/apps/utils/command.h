#ifndef __COMMAND__H
#define __COMMAND__H

//define commands.
#define MOTOR_SPEED 'M'
#define LOCK_MOTOR 'L'
#define BUMPER_STATUS 'B'
#define BATTARY_VOLTAGE 'V'
#define RACER_STATE 'R'

#define APE_CMD 'A'
enum{SERVO_NUM1, SERVO_NUM2}; //servo number.

#define INVALID 'I'
#define NOTHING 'N'


//protocal(Hank can't bother to implement starting and terminating character!)
#define SEPERATOR "?"

#include <stdbool.h>
//command struct.
struct Command{
    int cmd;
    int arg1;
    int arg2;
};

struct Command str2cmd(char* string);
struct Command create_command(char cmd, int arg1 , int arg2);

struct Command create_motor_command(int left_speed, int right_speed);
struct Command create_ape_mode_command(int b_ape_mode);
struct Command create_lock_motor_command(bool locked);
struct Command create_battary_voltage_command(double voltage);
struct Command create_racer_state_command(int state);

#endif
