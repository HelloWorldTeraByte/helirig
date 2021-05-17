#include "target.h"
#include <stdio.h>
#include "imu.h"
#include "mpu9250.h"

static twi_t twi_mpu
static mpu_t *mpu

void imu_init(){
    // Initialise the TWI (I2C) bus for the MPU
    twi_t twi_mpu = twi_init (&mpu_twi_cfg);
    // Initialise the MPU9250 IMU
    mpu_t* mpu = mpu9250_create (twi_mpu, MPU_ADDRESS);
}

void tilt_filter(double *tilt_f, double *tilt_s){
    //active zone 0.2 rad < tilt < 1 rad
    if (*tilt_f > 1){
        *tilt_f = 1; 
    } else if (*tilt_f < -1){
        *tilt_f = -1;
    }
    if (*tilt_s < -1){
        *tilt_s = -1;
    } else if (*tilt_s > 1){
        *tilt_s = 1;
    }

    if (*tilt_f > -0.2 && *tilt_f < 0.2){
        *tilt_f = 0;
    }
    if  (*tilt_s > -0.2 && *tilt_s < 0.2){
        *tilt_s = 0;
    }
}

void calc_motor(double *tilt_f, double *tilt_s, double *motor1, double *motor2){
    if ((*tilt_s > 0 || *tilt_s < 0) && *tilt_f == 0){
        *motor1 = MOTOR_SPEED * (1-*tilt_s)/2;
        *motor2 = MOTOR_SPEED * (1+*tilt_s)/2;
    } else {
        *motor1 = MOTOR_SPEED * *tilt_f * (1-*tilt_s);
        *motor2 = MOTOR_SPEED * *tilt_f * (1+*tilt_s);
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