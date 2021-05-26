#ifndef __COMMAND__H
#define __COMMAND__H

//define commands#include <stdbool.h>
#define MOTOR_SPEED 'M'
#define LOCK_MOTOR 'L'
#define BUMPER_STATUS 'B'
#define SERVO 'S'
enum{SERVO_NUM1, SERVO_NUM2}; //servo modes.

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


struct Command create_bumper_command(bool activated);
struct Command create_motor_command(int left_speed, int right_speed);
struct Command create_servo_command(int servo_num, bool activate);
struct Command create_lock_motor_command(bool locked);

#endif
