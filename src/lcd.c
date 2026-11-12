#include "lcd.h"
#include "config.h"

#include <avr/io.h>

#include <stdint.h>

/* Ensure delay calculations use the correct CPU frequency. */
#ifndef F_CPU
#define F_CPU SYSTEM_CLOCK
#endif

#include <util/delay.h>

#define LCD_DATA_MASK ( \
    (1U << PIN_LCD_D4) | \
    (1U << PIN_LCD_D5) | \
    (1U << PIN_LCD_D6) | \
    (1U << PIN_LCD_D7) )

static void lcd_enable_pulse(void)
{
    PORTB |= (1U << PIN_LCD_ENABLE);
    _delay_us(1);

    PORTB &= ~(1U << PIN_LCD_ENABLE);
    _delay_us(50);
}

static void lcd_send_nibble(uint8_t nibble)
{
    /* Clear the four LCD data output bits */
    PORTC &= ~LCD_DATA_MASK;

    /* Map nibble bits to PC1, PC2, PC3 and PC4 */
    if (nibble & 0x01U)
        PORTC |= (1U << PIN_LCD_D4);

    if (nibble & 0x02U)
        PORTC |= (1U << PIN_LCD_D5);

    if (nibble & 0x04U)
        PORTC |= (1U << PIN_LCD_D6);

    if (nibble & 0x08U)
        PORTC |= (1U << PIN_LCD_D7);

    lcd_enable_pulse();
}

static void lcd_send_byte(uint8_t value, uint8_t is_data)
{
    if (is_data)
    {
        PORTC |= (1U << PIN_LCD_RS);
    }
    else
    {
        PORTC &= ~(1U << PIN_LCD_RS);
    }

    /* Send upper four bits first */
    lcd_send_nibble(value >> 4);

    /* Send lower four bits */
    lcd_send_nibble(value & 0x0FU);
}

static void lcd_command(uint8_t command)
{
    lcd_send_byte(command, 0U);

    if (command == 0x01U || command == 0x02U)
    {
        _delay_ms(2);
    }
}

static void lcd_data(uint8_t data)
{
    lcd_send_byte(data, 1U);
}

void lcd_init(void)
{
    /* Configure LCD pins as outputs */
    DDRB |= (1U << PIN_LCD_ENABLE);

    DDRC |= LCD_DATA_MASK |
            (1U << PIN_LCD_RS);

    /* Start with LCD control and data signals LOW */
    PORTB &= ~(1U << PIN_LCD_ENABLE);

    PORTC &= ~(LCD_DATA_MASK |
               (1U << PIN_LCD_RS));

    _delay_ms(20);

    /* Initialize LCD into 4-bit communication mode */
    lcd_send_nibble(0x03U);
    _delay_ms(5);

    lcd_send_nibble(0x03U);
    _delay_us(150);

    lcd_send_nibble(0x03U);
    _delay_us(150);

    lcd_send_nibble(0x02U);

    lcd_command(0x28U);  /* 4-bit, 2 lines, 5x8 font */
    lcd_command(0x0CU);  /* Display ON, cursor OFF */
    lcd_command(0x06U);  /* Increment cursor */
    lcd_command(0x01U);  /* Clear display */
}

void lcd_clear(void)
{
    lcd_command(0x01U);
}

void lcd_set_cursor(uint8_t row, uint8_t column)
{
    uint8_t address;

    if (row == 0U)
    {
        address = 0x00U + column;
    }
    else
    {
        address = 0x40U + column;
    }

    lcd_command(0x80U | address);
}

void lcd_print(const char *text)
{
    while (*text != '\0')
    {
        lcd_data((uint8_t)*text);
        text++;
    }
}

/* Display an unsigned integer */
void lcd_print_number(uint16_t number)
{
    char buffer[5];
    uint8_t index = 0U;

    if (number == 0U)
    {
        lcd_data('0');
        return;
    }

    while (number > 0U)
    {
        buffer[index] = (char)('0' + (number % 10U));
        number /= 10U;
        index++;
    }

    while (index > 0U)
    {
        index--;
        lcd_data((uint8_t)buffer[index]);
    }
}