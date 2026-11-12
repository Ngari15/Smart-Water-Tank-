#include "gpio.h"
#include "config.h"

#include <avr/io.h>
#include <stdint.h>

void gpio_init(void){

PORTD &= ~(1<<PUMP_RELAY);

PORTB &= ~(
(1<<RED_LED)|
(1<<GREEN_LED )|
(1<<PIN_LCD_ENABLE ));

DDRD |= (1<<PUMP_RELAY);

DDRB |= (
(1<<RED_LED)|
(1<<GREEN_LED )|
(1<<PIN_LCD_ENABLE )  
);

DDRD &= ~(
    (1<<PIN_SOURCE_EMPTY)|
    (1<<PIN_HIGH_HIGH )|
    (1<<PIN_MODE_SELECT )|
    (1<<PIN_MANUAL_START)|
    (1<<PIN_MANUAL_STOP)
);

PORTD |= (   
    (1<<PIN_SOURCE_EMPTY)|
    (1<<PIN_HIGH_HIGH )|
    (1<<PIN_MODE_SELECT )|
    (1<<PIN_MANUAL_START)|
    (1<<PIN_MANUAL_STOP));

}

gpio_inputs_t gpio_read_inputs(void){
    gpio_inputs_t inputs;
    const uint8_t port_state= PIND;

    inputs.source_empty = 
    (port_state & (1<<PIN_SOURCE_EMPTY)) == 0U;

    inputs.high_high = 
    (port_state & (1<<PIN_HIGH_HIGH)) == 0U;
       
    inputs.auto_mode = 
    (port_state & (1<<PIN_MODE_SELECT)) == 0U;

    inputs.manual_start_pressed  = 
    (port_state & (1<<PIN_MANUAL_START)) == 0U;

    inputs.manual_stop_pressed = 
    (port_state & (1<<PIN_MANUAL_STOP)) == 0U;

    return inputs;
}

void gpio_pump_set(bool on)
{
    if (on){
        PORTD |= (1<<PUMP_RELAY);
    }else{
        PORTD &= ~(1<<PUMP_RELAY);
    }
}

void gpio_GREEN_LED_set(bool on)
{
    if (on){
        PORTB |= (1<<GREEN_LED);
    }else{
        PORTB &= ~(1<<GREEN_LED);
    }
}

void gpio_RED_LED_set(bool on)
{
    if (on){
        PORTB |= (1<<RED_LED);
    }else{
        PORTB &= ~(1<<RED_LED);
    }
}