#define F_CPU 4915200UL

#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>


#include "adc.h"

#define ADC_BASE 0x1000

static volatile uint8_t * const ADC = (volatile uint8_t *)ADC_BASE;




void adc_init() {
    DDRD |= (1 << PD5); // PD5 satt til output
    TCCR1A = (1 << COM1A0); // Sets OC1A to CTC square output with toggle.  
    TCCR1B = (1 << WGM12) | (1 << CS10);; // set waveform, N = 1 - No prescaling
    
    OCR1A = 0; // toggle at f_osc/2*N (1 + OCR1A)

    DDRE &= ~(1 << PE0);
    
}

void adc_read_joystick(uint8_t *x, uint8_t *y){
      
    *ADC = 0; //Write til 0x1000 ,XMEM lager CS og WR automatisk,  WR-pulsen starter ADC-konverteringen.

    while (PINE & (1 << PE0)){ // busy går LOW, ved start konvertering
        ;
    }

    while (!(PINE & (1 << PE0))){ //busy går HIGH ved ferdig konvertering
        ;
    }
    

    *y = *ADC; // AIN0 ved første RD puls

    *x = *ADC; //AIN1 ved andre RD puls
}


void adc_read_touchpad(uint8_t *x_touch, uint8_t *y_touch){
    *ADC = 0; //Write til 0x1000 ,XMEM lager CS og WR automatisk,  WR-pulsen starter ADC-konverteringen.

    while (PINE & (1 << PE0)){ // busy går LOW, ved start konvertering
        ;
    }

    while (!(PINE & (1 << PE0))){ //busy går HIGH ved ferdig konvertering
        ;
    }


    (void)*ADC;   // skip AIN0
    (void)*ADC;   // skip AIN1
    

    *y_touch = *ADC;

    *x_touch = *ADC;
}


Position adc_joystick_position(Position_Calibration calibration){ // takes in calibration struct, reads position and returns a position in x and y direction from -100 to 100.
    Position pos;
    uint8_t x_pos_joystick;
    uint8_t y_pos_joystick;
    adc_read_joystick(&x_pos_joystick, &y_pos_joystick);

    //set x_pos
    if (x_pos_joystick <= calibration.neutral_x + 1 && x_pos_joystick >= calibration.neutral_x - 1){
        pos.x_pos = 0;
    }

    else if(x_pos_joystick > calibration.neutral_x){
        pos.x_pos =
            ((x_pos_joystick - calibration.neutral_x) * 100) /
            (calibration.max_x_pos - calibration.neutral_x);
    }
    else{
         pos.x_pos =
            -((calibration.neutral_x - x_pos_joystick) * 100) /
            (calibration.neutral_x - calibration.min_x_pos);
    }

    //set y_pos
    if (y_pos_joystick <= calibration.neutral_y + 1 && y_pos_joystick >= calibration.neutral_y - 1){
        pos.y_pos = 0;
    }

    else if(y_pos_joystick > calibration.neutral_y){
        pos.y_pos =
            ((y_pos_joystick - calibration.neutral_y) * 100) /
            (calibration.max_y_pos - calibration.neutral_y);
    }
    
    else{
        pos.y_pos =
            -((calibration.neutral_y - y_pos_joystick) * 100) /
            (calibration.neutral_y - calibration.min_y_pos);
    }
    return pos;
}

Position_Direction adc_joystick_direction(Position_Calibration calibration, uint8_t neutral_limit){
    Position_Direction dir;
    Position pos = adc_joystick_position(calibration);

    if (pos.x_pos <= 0 + neutral_limit && 
        pos.x_pos >= 0 - neutral_limit && 
        pos.y_pos <= 0 + neutral_limit &&
        pos.y_pos >= 0 - neutral_limit){
        dir = NEUTRAL;
    }

    else if(pos.y_pos > 0){
        if (pos.y_pos > abs(pos.x_pos)){
            dir = UP;
        }
        else if(pos.x_pos < 0){
            dir = LEFT;
        }
        else{
            dir = RIGHT;
        }
    }

    else{
        if(abs(pos.y_pos) > abs(pos.x_pos)){
            dir = DOWN;
        }
         else if(pos.x_pos < 0){
            dir = LEFT;
        }
        else{
            dir = RIGHT;
        }
    }
        
    return dir;
}

Position adc_touchpad_position(Position_Calibration calibration){
    Position pos;
    uint8_t x_pos_touchpad;
    uint8_t y_pos_touchpad;
    adc_read_touchpad(&x_pos_touchpad, &y_pos_touchpad);


    pos.x_pos =
        ((int16_t)(x_pos_touchpad - calibration.min_x_pos) * 200) /
        (calibration.max_x_pos - calibration.min_x_pos)
        - 100;

    pos.y_pos =
        ((int16_t)(y_pos_touchpad - calibration.min_y_pos) * 200) /
        (calibration.max_y_pos - calibration.min_y_pos)
        - 100;

    return pos;
}



Position_Calibration adc_calibrate(adc_read_function read_function){
    
    uint8_t x;
    uint8_t y;
    read_function(&x, &y);
    
    Position_Calibration cal;
    cal.neutral_x = x;
    cal.neutral_y = y;
    
    int i = 0;
    uint8_t max_x = x;
    uint8_t min_x = x;
    uint8_t max_y = y;
    uint8_t min_y = y;
    
   for(int i = 0; i < 5000; i++){
        read_function(&x, &y);
        if (max_x < x){
            max_x = x;
        }

        else if(min_x > x){
            min_x = x;
        }

        if (max_y < y){
            max_y = y;
        }

        else if(min_y > y){
            min_y = y;
        }
        _delay_ms(1);
    }
    cal.max_x_pos = max_x;
    cal.min_x_pos = min_x;
    cal.max_y_pos = max_y;
    cal.min_y_pos = min_y;
    return cal;
}



/*
    Position_Calibration cal_joy = adc_calibrate(adc_read_joystick);
    printf("Joystick Calibration Done\r\n------------\r\n");
    Position_Calibration cal_touch = adc_calibrate(adc_read_touchpad);
    printf("Joystick Calibration Done\r\n------------\r\n");
    printf("Joystick Calibration: \r\nmax x: %u \r\nmin x: %u \r\nmax y: %u \r\nmin y: %u \r\nneutral x: %u \r\nneutral y: %u \r\n------------\r\nToucpad Calibration: \r\nmax x: %u \r\nmin x: %u \r\nmax y: %u \r\nmin y: %u \r\nneutral x: %u \r\nneutral y: %u\r\n------------\r\n\r\n", cal_joy.max_x_pos, cal_joy.min_x_pos, cal_joy.max_y_pos, cal_joy.min_y_pos, cal_joy.neutral_x, cal_joy.neutral_y, cal_touch.max_x_pos, cal_touch.min_x_pos, cal_touch.max_y_pos, cal_touch.min_y_pos, cal_touch.neutral_x, cal_touch.neutral_y);
*/
    

