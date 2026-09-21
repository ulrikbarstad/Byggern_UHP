#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "position.h"

typedef void (*adc_read_function)(uint8_t *, uint8_t *);

void adc_init(void);
void adc_read_joystick(uint8_t *x, uint8_t *y);
void adc_read_touchpad(uint8_t *x_touch, uint8_t *y_touch);
Position adc_joystick_position(Position_Calibration calibration);
Position_Direction adc_joystick_direction(Position_Calibration calibration, uint8_t neutral_limit);
Position adc_touchpad_position(Position_Calibration calibration);
Position_Calibration adc_calibrate(adc_read_function read_function);
#endif