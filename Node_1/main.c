#define F_CPU 4915200UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "uart.h"
#include "sram.h"
#include "adc.h"
#include "position.h"
#include "spi.h"


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
    FILE *uart_stream = fdevopen(uart_transmit, uart_recieve);
    stdout = uart_stream;
    stdin = uart_stream;

   
    // Mask PC4-PC7 so JTAG can use them.
    // PC0-PC3 remain A8-A11.
    SFIOR &= ~((1 << XMM2) | (1 << XMM1) | (1 << XMM0));
    SFIOR |= (1 << XMM2);

    
    

    
    //SRAM_test();DISPLAY
    
    adc_init();
    
    Position_Calibration cal_joy = {245, 72, 240, 79, 165, 161};

    Position_Calibration cal_touch = {255, 2, 255, 2, 0, 0};
    spi_select_slave(SPI_SLAVE_DISPLAY);

/*
    Position_Calibration cal_joy = adc_calibrate(adc_read_joystick);
    printf("Joystick Calibration Done\r\n------------\r\n");
    Position_Calibration cal_touch = adc_calibrate(adc_read_touchpad);
    printf("Joystick Calibration Done\r\n------------\r\n");
    printf("Joystick Calibration: \r\nmax x: %u \r\nmin x: %u \r\nmax y: %u \r\nmin y: %u \r\nneutral x: %u \r\nneutral y: %u \r\n------------\r\nToucpad Calibration: \r\nmax x: %u \r\nmin x: %u \r\nmax y: %u \r\nmin y: %u \r\nneutral x: %u \r\nneutral y: %u\r\n------------\r\n\r\n", cal_joy.max_x_pos, cal_joy.min_x_pos, cal_joy.max_y_pos, cal_joy.min_y_pos, cal_joy.neutral_x, cal_joy.neutral_y, cal_touch.max_x_pos, cal_touch.min_x_pos, cal_touch.max_y_pos, cal_touch.min_y_pos, cal_touch.neutral_x, cal_touch.neutral_y);
*/
    while (1) {
        
        Position pos_joy = adc_joystick_position(cal_joy);
        Position_Direction dir_joy = adc_joystick_direction(cal_joy, 10);
        Position pos_touch = adc_touchpad_position(cal_touch);
        

        printf("Joystick pos X: %i, Y: %i\r\n", pos_joy.x_pos, pos_joy.y_pos);
        printf("Joystick dir: %u\r\n", dir_joy);
        printf("Touchpod pos X: %i, Y: %i\r\n", pos_touch.x_pos, pos_touch.y_pos);
        printf("\n");

        _delay_ms(5000);
    }
    return 0;
}
