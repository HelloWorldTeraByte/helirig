#include "radio_comm.h"
#include "nrf24.h"
#include "delay.h"
#include "pio.h"

static void panic(void)
{
    while (1) {
        pio_output_toggle(LED_ERROR);
        delay_ms(400);
    }
}

static nrf24_t *nrf;
static spi_t spi;

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

    // initialize the NRF24 radio with its unique 5 byte address
    if (!nrf24_begin(nrf, 2, 0x1234567893, 12))
        panic();
}

void radio_transmit(int motor_1_val, int motor_2_val){
    char buffer[12] = {0};
    sprintf (buffer, "%d,%d\n",motor_1_val, motor_2_val);
    //printf("%d,%d \n", motor_1_val, motor_2_val);
    if (! nrf24_write(nrf, buffer, sizeof (buffer)))
        pio_output_set(LED_STATUS, 0);
    else
        pio_output_set(LED_STATUS, 1);
}