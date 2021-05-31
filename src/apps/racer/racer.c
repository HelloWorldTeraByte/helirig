#include "target.h"
#include "pacer.h"
#include "pio.h"
#include "motors.h"
#include "servos.h"
#include "radio_comm.h"
#include "servos.h"
#include "usb_comm.h"
#include "power.h"
#include "led_tape.h"
#include "bumper.h"
#include "buttons.h"
#include "delay.h"
#include "mcu_sleep.h"
#include "state_manager.h"

//TODO:
//Timeout for motors
//Mount everything HAT & CAR
//Change loop rate
//Sleep
//Caps on motors

static const mcu_sleep_wakeup_cfg_t sleep_wakeup_cfg = 
{
    .pio = BUTTON_WAKEUP_PIO,
    .active_high = true
};

static const mcu_sleep_cfg_t sleep_cfg = 
{
    .mode = MCU_SLEEP_MODE_WAIT
};

static struct Command command_tx;
static struct Command command_rx;

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

    //pio_irq_config_t pio_sleep_cfg = PIO_IRQ_FALLING_EDGE;
    pio_config_set(BUTTON_WAKEUP_PIO, PIO_PULLUP);
    //pio_irq_config_set(BUTTON_WAKEUP_PIO, pio_sleep_cfg);
    //pio_irq_enable(BUTTON_WAKEUP_PIO);
    mcu_sleep_wakeup_set(&sleep_wakeup_cfg);

    pio_init(RADIO_JUMPER1);
    pio_init(RADIO_JUMPER2);
    pio_init(RADIO_JUMPER3);
    pio_init(RADIO_JUMPER4);
}

void racer_init(void)
{
    gpio_init();

    motors_init();

    servos_init();

    /* Initilize the USB communication interface*/
    //usb_comm_init();

    power_sense_init();

    bumper_init();

    btns_init();

    ledt_init();

    state_manager_init();

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

    command_tx = create_command(INVALID, 0, 0);
    command_rx = create_command(INVALID, 0, 0);
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
        go_ape_mode();
        //mcu_sleep(&sleep_cfg);
    }
}

void racer_bumper_manage(void)
{
    bumper_update();
    if(b_go_hit_mode()) {
        go_hit_mode();
        motor_lock();
        pio_output_toggle(LED_STAT0);
    }
    if(b_exit_hit_mode()) {
        go_normal_mode();
        motor_unlock();
        pio_output_toggle(LED_STAT0);
    }
}

int main(void)
{   
    // TODO: make this smaler unint8?
    uint16_t loop_m_ticks = 0;
    uint16_t loop_u_ticks = 0;
    uint16_t loop_n_ticks = 0;

    uint16_t loop_rf_tx_ticks = 0;
    uint16_t loop_rf_rx_ticks = 0;

    uint8_t rf_cmd_num = 0;

    /* Initilize the car - GPIO, pacer and motors*/
    racer_init();

    while (1) {
        /* Wait until next clock tick.  */
        pacer_wait();

        loop_rf_rx_ticks++;
        if(loop_rf_rx_ticks >= LOOP_POLL_RATE / (LOOP_RATE_RF_RX * 2))
        {
            loop_rf_rx_ticks = 0;
            command_rx = radio_read_command();
            switch (command_rx.cmd)
            {
            case (int)MOTOR_SPEED:
                if( (command_rx.arg1 >= -100 && command_rx.arg1 <= 100) && (command_rx.arg2 >= -100 && command_rx.arg2<= 100)) {
                    motor_left_set(command_rx.arg1);
                    motor_right_set(command_rx.arg2);
                }
                break;
            case (int)APE_CMD:
                if(command_rx.arg1 == 1)
                    go_ape_mode();
               break;
            default:
                break;
            }
        }
 
        loop_rf_tx_ticks++;
        if(loop_rf_tx_ticks >= LOOP_POLL_RATE / (LOOP_RATE_RF_TX * 2))
        {
            loop_rf_tx_ticks = 0;

            command_tx = create_racer_state_command((int)get_current_state());
            radio_transmit_command(command_tx);
       }
 

        loop_m_ticks++;
        if (loop_m_ticks >= LOOP_POLL_RATE / (LOOP_RATE_M * 2))
        {
            loop_m_ticks = 0;

            racer_power_manage();
            state_manage();

            // Heart beat
            pio_output_toggle(LED_STAT1);
        }

        loop_u_ticks++;
        if (loop_u_ticks >= LOOP_POLL_RATE / (LOOP_RATE_U * 2))
        {
            loop_u_ticks = 0;

            racer_bumper_manage();
            racer_io_manage();
            servo_update();

            ledt_run();
        }
   }

   return 0;
}