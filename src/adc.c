#include "adc.h"

#include <avr/io.h>
#include <stdint.h>

void adc_init(void)
{
    /* Disable ADC before configuring it. */
    ADCSRA &= ~(1U << ADEN);

    /* AVCC reference, ADC0 selected. */
    ADMUX = (1U << REFS0);

    /* Disable digital input buffer on ADC0. */
    DIDR0 |= (1U << ADC0D);

    /* Enable ADC with prescaler 128. */
    ADCSRA = (1U << ADEN) |
             (1U << ADPS2) |
             (1U << ADPS1) |
             (1U << ADPS0);
}

uint16_t adc_read(uint8_t channel){
    if (channel > 5){
        return 0;
    }

    ADMUX = (ADMUX & 0xF0U) | (channel & 0x0F);

    ADCSRA |= (1<<ADSC);

    while((ADCSRA & (1<<ADSC))!= 0){

    }
    return ADC;
}

uint8_t adc_read_percentage(uint8_t channel){
    const uint16_t raw_value = adc_read(channel);

    return (uint8_t)(((uint32_t)raw_value*100)/1023U);
}