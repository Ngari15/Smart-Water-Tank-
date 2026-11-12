#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>

typedef struct{
    bool source_empty;
    bool high_high;
    bool auto_mode;
    bool manual_start_pressed;
    bool manual_stop_pressed;
}gpio_inputs_t;

void gpio_init(void);

gpio_inputs_t gpio_read_inputs(void);

void gpio_pump_set(bool on);
void gpio_GREEN_LED_set(bool on);
void gpio_RED_LED_set(bool on);

#endif