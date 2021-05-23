#include "radio_comm.h"
#include <stdbool.h>
#include "nrf24.h"
#include "delay.h"
#include "pio.h"
#include "command.h"

static nrf24_t *nrf;

static void panic(void)
{
    while (1) {
        pio_output_toggle(LED2_PIO);
        delay_ms(100);
    }
}

void radio_init(int channel)
{    

    //configuring the spi interface.
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
    if (!nrf24_begin(nrf, channel, NRF_ADDRESS1, NRF_PAYLOAD_SIZE))
        panic();
    if (!nrf24_listen(nrf))
        panic();
}

bool radio_read(char *buffer, uint8_t len)
{
    if(nrf24_read(nrf, buffer, len))
    {
        return true;
    }
    return false;
}

int8_t radio_write(char *buffer, uint8_t len)
{
    if(!nrf24_write(nrf, buffer, len))
        return -1;
    else
        return 0;

}


bool radio_transmit_command(struct Command cmd){
    char buffer[12] = {0};
    sprintf (buffer, "%u?%d?%d", cmd.cmd, cmd.arg1, cmd.arg2);
    if (! nrf24_write(nrf, buffer, sizeof (buffer)))
        return false;
    else
        return true;
}

struct Command radio_read_command(void){
    char r_buff[12] = {0};
    if (radio_read(r_buff, sizeof(r_buff))){
        struct Command cmd = str2cmd(r_buff);
        return cmd;
    }else{
        return create_command(NOTHING, 0, 0);
    }
}
