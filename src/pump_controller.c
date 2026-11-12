#include "pump_controller.h"

#include <stdbool.h>
#include <stdint.h>

#define BUTTON_DEBOUNCE_SAMPLE 2U

static bool pump_on = false;
static bool manual_run_latch = false;

static bool previous_auto_mode = false;
static bool mode_initialized = false;

typedef struct{
    bool stable_state;
    bool last_sample;
    uint8_t sample_count;
}button_debounce_t;

static button_debounce_t start_button = {
    false, false , BUTTON_DEBOUNCE_SAMPLE
};

static button_debounce_t stop_button = {
    false, false , BUTTON_DEBOUNCE_SAMPLE
};

    /* Detect button press events */
    static bool previous_start_pressed = false;
    static bool previous_stop_pressed = false;

static bool button_debounce_update(
    button_debounce_t *button,
    bool sample
){

    if(sample == button->last_sample)
    {
        if(button->sample_count < BUTTON_DEBOUNCE_SAMPLE)
        {
            button ->sample_count++;
        }
    }
    else
    {
        button-> last_sample = sample;
        button-> sample_count = 1U;
    }

    if(button->sample_count >= BUTTON_DEBOUNCE_SAMPLE)
    {
        button-> stable_state = button->last_sample;
    }
    return button->stable_state;
}

void pump_controller_init(void)
{
    pump_on = false;
    manual_run_latch = false;

    previous_auto_mode = false;
    mode_initialized = false;

    start_button.stable_state = false;
    start_button.last_sample = false;
    start_button.sample_count = BUTTON_DEBOUNCE_SAMPLE;

    stop_button.stable_state = false;
    stop_button.last_sample = false;
    stop_button.sample_count = BUTTON_DEBOUNCE_SAMPLE;

    previous_start_pressed = false;
    previous_stop_pressed = false;
}

bool pump_controller_update(
    uint8_t water_level_percent,
    gpio_inputs_t inputs
)
{
    /* Update button debounce */
    bool start_pressed =
        button_debounce_update(
            &start_button,
            inputs.manual_start_pressed
        );

    bool stop_pressed =
        button_debounce_update(
            &stop_button,
            inputs.manual_stop_pressed
        );


    bool start_event =
        start_pressed && !previous_start_pressed;

    bool stop_event =
        stop_pressed && !previous_stop_pressed;

    previous_start_pressed = start_pressed;
    previous_stop_pressed = stop_pressed;

    /* Detect operating mode changes */
    bool mode_changed = false;

    if (!mode_initialized)
    {
        previous_auto_mode = inputs.auto_mode;
        mode_initialized = true;
        mode_changed = true;
    }
    else if (inputs.auto_mode != previous_auto_mode)
    {
        previous_auto_mode = inputs.auto_mode;
        mode_changed = true;
    }

    /* Safety interlocks have highest priority */
    if (inputs.source_empty || inputs.high_high)
    {
        pump_on = false;
        manual_run_latch = false;

        return false;
    }

    /* Reset pump when operating mode changes */
    if (mode_changed)
    {
        pump_on = false;
        manual_run_latch = false;

        return false;
    }

    /* Automatic operating mode */
    if (inputs.auto_mode)
    {
        if (water_level_percent <= PUMP_START_LEVEL_PERCENT)
        {
            pump_on = true;
        }
        else if (water_level_percent >= PUMP_STOP_LEVEL_PERCENT)
        {
            pump_on = false;
        }
    }

    /* Manual operating mode */
 else
    {
        if (mode_changed)
        {
            return false;
        }

        /* STOP has priority over START */
        if (stop_event)
        {
            manual_run_latch = false;
        }
        else if (start_event)
        {
            manual_run_latch = true;
        }

        pump_on = manual_run_latch;
    }

    return pump_on;
}