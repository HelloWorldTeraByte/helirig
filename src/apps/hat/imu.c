#include "target.h"
#include <stdio.h>
#include "imu.h"
#include "mpu9250.h"
#include "command.h"
#include <math.h>
#include <stdlib.h>

#define JUMP_THRESH -14000 //190
#define JUMP_TIMEOUT 5000

#define IMU_MAX 16000
#define DEAD_ZONE 1000
#define SPIN_ZONE 2000
#define SPIN_MAX_SPEED 50
#define SENSITIVITY 0.3
static twi_t twi_mpu;
static mpu_t* mpu;


static int accel_x;
static int accel_y;
static int accel_z;

bool jump_flag = false;
bool cool_down = false;
static int count = 0;
enum {POS_SAT, NEG_SAT, UNSAT};

static twi_cfg_t mpu_twi_cfg =
{
    .channel = TWI_CHANNEL_0,
    .period = TWI_PERIOD_DIVISOR(100000), // 100 kHz
    .slave_addr = 0
};

void imu_init(void){
    // Initialise the TWI (I2C) bus for the MPU
    twi_mpu = twi_init (&mpu_twi_cfg);
    // Initialise the MPU9250 IMU
    mpu = mpu9250_create (twi_mpu, MPU_ADDRESS);
}

bool jump_detect(void){
    if (accel_x < JUMP_THRESH){
        return true;
    }
    return false;
}



void update_jump_detection(void){
    

    if (jump_detect() && count == 0){
        jump_flag = true;
        cool_down = true;
        count ++;
    }else if(cool_down && count < JUMP_TIMEOUT){
        count ++;
    }else{
        cool_down = false;
        count = 0;
    }
   
}

bool get_jump_status(void){
    if (jump_flag){
        jump_flag = false;
        return true;
    }else{
        return false;
    }
}


bool IMU_is_in_deadzone(int val){
    if (abs(val) < DEAD_ZONE){
        return true;
    }
    return false;
}

bool IMU_is_in_spinzone(void){
    if (IMU_is_in_deadzone(accel_y)){
        return false;
    }else if(abs(accel_x) < SPIN_ZONE + DEAD_ZONE){
        return true;
    }
    return false;
}

int my_map(int x, int x_min, int x_max, int y_min, int y_max){
    int A = ((double)x-(double)x_min)/((double)x_max-(double)x);
    int y = ((double)y_min+(double)y_max)/(1.0+(double)A);
    return y;
}

struct Command imu_get_speed_command(void)
{
    volatile int speed_buffer[2];
    if (IMU_is_in_spinzone()){
        speed_buffer[0] = (double) -accel_y/((double) IMU_MAX) * SPIN_MAX_SPEED;
        speed_buffer[1] = (double) accel_y/((double) IMU_MAX) * SPIN_MAX_SPEED;
    }else if(!IMU_is_in_deadzone(accel_x)){
        double scale = my_map(accel_x, DEAD_ZONE, IMU_MAX, 1, 10)*2.0/10.0;
        speed_buffer[0] = ((double) accel_x-((double) accel_y*SENSITIVITY/scale))/((double)IMU_MAX) * 100;
        speed_buffer[1] = ((double) accel_x+((double) accel_y*SENSITIVITY)/scale)/((double) IMU_MAX) * 100;
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

    if (speed_buffer[0] < -100){
        speed_buffer[0] = -100;
    }

    if (speed_buffer[1] < -100){
        speed_buffer[1] = -100;
    }
    
    return create_command(MOTOR_SPEED, speed_buffer[0], speed_buffer[1]);
}

bool is_cool_down(void){
    if (count == 0){
        return false;
    }
    return true;
}

void update_imu(void){
    int16_t accel[3];
    mpu9250_read_accel(mpu, accel);
    accel_x = accel[0];
    accel_y = accel[1];
    accel_z = accel[2];
    update_jump_detection();
}