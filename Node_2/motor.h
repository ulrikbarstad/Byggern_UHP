#ifndef MOTOR_H
#define MOTOR_H
#include <stdint.h>
#include "joystick.h"

void motor_init();

void motor_update_pulse_width(int32_t pulse_width);
void motor_set_direction(int32_t dir);
int32_t motor_get_increment(int32_t x_pos, int32_t max_increment, Joystick_Calibration calibration);
void motor_update_ref(int32_t *ref, int32_t x_pos, int32_t max_increment, Joystick_Calibration calibration);
void motor_PI_control(int32_t pos, int32_t ref, int32_t *integral_value, _Float32 k_p, _Float32 k_i);
void motor_control_by_joystick(int32_t x_pos);
#endif
