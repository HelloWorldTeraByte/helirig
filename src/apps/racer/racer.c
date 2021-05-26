#include "target.h"
#include "pacer.h"
#include "pio.h"
#include "motors.h"
#include "servos.h"
#include "radio_comm.h"
#include "servos.h"
#include "usb_comm.h"
#include "power.h"
#include "ledbuffer.h"
#include "led_tape.h"
#include "bumper.h"
#include "buttons.h"
#include "delay.h"
#include "mcu_sleep.h"

#include "stdlib.h"
#include "string.h"

//TODO:TX and RX radio at different frequncies
// Sleep
// Servos
// Enable the motors with a command

static const mcu_sleep_wakeup_cfg_t sleep_wakeup_cfg = 
{
    .pio = BUTTON_WAKEUP_PIO,
    .active_high = false
};

static const mcu_sleep_cfg_t sleep_cfg = 
{
    .mode = MCU_SLEEP_MODE_WAIT
};

void gpio_init(void)
{
    /* Configure LED PIO as output.  */
    pio_config_set(LED_STAT0, PIO_OUTPUT_HIGH);
    pio_config_set(LED_STAT1, PIO_OUTPUT_HIGH);
    pio_config_set(LED_LOW_BATT, PIO_OUTPUT_HIGH);

    /* Configure radio channels */
    pio_config_set(RADIO_JUMPER1, PIO_PULLDOWN);
    pio_config_set(RADIO_JUMPER2, PIO_PULLDOWN);
    pio_config_set(RADIO_JUMPER3, PIO_PULLDOWN);
    pio_config_set(RADIO_JUMPER4, PIO_PULLDOWN);

    pio_irq_config_t pio_sleep_cfg = PIO_IRQ_FALLING_EDGE;
    pio_config_set(BUTTON_WAKEUP_PIO, PIO_PULLUP);
    pio_irq_config_set(BUTTON_WAKEUP_PIO, pio_sleep_cfg);
    pio_irq_enable(BUTTON_WAKEUP_PIO);
    mcu_sleep_wakeup_set(&sleep_wakeup_cfg);

    pio_init(RADIO_JUMPER1);
    pio_init(RADIO_JUMPER2);
    pio_init(RADIO_JUMPER3);
    pio_init(RADIO_JUMPER4);
}

void racer_init(void)
{
    gpio_init();

    /* Initilize the motors*/
    motors_init();

    servos_init();

    /* Initilize the USB communication interface*/
    //usb_comm_init();

    power_sense_init();

    bumper_init();

    btns_init();

    pacer_init(LOOP_POLL_RATE);

    delay_ms(100); 
    if (pio_input_get(RADIO_JUMPER1))
    {
        radio_init(NRF_CHNNEL1);
    //}else if (pio_input_get(RADIO_JUMPER2)){
        //radio_init(NRF_CHNNEL2);
    }else if (pio_input_get(RADIO_JUMPER3)){
        radio_init(NRF_CHNNEL3);
    }else if (pio_input_get(RADIO_JUMPER4)){
        radio_init(NRF_CHNNEL4);
    }else{
        radio_init(NRF_CHNNEL5);
    }
}

void racer_power_manage(void)
{
    // TODO: turn off H bridge, LED Tape, radio
    if(power_is_batt_low()) {
        pio_config_set(LED_LOW_BATT, PIO_OUTPUT_LOW);
        //motors_sleep();
   }
    else {
        //TODO: Wakeup the motors?
        pio_config_set(LED_LOW_BATT, PIO_OUTPUT_HIGH);
    }
}

void racer_io_manage(void)
{
    btns_update();
    if(is_btn0_pressed()) {
        pio_config_set(LED_STAT0, PIO_OUTPUT_LOW);
        //servo1_duty_set(20);
        mcu_sleep(&sleep_cfg);
    }
}

void racer_bumper_manage(void)
{
    bumper_update();
    if(bumper_is_hit()) {
        motor_lock();
        pio_config_set(LED_STAT0, PIO_OUTPUT_LOW);
    }
    else {
        motor_unlock();
        pio_config_set(LED_STAT0, PIO_OUTPUT_HIGH);
    }
}

int main(void)
{   
    // TODO: make this smaler unint8?
    uint16_t loop_m_ticks = 0;
    uint16_t loop_u_ticks = 0;

    int dpacer = 0;
    bool ape_mode = 0;
    ledbuffer_t *ledsr;

    /* Initilize the car - GPIO, pacer and motors*/
    racer_init();

    ledsr = ledt_init();

    struct Command command_tx = create_command(INVALID, 0, 0);
    struct Command command_rx = create_command(INVALID, 0, 0);

    while (1) {
        /* Wait until next clock tick.  */
        pacer_wait();

        loop_m_ticks++;
        if (loop_m_ticks >= LOOP_POLL_RATE / (LOOP_RATE_M * 2))
        {
            loop_m_ticks = 0;
            racer_power_manage();
            // Heart beat
            pio_output_toggle(LED_STAT1);
        }

        loop_u_ticks++;
        if (loop_u_ticks >= LOOP_POLL_RATE / (LOOP_RATE_U * 2))
        {
            loop_u_ticks = 0;

            //TODO:Move this the bumper manage
            if (bumper_is_hit()){
                command_tx = create_bumper_command(true);
            }else{
                command_tx = create_bumper_command(false);
            }

            command_rx = radio_read_command();
            radio_transmit_command(command_tx);
            switch (command_rx.cmd)
            {
            case (int)MOTOR_SPEED:
                motor_left_set(command_rx.arg1);
                motor_right_set(command_rx.arg2);
                break;
            default:
                break;
            }
            racer_bumper_manage();

            racer_io_manage();
        }

        if(dpacer++ == 19) {
            ledt_run(ape_mode, ledsr);
            dpacer = 0;
        }
    }
   return 0;
}
