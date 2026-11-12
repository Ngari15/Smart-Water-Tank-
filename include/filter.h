#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

#define FILTER_WINDOW_SIZE 8U

typedef struct{
    uint16_t samples [FILTER_WINDOW_SIZE];
    uint32_t sum;
    uint8_t index;
}moving_average_t;

void filter_init(moving_average_t * filter,uint16_t initial_value);

uint16_t filter_update(moving_average_t *filter, uint16_t new_sample);

#endif