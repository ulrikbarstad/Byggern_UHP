#include <stdio.h>
#include <stdint.h>
#include "joystick.h"
#include "can.h"



Joystick_Position joystick_read_position(){
    CanMsg rx;
    Joystick_Position pos = {.x_pos = 165, .y_pos = 0, .r6_pressed = 0};
    if (can_rx(&rx)) {
        pos.x_pos = rx.byte[0];
        pos.y_pos = rx.byte[1];
        pos.r6_pressed = rx.byte[2];
        }
    
    return pos; 
}

void joystick_print_position(){
    CanMsg rx;

    if (can_rx(&rx)) {

            printf("Received message:\r\n");
            printf("ID:     0x%03X\r\n", rx.id);
            printf("Length: %u\r\n", rx.length);
            printf("Data: ");

            for (uint8_t i = 0; i < rx.length; i++) {
                printf("0x%02X ", rx.byte[i]);
            }

                printf("\r\n\r\n");
        }
        else {
            printf("No CAN message received\r\n\r\n");
        }
}

//Scales joystick x position from calibration values to -100 to 100. Deadzone of 
void joystick_scale_position(int32_t* pos, Joystick_Calibration calibration){
    //set x_pos
    if (*pos <= calibration.neutral + calibration.neutral_buffer && *pos >= calibration.neutral - calibration.neutral_buffer){ //if pos is in neutral position +- buffer Then y is 0
        *pos = 0;
    }

    // y != neutral position, check if y is positive. Then we get the difference between y and neutral position and scales it so that it is between 0 and 100 
    else if(*pos > calibration.neutral){ 
        *pos =
            ((*pos - calibration.neutral) * 100) /
            (calibration.max_pos - calibration.neutral);
    }
    else{ // same as last else if but now we do it so that y is negative. 
        *pos =
            -((calibration.neutral - *pos) * 100) /
            (calibration.neutral - calibration.min_pos);
    }
}

