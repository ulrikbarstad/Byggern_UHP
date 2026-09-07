#include <avr/io.h>

#define F_CPU 4915200UL
#include <util/delay.h>

#include <stdio.h>
#include "uart.h"


#define LED PA2

int main(void)
{
    DDRA |= (1 << LED);
    uart_init();
    FILE *uart_stream = fdevopen(uart_transmit, uart_recieve);
    stdout = uart_stream;
    stdin = uart_stream;

    while (1)
    {
        printf("Hello World! ");
        /*
        PORTA &= ~(1 << LED);
        unsigned char data = uart_recieve();
        if (data == 'p'){
            PORTA |= (1 << LED);
            _delay_ms(500);
            PORTA &= ~(1 << LED);
        }
        
        uart_transmit('B');
        
        _delay_ms(500);
        */
        _delay_ms(500);
    }

    return 0;
}
