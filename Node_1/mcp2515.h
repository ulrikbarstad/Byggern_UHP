#include <stdbool.h>
#ifndef MCP2515_H
#define MCP2515_H



#define MCP2515_CMD_READ 0x03
#define MCP2515_CMD_RESET 0b11000000
#define MCP2515_CMD_WRITE 0b00000010
#define MCP2515_CMD_READ_STATUS 0b10100000
#define MCP2515_CMD_BIT_MODIFY 0b00000101

#define MCP2515_CANCTRL 0x0F
#define MCP2515_MODE_MASK 0b11100000
#define MCP2515_MODE_LOOPBACK 0b01000000
#define MCP2515_MODE_NORMAL 0b00000000

#define MCP_2515_CNF1 0x2A
#define MCP_2515_BRP_MASK 0b00111111
#define MCP_2515_BRP_VAL 0
#define MCP_2515_SJW_MASK 0b11000000
#define MCP_2515_SJW_VAL 0

#define MCP_2515_CNF2 0x29
#define MCP_2515_BTLMODE_MASK 0b10000000
#define MCP_2515_SAM_MASK 0b01000000
#define MCP_2515_PHSEG1_MASK 0b00111000
#define MCP_2515_PRSEG_MASK 0b00000111
#define MCP_2515_BTLMODE_VAL 0b10000000
#define MCP_2515_SAM_VAL 0
#define MCP_2515_PHSEG1_VAL 0b00110000
#define MCP_2515_PRSEG_VAL 1

#define MCP_2515_CNF3 0x28
#define MCP_2515_PHSEG2_MASK 0b00000111
#define MCP_2515_PHSEG2_VAL 0b101

#define MCP_2515_CANINTF 0x2C
#define MCP_2515_CANSTAT 0xE

#define MCP2515_TXB0SIDH  0x31
#define MCP2515_TXB0SIDL  0x32
#define MCP2515_TXB0EID8  0x33
#define MCP2515_TXB0EID0  0x34
#define MCP2515_TXB0DLC   0x35
#define MCP2515_TXB0D0    0x36

#define MCP2515_RTS_TXB0  0x01

#define MCP2515_RXB0SIDH  0x61
#define MCP2515_RXB0SIDL  0x62
#define MCP2515_RXB0DLC   0x65
#define MCP2515_RXB0D0    0x66

#define MCP2515_RX0IF     0x01
#define MCP2515_STATUS_RX0IF 0x01



uint8_t mcp2515_read(uint8_t adress);
void mcp2515_reset();
void mcp2515_write(uint8_t adress, const uint8_t *data, int n);
void mcp2515_rts(const uint8_t buffer_mask);
uint8_t mcp2515_read_status();
void mcp2515_bit_modify(uint8_t adress_byte, uint8_t mask_byte, uint8_t data_byte);
void mcp2515_init(bool loopback);

void mcp2515_test(void);

#endif