#ifndef PUMP_CONTROLLER_H
#define PUMP_CONTROLLER_H

#include<stdbool.h>
#include<stdint.h>

#include "gpio.h"

#define PUMP_START_LEVEL_PERCENT 30U
#define PUMP_STOP_LEVEL_PERCENT  90U

void pump_controller_init(void);

bool pump_controller_update(
    uint8_t water_level_percent,
    gpio_inputs_t inputs
);

#endif