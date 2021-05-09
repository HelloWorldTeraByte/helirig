#include "target.h"
#include "pacer.h"
#include "pio.h"
#include "motors.h"
#include "radio_comm.h"

/* Define how fast ticks occur.  This must be faster than
   TICK_RATE_MIN.  */
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
    flash_ticks = 0;

    /* Initilize the car - GPIO, pacer and motors*/
    racer_init();

    char buffer[32];
    sprintf(buffer, "Ohh OOH\r\n");

    motor_left_set(0);
    motor_right_set(0);

    while (1) {
        /* Wait until next clock tick.  */
        pacer_wait();

        flash_ticks++;
        if (flash_ticks >= LOOP_POLL_RATE / (LED_FLASH_RATE * 2))
        {
            flash_ticks = 0;
            pio_output_toggle(LED1_PIO);
            radio_write(buffer, sizeof(buffer));
        }
    }
    return 0;
}