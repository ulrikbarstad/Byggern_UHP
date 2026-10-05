#define F_CPU 4915200UL

#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>
#include "mcp2515.h"
#include "spi.h"



uint8_t mcp2515_read(uint8_t adress){
    spi_select_slave(SPI_SLAVE_CAN);
    spi_write(MCP2515_CMD_READ); //Sender read commando
    spi_write(adress);
    uint8_t data = spi_read();
    spi_deselect_all();
    return data;
}

void mcp2515_reset(){
    spi_select_slave(SPI_SLAVE_CAN);
    spi_write(MCP2515_CMD_RESET);
    spi_deselect_all();
}

void mcp2515_write(uint8_t adress, const uint8_t *data, int n){
    spi_select_slave(SPI_SLAVE_CAN);
    spi_write(MCP2515_CMD_WRITE);
    spi_write(adress);
    spi_write_nbytes(data, n);
    spi_deselect_all();
}

void mcp2515_rts(const uint8_t buffer_mask){
    spi_select_slave(SPI_SLAVE_CAN);
    uint8_t rts_code = 0b10000000 | buffer_mask; //put 3 lsb from transmit buffer in rts code
    spi_write(rts_code);
    spi_deselect_all();
}

uint8_t mcp2515_read_status(){
    spi_select_slave(SPI_SLAVE_CAN);
    spi_write(MCP2515_CMD_READ_STATUS);
    uint8_t data = spi_read();
    spi_deselect_all();
    return data;
}

void mcp2515_bit_modify(uint8_t adress_byte, uint8_t mask_byte, uint8_t data_byte){
    spi_select_slave(SPI_SLAVE_CAN);
    spi_write(MCP2515_CMD_BIT_MODIFY);
    spi_write(adress_byte);
    spi_write(mask_byte);
    spi_write(data_byte);
    spi_deselect_all();
}

void mcp2515_init(bool loopback){
    _delay_ms(10); //gi oscillator tid for å starte opp
    mcp2515_reset(); //Når mcp2515 blir resatt går den automatisk i configuration mode
    _delay_ms(10); 
    uint8_t status = mcp2515_read(MCP_2515_CANSTAT);

    if ((status & MCP2515_MODE_MASK) != 0x80) {
        // MCP2515 did not enter configuration mode
        printf("CP2515 did not enter configuration mode");

    }

    mcp2515_bit_modify(MCP_2515_CNF1, 
                        MCP_2515_BRP_MASK, 
                        MCP_2515_BRP_VAL); // Set BRP bits to 0 so that TQ = 125 ns 16MHz oscillator

    mcp2515_bit_modify(MCP_2515_CNF1, 
                        MCP_2515_SJW_MASK, 
                        MCP_2515_SJW_VAL); //Set SJW bits to 0 so that length is 1 x Tq

    mcp2515_bit_modify(MCP_2515_CNF2, 
                        MCP_2515_BTLMODE_MASK, 
                        MCP_2515_BTLMODE_VAL); //Set BLTMODE to 1

    mcp2515_bit_modify(MCP_2515_CNF2, 
                        MCP_2515_SAM_MASK, 
                        MCP_2515_SAM_VAL); //Set SAM to 0
    
    mcp2515_bit_modify(MCP_2515_CNF2, 
                        MCP_2515_PHSEG1_MASK, 
                        MCP_2515_PHSEG1_VAL); //Set phseg1 to 110

    mcp2515_bit_modify(MCP_2515_CNF2, 
                        MCP_2515_PRSEG_MASK, 
                        MCP_2515_PRSEG_VAL); //Set prseg to 001

    mcp2515_bit_modify(MCP_2515_CNF3, 
                        MCP_2515_PHSEG2_MASK, 
                        MCP_2515_PHSEG2_VAL); //Set prseg to 101

    mcp2515_bit_modify(MCP_2515_CANINTF, 0b00000001, 0b00000000); // clear interupt RX0IF


    if(loopback){
        mcp2515_bit_modify(MCP2515_CANCTRL, 
                            MCP2515_MODE_MASK, 
                            MCP2515_MODE_LOOPBACK); //Set REQOP bits to 010 => Loopback mode
    }
    else{
        mcp2515_bit_modify(MCP2515_CANCTRL, 
                            MCP2515_MODE_MASK, 
                            MCP2515_MODE_NORMAL); // Set REQOP bits to 000 => Normal mode
    }

}

