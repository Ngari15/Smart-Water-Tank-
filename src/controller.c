#include "controller.h"
#include "config.h"

controller_output_t controller_update(
    controller_state_t current_state,
    uint8_t water_level_percent,
    bool source_empty,
    bool high_high
)
{
    controller_output_t output;

    output.state = current_state;
    output.pump_on = false;
    output.green_led_on = false;
    output.red_led_on = false;

    switch (current_state)
    {
    case CONTROLLER_IDLE:
        
        if(source_empty||high_high){
            output.state = CONTROLLER_FAULT;
        }else if(water_level_percent <= START_LEVEL){
            output.state = CONTROLLER_FILLING;
        }
        break;

    case CONTROLLER_FILLING:

        if(source_empty||high_high){
        output.state = CONTROLLER_FAULT;
        
        }else if(water_level_percent >= STOP_LEVEL){
            output.state = CONTROLLER_IDLE;
           
        }
        break;

    case CONTROLLER_FAULT:
        output.pump_on = false;
        if(!source_empty && !high_high)
            {
                output.state = CONTROLLER_IDLE;
            }
        break;
    
    default:
        output.state = CONTROLLER_FAULT;
        output.pump_on = false;
        break;
    }
    output.pump_on = (output.state == CONTROLLER_FILLING);

    output.green_led_on = output.pump_on;

    output.red_led_on = source_empty || high_high ||
                        (water_level_percent>= STOP_LEVEL) ||
                        (output.state == CONTROLLER_FAULT);
    
    return output;
}