#include <pio.h>
#include <fcntl.h>
#include "target.h"
#include "pacer.h"
#include "usb_serial.h"
#include "mpu9250.h"
#include <math.h>
#include "nrf24.h"
#include "delay.h"
#include "imu.h"
#include "joystick.h"
#include "radio_comm.h"
#include "usb_comm.h"
/* Define how fast ticks occur.  This must be faster than
   TICK_RATE_MIN.  */
enum {LOOP_POLL_RATE = 200};

/* Define LED flash rate in Hz.  */
enum {LED_FLASH_RATE = 10};

#define MOTOR_OFFSET 100

void led_init(void)
{
    pio_config_set (LED_STATUS, PIO_OUTPUT_LOW);
    pio_config_set (LED_LOW_BAT, PIO_OUTPUT_LOW);
    pio_config_set (LED_ERROR, PIO_OUTPUT_LOW);
    pio_config_set (LED_DEBUG, PIO_OUTPUT_LOW);
}

void hat_init(void)
{
    led_init();

    init_joystick();
    pacer_init(10);
    usb_comm_init();
    imu_init();
    radio_comm_init();
}

int main (void)
{
    hat_init();
    

    uint8_t flash_ticks = 0;

    int motor_input[2];
    int motor_input_1;
    int motor_input_2;

    bool jumping;
    
    while (1)
    {
        /* Wait until next clock tick.  */
        pacer_wait ();
        imu_read(&motor_input_1, &motor_input_2, &jumping);
        get_left_speed(motor_input);
        radio_transmit(motor_input[0]+MOTOR_OFFSET, motor_input[1]+MOTOR_OFFSET);
        
        if (jumping){
            flash_ticks++;

            if (flash_ticks >= LOOP_POLL_RATE / (LED_FLASH_RATE * 2))
            {
                flash_ticks = 0;
                jumping = false;
                printf("TIMEOUT\n");

            /* Toggle LED.  */
                pio_output_toggle (LED_STATUS);
            }
        } else if (!jumping && flash_ticks != 0) {
            printf("JUMP DETECTED *************************** \n");
            flash_ticks = 0;
        }
        

        fflush(stdout);
    }
}
