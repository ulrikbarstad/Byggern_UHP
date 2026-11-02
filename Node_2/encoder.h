#ifndef ENCODER_H
#define ENCODER_H
#include <stdint.h>

typedef struct  {
    int32_t pos;
    int32_t dir;
} Encoder_Data;

void encoder_init();

void encoder_print_status();

Encoder_Data encoder_get_data();

#endif
