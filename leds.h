#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>

// Enum para facilitar o uso dos LEDs
typedef enum {
    LED_VERMELHO = 13,
    LED_VERDE    = 11,
    LED_AZUL     = 12
} led_t;

// Inicializa todos os LEDs
void leds_init(void);

// Liga um LED específico
void led_on(led_t led);

// Desliga um LED específico
void led_off(led_t led);

// Desliga todos os LEDs
void leds_off_all(void);

#endif
