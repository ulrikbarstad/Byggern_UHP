#include <stdint.h>

#include "can.h"
#include "mcp2515.h"

void can_send(const CAN_Message *message)
{
    // Standard CAN ID er 11 bit: maks 0x7FF
    if (message->id > 0x7FF) {
        return;
    }

    // CAN data frame kan maks ha 8 databytes
    if (message->length > 8) {
        return;
    }

    uint8_t tx_buffer[13];

    // ID bit 10..3
    tx_buffer[0] = (uint8_t)(message->id >> 3);

    // ID bit 2..0 plasseres i bit 7..5.
    // EXIDE blir 0 -> standard CAN ID.
    tx_buffer[1] = (uint8_t)((message->id & 0x07) << 5);

    // Extended identifier brukes ikke
    tx_buffer[2] = 0x00; // EID8
    tx_buffer[3] = 0x00; // EID0

    // DLC = number of data bytes
    tx_buffer[4] = message->length;

    for (uint8_t i = 0; i < message->length; i++) {
        tx_buffer[5 + i] = message->data[i];
    }

    // Skriv hele TXB0 sekvensielt fra SIDH
    mcp2515_write(
        MCP2515_TXB0SIDH,
        tx_buffer,
        5 + message->length
    );

    // Be MCP2515 sende TXB0
    mcp2515_rts(MCP2515_RTS_TXB0);
}

bool can_receive(CAN_Message *message)
{
    // Sjekk om RXB0 faktisk inneholder en ny melding
    uint8_t status = mcp2515_read_status();

    if (!(status & MCP2515_STATUS_RX0IF)) {
        return false;
    }

    // Les standard CAN identifier
    uint8_t sidh = mcp2515_read(MCP2515_RXB0SIDH);
    uint8_t sidl = mcp2515_read(MCP2515_RXB0SIDL);

    message->id = ((uint16_t)sidh << 3)
                | (sidl >> 5);

    // Les lengden. Bare bit 3..0 er DLC.
    message->length =
        mcp2515_read(MCP2515_RXB0DLC) & 0x0F;

    // Sikkerhet: CAN kan maks inneholde 8 databytes
    if (message->length > 8) {
        message->length = 8;
    }

    // Les databytane
    for (uint8_t i = 0; i < message->length; i++) {
        message->data[i] =
            mcp2515_read(MCP2515_RXB0D0 + i);
    }

    // Vi er ferdige med RXB0, så clear RX0IF
    mcp2515_bit_modify(
        MCP_2515_CANINTF,
        MCP2515_RX0IF,
        0x00
    );

    return true;
}

