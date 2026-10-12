#include <stdio.h>
#include "joystick.h"
#include "adc.h"
#include "can.h"

void joystick_send_position(Position_Calibration joystickCalibration){
    Position position = adc_joystick_position(joystickCalibration);
    CAN_Message positionMsg = {.id = 0x69, .length = 2, .data = {position.x_pos, position.y_pos}};
    can_send(&positionMsg);
    printf("Joystick Position sent \r\n-----------\r\nx pos: %u\r\ny pos: %u\r\n\r\n", position.x_pos, position.y_pos);
}