
#include "timer.h"
#include "adc.h"
#include "gpio.h"
#include "pump_controller.h"
#include "lcd.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

static uint16_t water_level_raw = 0;
static uint8_t water_level_percent = 0;

static void lcd_display_status(
    uint8_t water_level,
    bool pump_on,
    gpio_inputs_t inputs
)
{
    if (inputs.source_empty)
    {
        lcd_set_cursor(0, 0);
        lcd_print("SOURCE EMPTY!   ");

        lcd_set_cursor(1, 0);
        lcd_print("PUMP: OFF       ");
        return;
    }

    if (inputs.high_high)
    {
        lcd_set_cursor(0, 0);
        lcd_print("HIGH LEVEL!     ");

        lcd_set_cursor(1, 0);
        lcd_print("PUMP: OFF       ");
        return;
    }

    lcd_set_cursor(0, 0);
    lcd_print("LEVEL: ");
    lcd_print_number(water_level);
    lcd_print("%");

    if (water_level < 10U)
        lcd_print("       ");
    else if (water_level < 100U)
        lcd_print("      ");
    else
        lcd_print("     ");

    lcd_set_cursor(1, 0);

    if (inputs.auto_mode)
        lcd_print("AUTO  ");
    else
        lcd_print("MANU  ");

    if (pump_on)
        lcd_print("PUMP: ON        ");
    else
        lcd_print("PUMP: OFF       ");
}

int main(void)
{
    /* Initialize hardware */
    gpio_init();
    adc_init();
    timer_init();
    lcd_init();

    /* Initialize pump controller */
    pump_controller_init();

    sei();

    while (1)
    {
        if (timer_100ms_ticks())
        {
            /* Read water level */
            water_level_raw = adc_read(0);

            water_level_percent =
                (uint8_t)(((uint32_t)water_level_raw * 100U) / 1023U);

            /* Read digital inputs */
            gpio_inputs_t inputs = gpio_read_inputs();

            /* Update pump controller */
            bool pump_on = pump_controller_update(
                water_level_percent,
                inputs
            );

            /* Apply pump output */
            gpio_pump_set(pump_on);

            /* Update LEDs */
            gpio_GREEN_LED_set(pump_on);

            gpio_RED_LED_set(
                inputs.source_empty || inputs.high_high
            );

            /* Update LCD */
            lcd_display_status(
                water_level_percent,
                pump_on,
                inputs
            );

        }
    }
}
