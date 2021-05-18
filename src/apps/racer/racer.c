#include "target.h"
#include "pacer.h"
#include "pio.h"
#include "motors.h"
#include "radio_comm.h"
#include "servos.h"
#include "usb_comm.h"

#include "stdlib.h"
#include "string.h"

void racer_init(void)
{
    /* Configure LED PIO as output.  */
    pio_config_set (LED1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LED2_PIO, PIO_OUTPUT_LOW);

    /* Initilize the motors*/
    motors_init();

    radio_init();

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
     }

    return 0;
}