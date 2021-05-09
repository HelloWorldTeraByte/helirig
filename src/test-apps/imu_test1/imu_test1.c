 /* File:   imu_test1.c
   Author: B Mitchell, UCECE
   Date:   15/4/2021
   Descr:  Read from an MP9250 IMU and write its output to the USB serial.
*/
#include <pio.h>
#include <fcntl.h>
#include "target.h"
#include "pacer.h"
#include "usb_serial.h"
#include "mpu9250.h"
#include <math.h>

/* Define how fast ticks occur.  This must be faster than
   TICK_RATE_MIN.  */
enum {LOOP_POLL_RATE = 200};

/* Define LED flash rate in Hz.  */
enum {LED_FLASH_RATE = 2};

double MOTOR_SPEED = 1;

static usb_serial_cfg_t usb_serial_cfg =
{
    .read_timeout_us = 1,
    .write_timeout_us = 1,
};

static twi_cfg_t mpu_twi_cfg =
{
    .channel = TWI_CHANNEL_0,
    .period = TWI_PERIOD_DIVISOR(100000), // 100 kHz
    .slave_addr = 0
};

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

int
main (void)
{
    // Create non-blocking tty device for USB CDC connection.
    usb_serial_init (&usb_serial_cfg, "/dev/usb_tty");

    freopen ("/dev/usb_tty", "a", stdout);
    freopen ("/dev/usb_tty", "r", stdin);

    // Initialise the TWI (I2C) bus for the MPU
    twi_t twi_mpu = twi_init (&mpu_twi_cfg);
    // Initialise the MPU9250 IMU
    mpu_t* mpu = mpu9250_create (twi_mpu, MPU_ADDRESS);

    pacer_init (10);

    uint8_t flash_ticks = 0;


    pio_config_set (LED_STATUS, PIO_OUTPUT_LOW);
    pio_config_set (LED_LOW_BAT, PIO_OUTPUT_LOW);
    pio_config_set (LED_ERROR, PIO_OUTPUT_LOW);
    pio_config_set (LED_DEBUG, PIO_OUTPUT_LOW);

    while (1)
    {
        /* Wait until next clock tick.  */
        pacer_wait ();

        if (mpu)
        {
            /* read in the accelerometer data */
            if (! mpu9250_is_imu_ready (mpu))
            {
                printf("Waiting for IMU to be ready...\n");
            }
            else
            {
                int16_t accel[3];
                if (mpu9250_read_accel(mpu, accel)) {

                    int accel_x = accel[0];
                    int accel_y = accel[1];
                    int accel_z = accel[2];

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
                    
                    printf("left: %5f  right: %5f \n ", motor_input_1, motor_input_2);

                } else {
                    printf("ERROR: failed to read acceleration\n");
                }
            }
        }
        else
        {
            printf("ERROR: can't find MPU9250!\n");
        }

        flash_ticks++;
        if (flash_ticks >= LOOP_POLL_RATE / (LED_FLASH_RATE * 2))
        {
            flash_ticks = 0;

            /* Toggle LED.  */
            pio_output_toggle (LED_STATUS);
        }

        fflush(stdout);
    }
}
