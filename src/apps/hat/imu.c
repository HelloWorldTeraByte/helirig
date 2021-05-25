#include "target.h"
#include <stdio.h>
#include "imu.h"
#include "mpu9250.h"
#include "command.h"
#include <math.h>

static twi_t twi_mpu;
static mpu_t* mpu;

int jump_counter = 0;
bool mid_air = false;
bool jump_flag = false;
bool rage_flag = false;

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

bool jump_detect(int accel_x, int accel_y, int accel_z, bool mid_air){
    double accel_mag = sqrt((abs(accel_x)^2)+(abs(accel_y)^2)+(abs(accel_z)^2));
    bool come_down = false;

    printf("%f, %d\n", accel_mag, mid_air);

    if (mid_air){
        if (accel_mag > 190){
            return true;
        }
    } else{
        if (accel_mag < 100){
            return false;
        }
    }
}

struct Command imu_get_speed_command(void)
{
    int motor_input_1_out;
    int motor_input_2_out;

    int16_t accel[3];
    mpu9250_read_accel(mpu, accel);

    int accel_x = accel[0];
    int accel_y = accel[1];
    int accel_z = accel[2];

    jump_flag = jump_detect(accel_x, accel_y, accel_z, mid_air);
    if (mid_air){
        jump_counter++;

        if (jump_flag){
            rage_flag = true;
            mid_air = false;
            jump_counter = 0;
        } 
        if (jump_counter >= 10)
        {
            jump_counter = 0;
            mid_air = false;
            printf("TIMEOUT\n");
        }
    } else {
        rage_flag = false;
    }

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