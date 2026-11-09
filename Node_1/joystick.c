#include <stdio.h>
#include "joystick.h"
#include "adc.h"
#include "can.h"

void joystick_send_position(int8_t r6_pressed){
    uint8_t x_pos;
    uint8_t y_pos;
    adc_read_joystick(&x_pos, &y_pos);
    CAN_Message positionMsg = {.id = 0x69, .length = 3, .data = {x_pos, y_pos, r6_pressed}};
    can_send(&positionMsg);
    //printf("Joystick Position sent \r\n-----------\r\nx pos: %u\r\ny pos: %u\r\n\r\n", x_pos, y_pos);
}