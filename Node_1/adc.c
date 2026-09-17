#include <avr/io.h>
#include <stdint.h>

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
    


    *x_touch = *ADC;

    *y_touch = *ADC;




}






    

