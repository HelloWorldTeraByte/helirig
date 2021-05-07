#include "target.h"
#include "pacer.h"
#include "pio.h"
#include "motors.h"

/* Define how fast ticks occur.  This must be faster than
   TICK_RATE_MIN.  */
enum {LOOP_POLL_RATE = 200};

/* Define LED flash rate in Hz.  */
enum {LED_FLASH_RATE = 2};

/* Define Motor speed update rate in Hz.  */
enum {MOTOR_UPDATE_RATE = 10};

void racer_init(void)
{
    /* Configure LED PIO as output.  */
    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED2_PIO, PIO_OUTPUT_HIGH);

    /* Initilize the motors*/
    motors_init();
}

int main(void)
{   
    uint8_t flash_ticks;
    uint8_t speed = 0;
    pacer_init(LOOP_POLL_RATE);
    flash_ticks = 0;

    /* Initilize the car - GPIO and motors*/
    racer_init();

    motor_left_set(50);
    motor_right_set(50);

    while (1) {
        /* Wait until next clock tick.  */
        pacer_wait();

        flash_ticks++;
        if (flash_ticks >= LOOP_POLL_RATE / (LED_FLASH_RATE * 2))
        {
            flash_ticks = 0;
            /* Toggle LED.  */
            pio_output_toggle(LED1_PIO);
            pio_output_toggle(LED2_PIO);

        }
    }
    return 0;
}