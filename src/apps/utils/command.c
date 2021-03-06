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

struct Command create_motor_command(int left_speed, int right_speed){
    return create_command(MOTOR_SPEED, left_speed, right_speed);
}

struct Command create_ape_mode_command(int b_ape_mode) {
    return create_command(APE_CMD, b_ape_mode, 0);
}

struct Command create_lock_motor_command(bool locked) {
    return create_command(LOCK_MOTOR, locked, 0);
}

struct Command create_battary_voltage_command(double voltage){
    int voltage_int = voltage * 100;
    return create_command(BATTARY_VOLTAGE, voltage_int, 0);
}

struct Command create_racer_state_command(int state){
    return create_command(RACER_STATE, state, 0);
}