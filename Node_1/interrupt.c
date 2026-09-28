#include <avr/io.h>
#include <avr/interrupt.h>
#include "interrupt.h"

volatile uint8_t joystick_tick = 0;
volatile uint8_t button_click = 0;

void joystick_timer_init(void){
    // Timer0 CTC mode
    TCCR0 = (1 << WGM01)
          | (1 << CS02)
          | (1 << CS00);

    // 20 ms ved 4.9152 MHz og prescaler 1024
    OCR0 = 95;

    // Enable Output Compare Match interrupt
    TIMSK |= (1 << OCIE0);
}
ISR(TIMER0_COMP_vect){
    joystick_tick = 1;
}

void joystick_button_init(void){
    DDRD &= ~(1 << PD2);   
    PORTD |= (1 << PD2); 

    MCUCR &= ~(1 << ISC00);
    MCUCR |=  (1 << ISC01);

    GIFR = (1 << INTF0);

    GICR |= (1 << INT0);

    sei();
}
ISR(INT0_vect){
    button_click = 1;
}