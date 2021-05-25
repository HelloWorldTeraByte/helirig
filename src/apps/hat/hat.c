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
#include "ledbuffer.h"
#include "mcu_sleep.h"
#include "buzzer.h"

/* Define how fast ticks occur.  This must be faster than
   TICK_RATE_MIN.  */
#define LOOP_POLL_RATE 200

//config rates
#define ADC_RATE 10
#define RADIO_RATE 10
#define IMU_RATE 10
#define ONE_SECOND_RATE 1


/* Define LED flash rate in Hz.  */
enum {LED_FLASH_RATE = 10};


#define MOTOR_OFFSET 100

#define NUM_LEDS 29


static const mcu_sleep_wakeup_cfg_t sleep_wakeup_cfg = 
{
    .pio = BUTTON_SLEEP_PIO,
    .active_high = false
};

static const mcu_sleep_cfg_t sleep_cfg = 
{
    .mode = MCU_SLEEP_MODE_SLEEP
};



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
    pacer_init(LOOP_POLL_RATE);
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
    joystick_power_sense_init(LOOP_POLL_RATE);
    buzzer_init();
}

int main (void)
{
    hat_init();
    
    int led_tape_count = 0;

    ledbuffer_t* leds = ledbuffer_init(LEDTAPE_PIO, NUM_LEDS);

    uint8_t flash_ticks = 0;

    int imu_ticks = 0;
    int radio_ticks = 0;
    int adc_tick = 0;
    int one_second_tick = 0;

    struct Command command_tx = create_command(INVALID,0,0);
    struct Command command_rx = create_command(INVALID,0,0);

    bool jumping;
    
    while (1)
    {

        /*ticks increment*/
        imu_ticks++;
        radio_ticks++;
        adc_tick++;
        one_second_tick++;


        /* pacer */
        pacer_wait ();

        /* update button */
        update_button();

        /* update adc */
        if (adc_tick >= LOOP_POLL_RATE / (ADC_RATE * 2))
        {
            update_adc();
            adc_tick = 0;
            
        }
        




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
       if (one_second_tick >= LOOP_POLL_RATE / (ONE_SECOND_RATE * 2))
        {
        /*one second task start.*/
            //check bat.
            if (is_low_bat()){
                pio_output_set(LED_LOW_BAT, 1);
            }else{
                pio_output_set(LED_LOW_BAT, 0);
            }
            //do something.
        /*one second task ends.*/
            one_second_tick = 0;
        }


        //if it is in debug mode, use joystick as input. toggle by botton.
       if (is_debug()){
            command_tx = joystick_get_speed_command();          
            pio_output_set(LED_DEBUG, 1);
            
       }else{
            pio_output_set(LED_DEBUG, 0);
       }


       //radio transmit section.

       if (radio_ticks >= LOOP_POLL_RATE / (RADIO_RATE * 2))
        {
            if (radio_transmit_command(command_tx)){
                pio_output_set(LED_STATUS, 1);
            }else{
                pio_output_set(LED_STATUS, 0);
            }
            command_rx = radio_read_command();
            radio_ticks = 0;
        }
        
        




        //execute intake commands.
        switch(command_rx.cmd){
            case (int) BUMPER_STATUS:
                if (command_rx.arg1){
                    //car hit something!! do stuff.
                    buzzer_beep(1);
                    pio_output_set(LED_ERROR, 1);
                }else{
                    //car is fine, do normal stuff.
                    pio_output_set(LED_ERROR, 0);
                }
                break;
            default:
                break;
        }
        /*
        if (led_tape_count++ == 8) {
            // wait for a revolution
                ledbuffer_set(leds, 0, 255, 0, 0);
                ledbuffer_set(leds, 1, 252, 94, 0);
                ledbuffer_set(leds, 2, 255, 211, 0);
                ledbuffer_set(leds, 3, 11, 252, 3);
                ledbuffer_set(leds, 4, 3, 102, 252);
                ledbuffer_set(leds, 5, 3, 3, 252);
                ledbuffer_set(leds, 6, 123, 3, 252);
                ledbuffer_set(leds, 7, 255, 255, 255);
            led_tape_count = 0;
            piezo_beep(buzzer, 1000);
        }
        ledbuffer_write (leds);
        ledbuffer_advance (leds, 1);
        */


       
        if (joystick_button_pushed()){

            //do some stuff.
            buzzer_beep(200);
        }

       if (go_sleep()){
           mcu_sleep_wakeup_set(&sleep_wakeup_cfg);
           mcu_sleep(&sleep_cfg);
       }



        fflush(stdout);
    }
}
