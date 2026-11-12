#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>

void uart_init(void);
void uart_transmit(char data);
void uart_print_number(uint16_t number);

#endif