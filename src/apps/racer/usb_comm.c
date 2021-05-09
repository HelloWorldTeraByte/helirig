#include "usb_comm.h"

static void usb_comm_init(void)
{
    usb_cdc_t usb_cdc;
    int flash_ticks = 0;
    int i;

    // Create non-blocking tty device for USB CDC connection.
    usb_serial = usb_serial_init(&usb_serial_cfg, "");

    for (i = 0; i < 100; i++)
    {
        usb_serial_puts(usb_serial, "Hello world\n");
        delay_ms(100);
    }

    prompt_command();
}

static void prompt_command(void)
{
    usb_serial_puts (usb_serial, "> ");
}

static void process_command(void)
{
    char buffer[80];
    char *str;
    
    str = usb_serial_gets (usb_serial, buffer, sizeof (buffer));
    if (! str)
        return;

    // usb_serial (usb_serial, "<<<%s>>>\n", str);
    
    switch (str[0])
    {
    case '0':
        break;
        
    case '1':
        break;

    case 'h':
        usb_serial_puts (usb_serial, "Hello world!\n");
        break;

    default:
       break;
    }

    prompt_command ();
}