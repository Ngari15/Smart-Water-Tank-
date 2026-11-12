#ifndef CONFIG_H
#define CONFIG_H

#include <avr/io.h>
#include <stdint.h>

#define SYSTEM_CLOCK 16000000UL

#define WATER_LEVEL_ADC 0U

#define PIN_SOURCE_EMPTY       PD2
#define PIN_HIGH_HIGH          PD3
#define PIN_MODE_SELECT        PD4
#define PIN_MANUAL_START       PD5
#define PIN_MANUAL_STOP        PD6

#define PUMP_RELAY             PD7
#define GREEN_LED              PB1
#define RED_LED                PB0

#define PIN_LCD_ENABLE         PB2
#define PIN_LCD_RS             PC5

#define PIN_LCD_D4             PC1
#define PIN_LCD_D5             PC2
#define PIN_LCD_D6             PC3
#define PIN_LCD_D7             PC4

#define START_LEVEL            15U
#define STOP_LEVEL             90U

#define MAX_RUNTIME_MS        900000UL

#endif