#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
//#include <util/delay.h>
#include "spi.h"


#define CS_DISPLAY PB2
#define CS_IO PB3

#define CS_CAN PB0

void spi_init(void){
    DDRB |= (1 << PB4)  
    | (1 << CS_IO) 
    | (1 << CS_DISPLAY) 
    | (1 << PB5) 
    | (1 << PB7)
    | (1 << CS_CAN); //satt til output


    DDRB &= ~(1 << PB6); // miso satt til input

    PORTB |= (1 << PB4); // holde pinnen høy
   
    PORTB |= (1 << CS_IO) 
    | (1 << CS_DISPLAY)
    | (1 << CS_CAN); // deselect  IO og display

    SPCR = (1 << SPE) 
    | (1 << MSTR) 
    | (1 << SPR0); // SPI enable, avr i SPI master mode, klokke frekvens f/16
    

}

uint8_t spi_transfer(uint8_t data){
    SPDR = data;

    while(!(SPSR & (1 << SPIF))){
        ;
    }
    return SPDR;
}

void spi_write(uint8_t data){

    (void)spi_transfer(data);
}

uint8_t spi_read(void){
    return spi_transfer(0x00);
}

void spi_deselect_all(void){
    PORTB |= (1 << CS_IO) 
    | (1 << CS_DISPLAY)
    | (1 << CS_CAN);
}

void spi_select_slave(SPI_Slave slave){
    spi_deselect_all();

    switch(slave){

        case SPI_SLAVE_DISPLAY:
            PORTB &= ~(1 << CS_DISPLAY);
            break;

        case SPI_SLAVE_IO: 
            PORTB &= ~(1 << CS_IO);
            break;
        
        case SPI_SLAVE_CAN:
            PORTB &= ~(1 << CS_CAN);
            break;

    }
}

void spi_write_nbytes(const uint8_t *data, int n){

    for(int i = 0; i < n; i ++){
        spi_write(data[i]);
    }
}

void spi_read_nbytes(uint8_t *data, int n){
    for(int i = 0; i < n; i++){
        data[i] = spi_read();
    }
    

}

