#include "stdlib.h"
#include "string.h"


//This function is working properly! confirmed by hank! atoi should be able to deal with negative numbers.
struct Command str2cmd(char* string)
{
    
	char delim[] = SEPERATOR;
	char *ptr = strtok(buffer, delim);

    int val[3]; //buffer for values.
    
    int i = 0;
	while (ptr != NULL)
	{
        val[i] = atoi(ptr);
		ptr = strtok(NULL, delim);
        i++;
	}
    struct Command command = 
    {
        .cmd = val[0],
        .arg1 = val[1],
        .arg2 = val[2]
    };

    return command;
}


struct Command create_command(char cmd, int arg1 , int arg2){
    struct Command command = 
    {
        .cmd = (int) cmd,
        .arg1 = arg1,
        .arg2 = arg2
    };
    return command;
}
