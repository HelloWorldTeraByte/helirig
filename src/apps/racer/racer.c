#include "target.h"
#include "pacer.h"
#include "pio.h"
#include "motors.h"
#include "radio_comm.h"
#include "servos.h"
#include "usb_comm.h"

#include "stdlib.h"
#include "string.h"

#include "ledbuffer.h"

#define NUM_LEDS 28

void racer_init(void)
{
    /* Configure LED PIO as output.  */
    pio_config_set (LED1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LED2_PIO, PIO_OUTPUT_LOW);
    
    /* Configure Radio Channels */
    pio_config_set (RADIO_JUMPER1, PIO_PULLDOWN);
    pio_config_set (RADIO_JUMPER2, PIO_PULLDOWN);
    pio_config_set (RADIO_JUMPER3, PIO_PULLDOWN);
    pio_config_set (RADIO_JUMPER4, PIO_PULLDOWN);

    /*Choose a radio channel*/

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

    /* Initilize the motors*/
    motors_init();

    usb_comm_init();

    pacer_init(LOOP_POLL_RATE);
}

int main(void)
{   
    uint8_t flash_ticks;
    uint8_t radio_ticks;
    uint8_t count = 0;
    flash_ticks = 0;
    radio_ticks = 0;

    char w_buff[32];
    char r_buff[12] = {0};
    int radio_vals[12];
    int16_t mtr_l_speed;
    int16_t mtr_r_speed;
    sprintf(w_buff, "Ohh OOH\r\n");

    bool ape = false;
    int counter = 0;
    int dpacer = 0;

    ledbuffer_t* ledsr = ledbuffer_init(LEDTAPE_PIO, NUM_LEDS);

    /* Initilize the car - GPIO, pacer and motors*/
    racer_init();

    while (1) {
        pio_config_set (LED2_PIO, PIO_OUTPUT_HIGH);
        /* Wait until next clock tick.  */
        pacer_wait();

        radio_read(r_buff, sizeof(r_buff));
        

        //printf("BUFF:%s\n", r_buff);

        int i = 0;
        char *token = strtok(r_buff, ",");
        // loop through the string to extract all other tokens
        while (token != NULL)
        {
            radio_vals[i] = atoi(token);
            token = strtok(NULL, ",");
            i++;
        }

        mtr_r_speed = radio_vals[0] - 100;
        mtr_l_speed = radio_vals[1] - 100;

        motor_left_set(mtr_l_speed);
        motor_right_set(mtr_r_speed);

        printf("%d,%d\n", mtr_l_speed, mtr_r_speed);

        flash_ticks++;
        if (flash_ticks >= LOOP_POLL_RATE / (LED_FLASH_RATE * 2))
        {
            flash_ticks = 0;
            pio_output_toggle(LED1_PIO);
            //radio_write(w_buff, sizeof(w_buff));
            count++;
            if (count > 60)
                count = 0;

       }
    
        radio_ticks++;
        if (radio_ticks >= LOOP_POLL_RATE / (RADIO_POLL_RATE * 2))
        {
           radio_ticks = 0;
        }

        if (dpacer++ == 19){
            if (counter++ == NUM_LEDS) {

                ledbuffer_clear(ledsr);
                ledbuffer_set(ledsr, 0, 0, 100, 0);
                ledbuffer_set(ledsr, 1, 14, 94, 0);
                ledbuffer_set(ledsr, 2, 29, 87, 0);
                ledbuffer_set(ledsr, 3, 43, 80, 0);
                ledbuffer_set(ledsr, 4, 58, 72, 0);
                ledbuffer_set(ledsr, 5, 72, 65, 0);
                ledbuffer_set(ledsr, 6, 87, 58, 0);
                ledbuffer_set(ledsr, 7, 102, 51, 0);
                ledbuffer_set(ledsr, 8, 87, 58, 0);
                ledbuffer_set(ledsr, 9, 72, 65, 0);
                ledbuffer_set(ledsr, 10, 58, 72, 0);
                ledbuffer_set(ledsr, 11, 43, 80, 0);
                ledbuffer_set(ledsr, 12, 29, 87, 0);
                ledbuffer_set(ledsr, 13, 14, 94, 0);
                ledbuffer_set(ledsr, 14, 0, 100, 0);
                ledbuffer_set(ledsr, 15, 14, 94, 0);
                ledbuffer_set(ledsr, 16, 29, 87, 0);
                ledbuffer_set(ledsr, 17, 43, 80, 0);
                ledbuffer_set(ledsr, 18, 58, 72, 0);
                ledbuffer_set(ledsr, 19, 72, 65, 0);
                ledbuffer_set(ledsr, 20, 87, 58, 0);
                ledbuffer_set(ledsr, 21, 102, 51, 0);
                ledbuffer_set(ledsr, 22, 87, 58, 0);
                ledbuffer_set(ledsr, 23, 72, 65, 0);
                ledbuffer_set(ledsr, 24, 58, 72, 0);
                ledbuffer_set(ledsr, 25, 43, 80, 0);
                ledbuffer_set(ledsr, 26, 29, 87, 0);
                ledbuffer_set(ledsr, 27, 14, 94, 0);
                

                ape = !ape;
            
                counter = 0;
            }

            ledbuffer_write (ledsr);
            if (ape) {
                ledbuffer_advance (ledsr, -1);
            } else{
                ledbuffer_advance (ledsr, 1);
            }
            dpacer = 0;

        }
     }

    return 0;
}