#include <avr/io.h>
#include "adc.h"




void adc_init() {
    DDRD |= (1 << PD5); // PD5 satt til output
    TCCR1A = 0b01000000; // Sets OC1A to CTC square output with toggle.  
    TCCR1B |= (1 << WGM12); // set waveform
    TCCR1B |= (1 << CS10); // N = 1 - No prescaling
    
    OCR1AH = 0; // toggle at f_osc/2*N (1 + OCR1A)
    OCR1AL = 0;
}