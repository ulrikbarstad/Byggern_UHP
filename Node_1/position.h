#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>

typedef struct  {
    int8_t x_pos;
    int8_t y_pos;
} Position;

typedef struct {
    uint8_t max_x_pos;
    uint8_t min_x_pos;
    uint8_t max_y_pos;
    uint8_t min_y_pos;
    uint8_t neutral_x;
    uint8_t neutral_y;
} Position_Calibration;

typedef enum {
    LEFT, 
    RIGHT, 
    UP, 
    DOWN, 
    NEUTRAL
} Position_Direction;

#endif