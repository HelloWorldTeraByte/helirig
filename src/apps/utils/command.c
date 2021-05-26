#include "command.h"
#include "stdlib.h"
#include "string.h"

struct Command create_command(char cmd, int arg1 , int arg2){
    struct Command command = 
    {
        .cmd = (int) cmd,
        .arg1 = arg1,
        .arg2 = arg2
    };
    return command;
}

//This function is working properly! confirmed by hank! atoi should be able to deal with negative numbers.
struct Command str2cmd(char* buffer)
{
	char delim[] = SEPERATOR;
    int val[3]; //buffer for values.

	char *ptr = strtok(buffer, delim);
    
    int i = 0;
	while (ptr != NULL)
	{
        val[i] = atoi(ptr);
		ptr = strtok(NULL, delim);
        i++;
	}
    return create_command(val[0], val[1], val[2]);
}


struct Command create_bumper_command(bool activated){
    return create_command(BUMPER_STATUS, activated, 0);
}

struct Command create_motor_command(int left_speed, int right_speed){
    return create_command(MOTOR_SPEED, left_speed, right_speed);
}

struct Command create_servo_command(int servo_num, bool activate) {
    return create_command(SERVO, servo_num, activate);
}


struct Command create_lock_motor_command(bool locked) {
    return create_command(LOCK_MOTOR, locked, 0);
}