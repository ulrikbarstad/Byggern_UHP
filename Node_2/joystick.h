#ifndef JOYSTICK_H
#define JOYSTICK_H


typedef struct  {
    int32_t x_pos;
    int32_t y_pos;
    int32_t r6_pressed;
} Joystick_Position;

typedef struct {
    int32_t max_pos;
    int32_t min_pos;
    
    int32_t neutral;
    int32_t neutral_buffer;
} Joystick_Calibration;

Joystick_Position joystick_read_position();
void joystick_print_position();
void joystick_scale_position(int32_t* pos, Joystick_Calibration calibration);



#endif