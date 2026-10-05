#define F_CPU 4915200UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "uart.h"
#include "sram.h"
#include "adc.h"
#include "position.h"
#include "spi.h"
#include "oled.h"
#include "interrupt.h"
#include "io.h"
#include "can.h"
#include "mcp2515.h"


#include <stdlib.h>
#include <stdint.h>


#define SRAM_BASE  0x1800
#define SRAM_SIZE  0x0800


volatile uint8_t * const SRAM = (volatile uint8_t *)SRAM_BASE;






int main(void)
{
    /*
    DDRA |= (1 << LED);
    uart_init();
    FILE *uart_stream = fdevopen(uart_transmit, uart_recieve);
    stdout = uart_stream;
    stdin = uart_stream;
     volatile char *addy = (char *)0x1FFF;
    */
    sram_init();
    uart_init();


    
    spi_init();
    spi_deselect_all();
    oled_init();
    joystick_timer_init();
    adc_init();
    joystick_button_init();
    mcp2515_init(true);
    sei();
    

    FILE *uart_stream = fdevopen(uart_transmit, uart_recieve);
    stdout = uart_stream;
    stdin = uart_stream;
    

   
    // Mask PC4-PC7 so JTAG can use them.
    // PC0-PC3 remain A8-A11.
    SFIOR &= ~((1 << XMM2) | (1 << XMM1) | (1 << XMM0));
    SFIOR |= (1 << XMM2);

    
    

    
    //SRAM_test();DISPLAY
    
    
    
    Position_Calibration cal_joy = {245, 72, 240, 79, 165, 161};

    Position_Calibration cal_touch = {255, 2, 255, 2, 0, 0};

    MenuOption selected_mode = EASY_MODE;
    oled_main_menu(selected_mode);

    CAN_Message tx = {
        .id = 0x123,
        .length = 3,
        .data = {0xAA, 0xBB, 0xCC}
    };
    CAN_Message rx;

    can_send(&tx);

    _delay_ms(10);

    if (can_receive(&rx)) {

        printf("Received message:\r\n");
        printf("ID:     0x%03X\r\n", rx.id);
        printf("Length: %u\r\n", rx.length);
        printf("Data: ");

        for (uint8_t i = 0; i < rx.length; i++) {
            printf("0x%02X ", rx.data[i]);
        }

        printf("\r\n");
    }
    else {
        printf("No CAN message received\r\n");
    }



    

/*
    Position_Calibration cal_joy = adc_calibrate(adc_read_joystick);
    printf("Joystick Calibration Done\r\n------------\r\n");
    Position_Calibration cal_touch = adc_calibrate(adc_read_touchpad);
    printf("Joystick Calibration Done\r\n------------\r\n");
    printf("Joystick Calibration: \r\nmax x: %u \r\nmin x: %u \r\nmax y: %u \r\nmin y: %u \r\nneutral x: %u \r\nneutral y: %u \r\n------------\r\nToucpad Calibration: \r\nmax x: %u \r\nmin x: %u \r\nmax y: %u \r\nmin y: %u \r\nneutral x: %u \r\nneutral y: %u\r\n------------\r\n\r\n", cal_joy.max_x_pos, cal_joy.min_x_pos, cal_joy.max_y_pos, cal_joy.min_y_pos, cal_joy.neutral_x, cal_joy.neutral_y, cal_touch.max_x_pos, cal_touch.min_x_pos, cal_touch.max_y_pos, cal_touch.min_y_pos, cal_touch.neutral_x, cal_touch.neutral_y);
*/
    while (1) {
        
        /*
        io_check_joystick_pressed(selected_mode);
        selected_mode = io_check_joystick_tick(selected_mode, cal_joy);
        io_check_buttons();

        */

        //can_write(0b00000000, 1, 0x0);

        //mcp2515_reset();
      
       
        
        _delay_ms(1000);
                    

    }
    return 0;
}
