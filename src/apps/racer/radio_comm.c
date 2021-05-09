#include "radio_comm.h"
#include "nrf24.h"
#include "delay.h"
#include "pio.h"

static nrf24_t *nrf;

static void panic(void)
{
    while (1) {
        pio_output_toggle(LED2_PIO);
        delay_ms(500);
    }
}

void radio_init(void)
{    
    spi_cfg_t nrf_spi = {
        .channel = 0,
        .clock_speed_kHz = 1000,
        .cs = RADIO_CS_PIO,
        .mode = SPI_MODE_0,
        .cs_mode = SPI_CS_MODE_FRAME,
        .bits = 8,
    };
    spi_t spi;
  
    spi = spi_init(&nrf_spi);
    nrf = nrf24_create(spi, RADIO_CE_PIO, RADIO_IRQ_PIO);
    if (!nrf)
        panic();

    // initialize the NRF24 radio with its unique 5 byte address
    if (!nrf24_begin(nrf, 4, 0x0123456789, 32))
        panic();
    if (!nrf24_listen(nrf))
        panic();
}

void radio_read(char *buffer, uint8_t len)
{
    if(nrf24_read(nrf, buffer, len))
    {
        printf("%s\n", buffer);
    }
}

int8_t radio_write(char *buffer, uint8_t len)
{
    if(!nrf24_write(nrf, buffer, len))
        return -1;
    else
        return 0;

}
