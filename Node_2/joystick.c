#include <stdio.h>
#include <stdint.h>
#include "joystick.h"
#include "can.h"



int32_t joystick_read_x_position(){
    CanMsg rx;
    if (can_rx(&rx)) {
        return rx.byte[0];
        }
    
    return 165; 
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
void joystick_scale_x_position(int32_t* x_pos, Joystick_X_Calibration calibration){
    //set x_pos
    if (*x_pos <= calibration.neutral_x + calibration.neutral_buffer && *x_pos >= calibration.neutral_x - calibration.neutral_buffer){ //if x_pos is in neutral position +- buffer Then x is 0
        *x_pos = 0;
    }

    // x != neutral position, check if x is positive. Then we get the difference between x and neutral position and scales it so that it is between 0 and 100 
    else if(*x_pos > calibration.neutral_x){ 
        *x_pos =
            ((*x_pos - calibration.neutral_x) * 100) /
            (calibration.max_x_pos - calibration.neutral_x);
    }
    else{ // same as last else if but now we do it so that x is negative. 
        *x_pos =
            -((calibration.neutral_x - *x_pos) * 100) /
            (calibration.neutral_x - calibration.min_x_pos);
    }
}
