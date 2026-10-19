#ifndef SERVO_H
#define SERVO_H
#include "joystick.h"
void servo_init();
int32_t servo_pulse_width_guard(int32_t pulse_width);
void servo_update_pulse_width(int32_t pulse_width);
int32_t servo_get_increment(int32_t max_increment, Joystick_X_Calibration calibration);
void servo_move(int32_t* current_pulse_width, int32_t max_increment, Joystick_X_Calibration calibration);
#endif