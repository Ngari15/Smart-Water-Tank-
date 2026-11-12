#include "terminal.h"

#include <avr/io.h>
#include <stdint.h>

#define BAUD_RATE 9600UL
#define UBRR_VALUE ((F_CPU / (16UL * BAUD_RATE)) - 1UL)

void uart_init(void)
{
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;

    UCSR0B = (1U << TXEN0);

    UCSR0C = (1U << UCSZ01) | (1U << UCSZ00);
}

void uart_transmit(char data)
{
    while (!(UCSR0A & (1U << UDRE0)))
    {
    }

    UDR0 = data;
}

void uart_print_number(uint16_t number)
{
    char buffer[6];
    uint8_t i = 0U;

    if (number == 0U)
    {
        uart_transmit('0');
        return;
    }

    while (number > 0U)
    {
        buffer[i++] = (char)('0' + (number % 10U));
        number /= 10U;
    }

    while (i > 0U)
    {
        uart_transmit(buffer[--i]);
    }
}
