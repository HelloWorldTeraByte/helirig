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
#include "led_tape.h"

/* Define how fast ticks occur.  This must be faster than
   TICK_RATE_MIN.  */
#define LOOP_POLL_RATE 200

//config rates
#define ADC_RATE 10
#define RADIO_TX_RATE 20
#define RADIO_RX_RATE 40
#define IMU_RATE 10
#define ONE_SECOND_RATE 1
#define LED_RATE 3


/* Define LED flash rate in Hz.  */
enum {LED_FLASH_RATE = 10};
enum {MOTOR_MSG, SERVO_MSG, LOCK_MOTOR_MSG};


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
    pacer_init(LOOP_POLL_RATE);
    //usb_comm_init();
    imu_init();
    joystick_power_sense_init(LOOP_POLL_RATE);
    buzzer_init(LOOP_POLL_RATE*2);
    my_pio_init();   


    delay_ms(100); 
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
}

int main (void)
{
    hat_init();
    
    static int msg_select = 0;


    int led_tape_count = 0;

    ledbuffer_t* leds = ledt_init();

    uint8_t flash_ticks = 0;

    int imu_ticks = 0;
    int radio_tx_ticks = 0;
    int radio_rx_ticks = 0;
    int adc_tick = 0;
    int one_second_tick = 0;
    int led_tick = 0;

    struct Command command_tx = create_command(INVALID,0,0);
    struct Command command_rx = create_command(INVALID,0,0);
    while (1)
    {

        /*ticks increment*/
        imu_ticks++;
        radio_tx_ticks++;
        radio_rx_ticks++;
        adc_tick++;
        one_second_tick++;
        led_tick++;


        /* pacer */
        pacer_wait ();

        /* update button */
        update_button();

        /* update buzzer*/
        buzzer_update();

        /* update adc */
        if (adc_tick >= LOOP_POLL_RATE / (ADC_RATE * 2))
        {
            update_adc();
            adc_tick = 0;
            
        }
        
        //get_left_speed(motor_input);
       if (one_second_tick >= LOOP_POLL_RATE / (ONE_SECOND_RATE * 2))
        {
        /*one second task start.*/
            //check bat.
            if (is_low_bat()){
                pio_output_toggle(LED_LOW_BAT);
            } else {
                pio_output_set(LED_LOW_BAT, 0);
            }

            if(is_debug()){
                pio_output_toggle(LED_DEBUG);
            }
            //do something.
        /*one second task ends.*/
            one_second_tick = 0;
        }



        if (radio_rx_ticks >= LOOP_POLL_RATE / (RADIO_RX_RATE * 2))
        {
            command_rx = radio_read_command();

             //execute intake commands.
            switch(command_rx.cmd){

                case (int) BUMPER_STATUS:

                    if (command_rx.arg1){
                        //car hit something!! do stuff.
                        buzzer_beep(1);
                        //pio_output_set(LED_ERROR);
                    }else{
                        //car is fine, do normal stuff.
                        //pio_output_set(LED_ERROR, 0);
                    }
                    break;

                default:
                    break;
            }

            radio_rx_ticks = 0;
        }




       //radio transmit section.
       if (radio_tx_ticks >= LOOP_POLL_RATE / (RADIO_TX_RATE * 2))
        {   
            
            //create msg.
            switch(msg_select){
                case MOTOR_MSG:
                    if (is_debug()){
                        command_tx = joystick_get_speed_command();          
                    
                    }else{
                        command_tx = imu_get_speed_command();
                    }
                    break;
                case SERVO_MSG:
                    command_tx = create_servo_command(SERVO_NUM1, true);
                    msg_select = MOTOR_MSG;
                    break;
                case LOCK_MOTOR_MSG:
                    command_tx = create_lock_motor_command(false);
                    msg_select = MOTOR_MSG;
                    break;
                default:
                    
                    msg_select = MOTOR_MSG;
                    break;
            }
            




            //transmit.
            if (radio_transmit_command(command_tx)){
                pio_output_set(LED_STATUS, 1);
            }else{
                pio_output_set(LED_STATUS, 0);
            }


            radio_tx_ticks = 0;
        }
        



        if (led_tick >= LOOP_POLL_RATE / (LED_RATE * 2))
        {
            ledt_run(true, leds);
            led_tick = 0;
        }




       

       
        if (joystick_button_pushed()){
            msg_select = SERVO_MSG;
            buzzer_beep(1000);
            
        }

        if(button_pushed()){
            msg_select = LOCK_MOTOR_MSG;
            if(buzzer_is_playing()){
                buzzer_music_pause();
            }else{
                buzzer_music_play(MUSIC_CANNON);
            }
        }


        
       if (go_sleep()){
           //pio_output_toggle(LED_ERROR);
           mcu_sleep_wakeup_set(&sleep_wakeup_cfg);
           mcu_sleep(&sleep_cfg);
       }



        //fflush(stdout);
    }
}
