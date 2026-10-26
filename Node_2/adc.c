#include "adc.h"
#include "sam.h"


void adc_init(){

    ADC->ADC_WPMR = 0x41444300; //Disable write protect
    
    PMC->PMC_WPMR = 0x504D4300; // Disables write protect mode register for pmc
    PMC->PMC_PCER1 = (1 << 5); // Enables peripheral clock for ADC
    

    ADC->ADC_MR &= ~(ADC_MR_TRGEN); //Hardware trigger disabled. Can only start a conversion from software
    ADC->ADC_MR &= ~(ADC_MR_LOWRES); //12 BIT RES
    ADC->ADC_MR &= ~(ADC_MR_SLEEP); //NORMAL SLEEPMODE
    ADC->ADC_MR &= ~ADC_MR_FREERUN; //WAIT FOR TRIGGER

    ADC->ADC_MR &= ~ADC_MR_PRESCAL_Msk; //set all prescaler bits to 0
    ADC->ADC_MR |= (20 << ADC_MR_PRESCAL_Pos) & ADC_MR_PRESCAL_Msk; // set prescaler to 20 so that adcclock = mck /((20 + 1)*2)

    ADC->ADC_MR &= ~ADC_MR_STARTUP_Msk; //set all startup bits to 0
    ADC->ADC_MR |= (8 << ADC_MR_STARTUP_Pos) & ADC_MR_STARTUP_Msk; // SET STARTUP TIME TO 512 PERIODS OF ADCCLOCK

    ADC->ADC_MR |= (ADC_MR_SETTLING_Msk & 1<< ADC_MR_SETTLING_Pos); // SET SETTLING TIME TO 5 PERIODS OF ADCCLOCK
    ADC->ADC_MR &= ~(ADC_MR_ANACH); //NO ANALOG CHANGE
    ADC->ADC_MR &= ~(ADC_MR_TRACKTIM_Msk); // set to 0. Tracking time is 1 * ADCCLOCK
    ADC->ADC_MR &= ~(ADC_MR_TRANSFER_Msk); // SET TO 0. TRANSFER TIME IS 3 * ADCCLOCK
    ADC->ADC_MR &= ~(ADC_MR_USEQ); // SET USE SEQUENCE IN NORMAL MODE

    ADC->ADC_CHER = 1 << 7; // ENABLE CHANNEL 7 which is linked to arduino pin a0
}

int32_t adc_read(){
    ADC->ADC_CR = ADC_CR_START; // START CONVERSION FROM A TO D
    while(!(ADC->ADC_ISR & ADC_ISR_EOC7)){ //wait until conversion is done

    }
    return ADC->ADC_CDR[7]; //return converted value
}