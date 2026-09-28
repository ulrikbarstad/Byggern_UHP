#define F_CPU 4915200UL

#include <util/delay.h>
#include "spi.h"
#include "io.h"

#define IO_CMD_BUTTONS 0x04

Buttons io_read_buttons(void){
    Buttons buttons;
    spi_select_slave(SPI_SLAVE_IO);
    spi_write(IO_CMD_BUTTONS);
    _delay_us(40);
    buttons.right = spi_read();
    _delay_us(2);
    buttons.left = spi_read();
    _delay_us(2);
    buttons.nav = spi_read();
    spi_deselect_all();
    return buttons;
}