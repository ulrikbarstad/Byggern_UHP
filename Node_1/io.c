#define F_CPU 4915200UL

#include <stdio.h>
#include <util/delay.h>
#include "spi.h"
#include "io.h"
#include "interrupt.h"
#include "oled.h"
#include "position.h"
#include "adc.h"

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

void io_check_joystick_pressed(MenuOption selected_mode){
    if (joystick_pressed) {
        joystick_pressed = 0;
        printf("Meny valgt: %s\r\n", menu_option_to_string(selected_mode));
    }
}

MenuOption io_check_joystick_tick(MenuOption selected_mode, Position_Calibration cal_joy){
    if(joystick_tick){
        joystick_tick = 0;
        Position_Direction dir = adc_joystick_direction(cal_joy, 10);
        if(dir != 0) {
            selected_mode = oled_menu_switch(dir, selected_mode);
    
        }
    }
    return selected_mode;
}

void io_check_buttons(){
    Buttons buttons = io_read_buttons();

        if (buttons.R1) {
            printf("R1 pressed\r\n");
        }

        if (buttons.L1) {
            printf("L1 pressed\r\n");
        }

        if (buttons.NU) {
            printf("Nav up\r\n");
        }

        if (buttons.NB) {
            printf("Nav button\r\n");
        }
}