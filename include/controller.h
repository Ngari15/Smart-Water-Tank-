#ifndef CONTROLLER_H
#define CONTROLLER_H

#include<stdint.h>
#include<stdbool.h>

typedef enum{
    CONTROLLER_IDLE =0,
    CONTROLLER_FILLING,
    CONTROLLER_FAULT
}controller_state_t;

typedef struct {
    controller_state_t state;
    bool pump_on;
    bool green_led_on;
    bool red_led_on;
} controller_output_t;

controller_output_t controller_update(
    controller_state_t current_state,
    uint8_t water_level_percent,
    bool source_empty,
    bool high_high
);

#endif