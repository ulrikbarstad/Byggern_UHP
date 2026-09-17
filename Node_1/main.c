#define F_CPU 4915200UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "uart.h"
#include "sram.h"
#include "adc.h"


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
   
    FILE *uart_stream = fdevopen(uart_transmit, uart_recieve);
    stdout = uart_stream;
    stdin = uart_stream;

   
    // Mask PC4-PC7 so JTAG can use them.
    // PC0-PC3 remain A8-A11.
    SFIOR &= ~((1 << XMM2) | (1 << XMM1) | (1 << XMM0));
    SFIOR |= (1 << XMM2);


    

    
    //SRAM_test();
    
    adc_init();
    

    while (1) {
        uint8_t x;
        uint8_t y;

        adc_read_touchpad(&x, &y);

        printf("X: %u, Y: %u\r\n", (unsigned)x, (unsigned)y);

        _delay_ms(10);
    }
    return 0;
}
