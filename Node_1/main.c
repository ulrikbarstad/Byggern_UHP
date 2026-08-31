#include <avr/io.h>
#include <util/delay.h>

#define LED PA2

int main(void)
{
    // Sett PA2 som output
    DDRA |= (1 << LED);

    while (1)
    {
        // LED på
        PORTA |= (1 << LED);
        _delay_ms(500);

        // LED av
        PORTA &= ~(1 << LED);
        _delay_ms(500);
    }

    return 0;
}