#ifndef CAN_H
#define CAN_H

#include <stdint.h>
#include <stdbool.h>




typedef struct {
    uint16_t id;
    uint8_t length;
    uint8_t data[8];
} CAN_Message;

void can_send(const CAN_Message *message);
bool can_receive(CAN_Message *message);


#endif