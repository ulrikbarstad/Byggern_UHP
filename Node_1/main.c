#include <avr/io.h>
#include <util/delay.h>


#include "uart.h"

#define LED PA2

int main(void)
{
    DDRA |= (1 << LED);

    while (1)
    {
        PORTA |= (1 << LED);
        uart_transmit('A')
        _delay_ms(500);

        PORTA &= ~(1 << LED);
        _delay_ms(500);
    }

    return 0;
}
