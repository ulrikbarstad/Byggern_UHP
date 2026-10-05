#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <avr/interrupt.h>
extern volatile uint8_t joystick_tick;
extern volatile uint8_t joystick_pressed;

void joystick_timer_init(void);
void joystick_button_init(void);
ISR(TIMER0_COMP_vect);
ISR(INT0_vect);

#endif