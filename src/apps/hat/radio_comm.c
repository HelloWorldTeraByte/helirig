#include "radio_comm.h"
#include "nrf24.h"
#include "delay.h"
#include "pio.h"
#include "target.h"
#include <pio.h>

static void panic(void)
{
    while (1) {
        pio_output_toggle(LED_ERROR);
        delay_ms(400);
    }
}

static nrf24_t *nrf;
static spi_t spi;

int channel_select(void){
    int8_t channel_num;

    pio_config_set (RADIO_JUMPER1, PIO_OUTPUT_LOW);
    pio_config_set (RADIO_JUMPER2, PIO_OUTPUT_LOW);
    pio_config_set (RADIO_JUMPER3, PIO_OUTPUT_LOW);
    pio_config_set (RADIO_JUMPER4, PIO_OUTPUT_LOW);

    if (pio_input_get(RADIO_JUMPER1)){
        channel_num = 1;
        pio_config_set (LED_STATUS, PIO_OUTPUT_HIGH);
    } else if (pio_input_get(RADIO_JUMPER2)){
        channel_num = 2;
        pio_config_set (LED_LOW_BAT, PIO_OUTPUT_HIGH);
    } else if (pio_input_get(RADIO_JUMPER3)){
        channel_num = 3;
        pio_config_set (LED_ERROR, PIO_OUTPUT_HIGH);
    } else {
        channel_num = 4;
        pio_config_set (LED_DEBUG, PIO_OUTPUT_HIGH);
    }

    return channel_num;
}

void radio_begin(int8_t channel_n){

    
    bool radio_is_on;

    radio_is_on = nrf24_begin(nrf, channel_n, 0x1234567893, 12);

    if (!radio_is_on)
        panic();
}

void radio_comm_init(void){
    spi_cfg_t nrf_spi = {
        .channel = 0,
        .clock_speed_kHz = 1000,
        .cs = RADIO_CS_PIO,
        .mode = SPI_MODE_0,
        .cs_mode = SPI_CS_MODE_FRAME,
        .bits = 8,
    };

    spi = spi_init(&nrf_spi);
    nrf = nrf24_create(spi, RADIO_CE_PIO, RADIO_IRQ_PIO);
    if (!nrf)
        panic();

    int radio_channel = channel_select();

    radio_begin(radio_channel);
    // initialize the NRF24 radio with its unique 5 byte address
}

void radio_transmit(int motor_1_val, int motor_2_val){
    char buffer[12] = {0};
    sprintf (buffer, "%d,%d\n",motor_1_val, motor_2_val);
    //printf("%d,%d \n", motor_1_val, motor_2_val);
    //printf("%d", channel_select());
    if (! nrf24_write(nrf, buffer, sizeof (buffer)))
        pio_output_set(LED_STATUS, 0);
    else
        pio_output_set(LED_STATUS, 1);
}