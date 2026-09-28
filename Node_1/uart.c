#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

#include "uart.h"

#define F_OSC 4915200UL
#define BAUD 9600UL
#define UBRR_VALUE ((F_OSC/(16UL*BAUD)) - 1)

void uart_init() {
    //Set baud rate for transmission. UBRR0H contains 4 msb in UBRR_VALUE and UBRR0L contains the 8 LSB
    //UBRR_VALUE is 31 with chosen BAUD and F_OSC. This makes it so that URSEL0 (MSB in UBRR0H) is 0. URSEL0 must be 0 when updating this register with bitrate.
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)(UBRR_VALUE);
    
    //Set frame format. Want 8 data bits, no parity and 1 stop bit
    //We want to access the UCSRC register so the MSB should be 1 (still writing to UBRR0H)
    UCSR0C = 0b10000110;
    //UCSR0B &= ~(0b00000100); // set UCSZ2 to 0 so that we get 8 data bits.

    //Enable Reciever and Transmitter
    //UCSR0B |= 0b10011000; 
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

}

int uart_transmit(char data, FILE *stream){
    //wait until UDRE bit in USCRA is set to 1 which means ready to transmit
    while ( !(UCSR0A & (1 << UDRE0)) )
        ;

    //transmit data
    UDR0 = data;
    return 0;
}


int uart_recieve(FILE *stream){
    //wait until RXC bit in USCRA is set to 1 which means ready to recieve
    while ( !(UCSR0A & (1 << RXC0)))
        ;
    
    // read data in register UDR0
    return UDR0;
}