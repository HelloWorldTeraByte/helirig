#include "target.h"
#include "pacer.h"
#include "pio.h"
#include "motors.h"
#include "radio_comm.h"
#include "servos.h"

#include "stdlib.h"
#include "string.h"

void racer_init(void)
{
    /* Configure LED PIO as output.  */
    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED2_PIO, PIO_OUTPUT_HIGH);

    /* Initilize the motors*/
    motors_init();

    radio_init();

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
    uint8_t mtr_l_speed;
    uint8_t mtr_r_speed;
    sprintf(w_buff, "Ohh OOH\r\n");

    /* Initilize the car - GPIO, pacer and motors*/
    racer_init();

    while (1) {
        /* Wait until next clock tick.  */
        pacer_wait();

        flash_ticks++;
        if (flash_ticks >= LOOP_POLL_RATE / (LED_FLASH_RATE * 2))
        {
            flash_ticks = 0;
            pio_output_toggle(LED1_PIO);
            radio_write(w_buff, sizeof(w_buff));
        }
        count++;
        if (count > 100)
            count = 0;

        radio_read(r_buff, sizeof(r_buff));

        char *pch;
        pch = strtok(r_buff, ",");
        mtr_l_speed = atoi(pch);
        motor_left_set(80);
        pch = strtok(NULL, ",");
        mtr_r_speed = atoi(pch);
        motor_right_set(80);

        radio_ticks++;
        if (radio_ticks >= LOOP_POLL_RATE / (RADIO_POLL_RATE * 2))
        {
            radio_ticks = 0;
        }
     }

    return 0;
}