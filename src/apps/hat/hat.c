
#include "target.h"
#include "nrf24.h"
#include "pacer.h"
#include "joystick.h"


static void radio_fault(void)
{

}


int
main (void)
{
    // TODO: write hat program here...

    //init leds
    pio_config_set (LED_STATUS, PIO_OUTPUT_LOW);
    pio_config_set (LED_LOW_BAT, PIO_OUTPUT_LOW);
    pio_config_set (LED_ERROR, PIO_OUTPUT_LOW);
    pio_config_set (LED_DEBUG, PIO_OUTPUT_LOW);



    //init radio
    spi_cfg_t nrf_spi = {
        .channel = 0,
        .clock_speed_kHz = 1000,
        .cs = RADIO_CS_PIO,
        .mode = SPI_MODE_0,
        .cs_mode = SPI_CS_MODE_FRAME,
        .bits = 8,
    };
    nrf24_t *nrf;
    spi_t spi;

    spi = spi_init(&nrf_spi);
    nrf = nrf24_create(spi, RADIO_CE_PIO, RADIO_IRQ_PIO);
    if (!nrf)
        radio_fault();

    // initialize the NRF24 radio with its unique 5 byte address
    if (!nrf24_begin(nrf, 2, 0x1234567893, 12))
        radio_fault();



    //init pacer
    pacer_init(10);


    while(1)
    {
        pacer_wait ();
        
    }
}
