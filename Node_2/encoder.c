#include "encoder.h"
#include "sam.h"
#include "time.h"
#include <stdio.h>
#include <stdint.h>

void encoder_init() {
    PIOC->PIO_WPMR = 0x50494F00; // Disables write protect mode register
    PIOC->PIO_PDR |= PIO_PC25; // Disable IO mode for PC25
    PIOC->PIO_PDR |= PIO_PC26; // Disable IO mode for PC26
    PIOC->PIO_ABSR |= PIO_PC25; //Enables peripheral function for PB13
    PIOC->PIO_ABSR |= PIO_PC26; //Enables peripheral function for PB13

    PMC->PMC_WPMR = 0x504D4300; // Disables write protect mode register
    PMC->PMC_PCER1 |= PMC_PCDR1_PID33; // Enables peripheral clock for channel 0 tc2

    
    TC2->TC_WPMR = 0x54494D00; // Disables write protect mode register
    TC2->TC_BMR = (TC_BMR_TC0XC0S_TCLK0) | (TC_BMR_QDEN) | (TC_BMR_POSEN); // set quadrature decoder with position
    TC2->TC_CHANNEL[0].TC_CMR |= TC_CMR_TCCLKS_XC0; // Set xc0 as clock
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; // Enables counter clock and starts it with a software trigger
}

void encoder_print_status(){
    int32_t dir_status;
    int32_t pos;

    dir_status = ((TC2->TC_QISR) & (TC_QISR_DIR)) >> 8;
    printf("Direction: %i \r\n", dir_status);
    pos = REG_TC2_CV0;
    printf("Position counter: %i \r\n", pos);
}

Encoder_Data encoder_get_data(){
    int32_t pos = REG_TC2_CV0;
    int32_t dir = ((TC2->TC_QISR) & (TC_QISR_DIR)) >> 8;
    Encoder_Data data = {.pos = pos, .dir = dir};
    return data;
}