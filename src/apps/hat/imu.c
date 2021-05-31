#include "target.h"
#include <stdio.h>
#include "imu.h"
#include "mpu9250.h"
#include "command.h"
#include <math.h>
#include <stdlib.h>

#define JUMP_THRESH -9000 //190
#define JUMP_TIMEOUT 1000

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

bool is_in_dead_zone(double tilt){
    if (tilt > -0.2 && tilt < 0.2){
        return true;
    } else{
        return false;
    }
}

int is_in_sat_zone(double tilt){
    if (tilt > 1){
        return POS_SAT;
    } else if (tilt < -1){
        return NEG_SAT;
    } else {
        return UNSAT;
    }
    
}

void tilt_filter(double *tilt_f, double *tilt_s){
    //active zone 0.2 rad < tilt < 1 rad
    int saturation_f = is_in_sat_zone(*tilt_f);
    int saturation_s = is_in_sat_zone(*tilt_s);

    if (saturation_f == POS_SAT){
        *tilt_f = 1; 
    } else if (saturation_f == NEG_SAT){
        *tilt_f = -1;
    }
    if (saturation_s == NEG_SAT){
        *tilt_s = -1;
    } else if (saturation_s == POS_SAT){
        *tilt_s = 1;
    }

    if (is_in_dead_zone(*tilt_f)){
        *tilt_f = 0;
    }
    if  (is_in_dead_zone(*tilt_s)){
        *tilt_s = 0;
    }
}

void calc_motor(double *tilt_f, double *tilt_s, double *motor1, double *motor2){
    if (*tilt_s > 0 && *tilt_f == 0){
        *motor1 = 0;
        *motor2 = (1+*tilt_s)/2;
    } else if ( *tilt_s < 0 && *tilt_f == 0){
        *motor1 = (1-*tilt_s)/2;
        *motor2 = 0;
    } else {
        *motor1 = *tilt_f * (1-*tilt_s);
        *motor2 = *tilt_f * (1+*tilt_s);
    }
    if (*motor1 > 1){
        *motor1 = 1;
    } else if (*motor1 < -1){
        *motor1 = -1;
    }
    if (*motor2 > 1){
        *motor2 = 1;
    } else if (*motor2 < -1){
        *motor2 = -1;
    }
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




struct Command imu_get_speed_command(void)
{
    int motor_input_1_out;
    int motor_input_2_out;

    

    

    double tilt_forward = atan((double) -accel_x/accel_z); 
    double tilt_side = atan((double) -accel_y/accel_z); 
    //double tilt_mag;
    //calculate the magnitude of tilt to control speed? v2 idea
                    
    double motor_input_1;
    double motor_input_2;
                    // the motors are organised in the following configuration:
                    // front
                    // 1   2
    tilt_filter(&tilt_forward, &tilt_side);
    calc_motor(&tilt_forward, &tilt_side, &motor_input_1, &motor_input_2);
                    
    motor_input_1_out = (int) (motor_input_1 * 100);
    motor_input_2_out = (int) (motor_input_2 * 100);
                    
    return create_command(MOTOR_SPEED, motor_input_1_out, motor_input_2_out);
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