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
#include "power.h"
/* Define how fast ticks occur.  This must be faster than
   TICK_RATE_MIN.  */
enum {LOOP_POLL_RATE = 200};

/* Define LED flash rate in Hz.  */
enum {LED_FLASH_RATE = 10};

#define PACER_RATE 100

#define MOTOR_OFFSET 100



void my_pio_init(void)
{
    //leds
    pio_config_set (LED_STATUS, PIO_OUTPUT_LOW);
    pio_config_set (LED_LOW_BAT, PIO_OUTPUT_LOW);
    pio_config_set (LED_ERROR, PIO_OUTPUT_LOW);
    pio_config_set (LED_DEBUG, PIO_OUTPUT_LOW);
    //jumpers
    pio_config_set (RADIO_JUMPER1, PIO_PULLDOWN);
    pio_config_set (RADIO_JUMPER2, PIO_PULLDOWN);
    pio_config_set (RADIO_JUMPER3, PIO_PULLDOWN);
    pio_config_set (RADIO_JUMPER4, PIO_PULLDOWN);
}

void hat_init(void)
{
    my_pio_init();
    pacer_init(PACER_RATE);
    usb_comm_init();
    if (pio_input_get(RADIO_JUMPER1))
    {
        radio_init(NRF_CHNNEL1);
    }else if (pio_input_get(RADIO_JUMPER2)){
        radio_init(NRF_CHNNEL2);
    }else if (pio_input_get(RADIO_JUMPER3)){
        radio_init(NRF_CHNNEL3);
    }else if (pio_input_get(RADIO_JUMPER4)){
        radio_init(NRF_CHNNEL4);
    }else{
        radio_init(NRF_CHNNEL5);
    }
    imu_init();
    joystick_power_sense_init(PACER_RATE);
}

int main (void)
{
    hat_init();
    

    uint8_t flash_ticks = 0;

    struct Command command_tx = create_command(INVALID,0,0);
    struct Command command_rx = create_command(INVALID,0,0);

    bool jumping;
    
    while (1)
    {
        /* Wait until next clock tick.  */
        pacer_wait ();
        update_adc_and_button();
        //imu_read(&motor_input_1, &motor_input_2, &jumping);
        //get_left_speed(motor_input);
        //radio_transmit_command();
        /*
        if (jumping){
            flash_ticks++;

            if (flash_ticks >= LOOP_POLL_RATE / (LED_FLASH_RATE * 2))
            {
                flash_ticks = 0;
                jumping = false;
                printf("TIMEOUT\n");

            // Toggle LED.
                pio_output_toggle (LED_STATUS);
            }
        } else if (!jumping && flash_ticks != 0) {
            printf("JUMP DETECTED *************************** \n");
            flash_ticks = 0;
        }
        */

       if (is_low_bat()){
            pio_output_set(LED_LOW_BAT, 1);
       }else{
            pio_output_set(LED_LOW_BAT, 0);
       }


        //if it is in debug mode, use joystick as input. toggle by botton.
       if (is_debug()){
            command_tx = joystick_get_speed_command();          
            pio_output_set(LED_DEBUG, 1);
            
       }else{
            pio_output_set(LED_DEBUG, 0);
       }


       //radio transmit section.
        if (radio_transmit_command(command_tx)){
            pio_output_set(LED_STATUS, 1);
        }else{
            pio_output_set(LED_STATUS, 0);
        }

        command_rx = radio_read_command();
        switch(command_rx.cmd){
            case (int) BUMPER_STATUS:
                if (command_rx.arg1){
                    //car hit something!! do stuff.
                }else{
                    //car is fine, do normal stuff.
                }
                break;
            default:
                break;
        }
        

        fflush(stdout);
    }
}
