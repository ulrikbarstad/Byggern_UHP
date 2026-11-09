#ifndef SOLENOID_H
#define SOLENOID_H
#include <stdint.h>


void solenoid_init();

void solenoid_activate();

void solenoid_deactivate();

void solenoid_start_timer();

void solenoid_pulse(int32_t *active, int32_t r6_pressed);

#endif