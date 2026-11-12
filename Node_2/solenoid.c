#include "solenoid.h"
#include "sam.h"
#include <stdint.h>

volatile int32_t timer_done = 0;

void solenoid_init(){
    //Arduino D10 = PA28
    PIOA->PIO_WPMR =  0x50494F00; //Write protect disable
    PIOA->PIO_PER |= PIO_PER_P28; //arduino D10 as output pin
    PIOA->PIO_OER |= PIO_PER_P28;

    PMC->PMC_WPMR = 0x504D4300; // Disables write protect mode register
    PMC->PMC_PCER0 = 1 << ID_TC0; // active clock for channel 0 TC0

    TC0->TC_WPMR = 0x54494D00; // Disables write protect mode register
    //We want waveform mode
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4 | //MCLK/128
                                TC_CMR_CPCSTOP | //Stopped when reaching RC 
                                TC_CMR_WAVE; //waveform mode

    TC0->TC_CHANNEL[0].TC_RC = 40000; //about 60 ms
    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS; //enable interrupt when TC0 Channel 0 == RC
    NVIC->ISER[0] = (1 << ID_TC0); //aktiver interrupt for tc0 i nvic
    

    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN; //Enables clock at channel 0

}

void solenoid_activate(){
    PIOA->PIO_CODR = PIO_PER_P28;
}

void solenoid_deactivate(){
    
    PIOA->PIO_SODR = PIO_PER_P28;
}

void solenoid_start_timer(){
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; //Software trigger and clock enable    
}

void TC0_Handler(){
    uint32_t status = TC0->TC_CHANNEL[0].TC_SR; //when reading interrupt status, the interrupt is cleared. ie. if flag is activated, it is now deactivated.

    if (status & TC_SR_CPCS) //interrupt because timer is RC
    {
        timer_done = 1;
    }
}

void solenoid_pulse(int32_t* active, int32_t r6_pressed){

    if(*active && timer_done){
        solenoid_deactivate();
        *active = 0;
        
    }

    else if(!(*active) && r6_pressed){
        solenoid_activate();
        solenoid_start_timer();
        timer_done = 0;
        *active = 1;
    }

}