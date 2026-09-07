#define F_CPU 4915200UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "uart.h"
#include "sram.h"


#include <stdlib.h>
#include <stdint.h>


void SRAM_test(void)
{
    volatile char *ext_ram = (char *)0x1800;   // Start address for SRAM
    uint16_t ext_ram_size = 0x800;

    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;

    printf("Starting SRAM test...\n");

    // rand() stores some internal state, so calling this function
    // repeatedly will give different seeds unless srand() was called before.
    uint16_t seed = rand();

    // Write phase: write a random value and immediately read it back
    srand(seed);

    for (uint16_t i = 0; i < ext_ram_size; i++)
    {
        uint8_t some_value = rand();

        ext_ram[i] = some_value;

        uint8_t retrieved_value = ext_ram[i];

        if (retrieved_value != some_value)
        {
            printf(volatile uint8_t *sram = (volatile uint8_t *)0x1800;
                "Write phase error: ext_ram[%4d] = %02X (should be %02X)\n",
                i,
                retrieved_value,
                some_value
            );

            write_errors++;
        }
    }

    // Retrieval phase:
    // Check that none of the values changed during/after the write phase
    srand(seed);

    for (uint16_t i = 0; i < ext_ram_size; i++)
    {
        uint8_t some_value = rand();
        uint8_t retrieved_value = ext_ram[i];

        if (retrieved_value != some_value)
        {
            printf(
                "Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n",
                i,
                retrieved_value,
                some_value
            );

            retrieval_errors++;
        }
    }

    printf(
        "SRAM test completed with\n"
        "%4d errors in write phase and\n"
        "%4d errors in retrieval phase\n\n",
        write_errors,
        retrieval_errors
    );
}




int main(void)
{
    /*
    DDRA |= (1 << LED);
    uart_init();
    FILE *uart_stream = fdevopen(uart_transmit, uart_recieve);
    stdout = uart_stream;
    stdin = uart_stream;
    */
    uart_init();
    

    FILE *uart_stream = fdevopen(uart_transmit, uart_recieve);
    stdout = uart_stream;
    stdin = uart_stream;

   
    // Mask PC4-PC7 so JTAG can use them.
    // PC0-PC3 remain A8-A11.
    SFIOR &= ~((1 << XMM2) | (1 << XMM1) | (1 << XMM0));
    SFIOR |= (1 << XMM2);

    sram_init();

    SRAM_test();

    

    


    while (1) {
        _delay_ms(100);
       
        
    }
    return 0;
}
