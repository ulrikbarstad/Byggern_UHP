#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void adc_init(void);
void adc_read_joystick(uint8_t *x, uint8_t *y);
void adc_read_touchpad(uint8_t *x_touch, uint8_t *y_touch);

#endif