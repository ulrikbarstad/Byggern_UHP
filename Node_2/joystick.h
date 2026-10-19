#ifndef JOYSTICK_H
#define JOYSTICK_H


typedef struct  {
    int32_t x_pos;
    int32_t y_pos;
} Joystick_Position;

typedef struct {
    int32_t max_x_pos;
    int32_t min_x_pos;
    int32_t neutral_x;
    int32_t neutral_buffer;
} Joystick_X_Calibration;

int32_t joystick_read_x_position();
void joystick_print_position();
void joystick_scale_x_position(int32_t* x_pos, Joystick_X_Calibration calibration);

#endif