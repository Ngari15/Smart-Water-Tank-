#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

static volatile uint8_t tick_flag = 0;

void timer_init(void){
    TCCR1A = 0;
    TCCR1B = 0;

    TCNT1 = 0;

    OCR1A = 24999;

    TCCR1B |= (1<<WGM12);

    TIMSK1 |= (1 << OCIE1A);

    TCCR1B |= (1 << CS11) | (1 << CS10);
}

ISR(TIMER1_COMPA_vect)
{
    tick_flag = 1;
}

bool timer_100ms_ticks(void)
{
    if (tick_flag)
    {
        tick_flag = 0;
        return true;
    }

    return false;
}
