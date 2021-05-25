#ifndef __COMMAND__H
#define __COMMAND__H

//define commands
#define MOTOR_SPEED 'M'
#define BUMPER_STATUS 'B'
#define SERVO 'S'
enum{SERVO_NORMAL, SERVO_RAGE}; //servo modes.

#define INVALID 'I'
#define NOTHING 'N'


//protocal(Hank can't bother to implement starting and terminating character!)
#define SEPERATOR "?"

//command struct.
struct Command{
    int cmd;
    int arg1;
    int arg2;
};

struct Command str2cmd(char* string);
struct Command create_command(char cmd, int arg1 , int arg2);


#endif
