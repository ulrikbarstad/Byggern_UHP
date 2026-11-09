#include <stdint.h>
#include "adc.h"
#include "ir.h"
#include "can.h"

#define IR_BROKEN_THRESHOLD   1000
#define IR_CLEAR_THRESHOLD    1500

int32_t ir_broken(){
    int32_t adc_value = adc_read();
    static int32_t locked = 0;
    static int32_t clear_counter = 0;

    if (!locked)
    {
        if (adc_value < IR_BROKEN_THRESHOLD)
        {
            locked = 1;
            clear_counter = 0;

            return 1;   // BALL LOST
        }
    }
    else
    {
        if (adc_value > IR_CLEAR_THRESHOLD)
        {
            clear_counter++;

            if (clear_counter >= 200)
            {
                locked = 0;
                clear_counter = 0;
            }
        }
        else
        {
            clear_counter = 0;
        }
    }

    return 0;
}

void ir_send_ball_lost(){
    int32_t broken = ir_broken();
    CanMsg message = {.id = 70, .length = 1, .byte = {broken}};
    can_tx(message);
}