command:
<command?arg1?arg2>

command will be a singal char.
arg1 and arg2 will be a positive number ranging from 0 to 512.


Speed:
    <M?motor1_speed?motor2_speed>       ->arg1&arg2 0-200


Servo:
    <A?mode?val> 
    mode:
    0: normal val:na
    1: rage val:0 to 100 how much rage.


Bumper activate:
    <B?is_bumper_activate?0> is_bumper_activate is a bool. 0 or 1.
    racer shold sent this every 100ms.


struct command{
    int cmd;
    int arg1;
    int arg2;
}

struct command readcommand(char* command){
    return cammand
}


struct command cmd = readcommand(shit)

switch(cmd){
    case M:
        set_motor(cmd.ar1, cmd.arg2)
        break;
    case A:
        //do something
        break;
    defaut:
        //do something
        break;
}