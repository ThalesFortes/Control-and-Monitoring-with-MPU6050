#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "servo.h"
#include "leds.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static float angulo_atual = -1;

uint16_t servo_angulo_para_pwm(float angulo) {
    float pulso = 0.5f + (angulo / 180.0f) * 2.0f;
    return (uint16_t)((pulso / 20.0f) * 20000.0f);
}

void servo_init(void) {
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f);
    pwm_config_set_wrap(&config, 20000);

    pwm_init(slice_num, &config, true);
}

float servo_mover_por_eixos(float acc_x, float acc_y, float acc_z) {
    // Calcula ângulo
    float angulo_calc = atan2f(acc_x, sqrtf(acc_y*acc_y + acc_z*acc_z)) * (180.0f / M_PI);
    float angulo = fminf(fmaxf(90.0f + angulo_calc, 0.0f), 180.0f);

    // Movimento suave
    if (angulo_atual < 0) angulo_atual = angulo;
    float passo = (angulo > angulo_atual) ? 1 : -1;

    while (fabs(angulo - angulo_atual) > 1) {
        angulo_atual += passo;
        pwm_set_gpio_level(SERVO_PIN, servo_angulo_para_pwm(angulo_atual));
        sleep_ms(10);
    }

    angulo_atual = angulo;

    // ============================
    // 🔴 ALERTA DE INCLINAÇÃO
    // ============================
    if (angulo >= ANGULO_ALERTA) {
        // Pisca LED vermelho como alerta
        for (int i = 0; i < 3; i++) {
            leds_off_all();
            led_on(LED_VERMELHO);
            sleep_ms(150);
            leds_off_all();
            sleep_ms(150);
        }
        return angulo; // não precisa mostrar LEDs normais
    }

    // LEDs de direção (modo normal)
    leds_off_all();
    if (fabs(acc_x) > fabs(acc_y) && fabs(acc_x) > fabs(acc_z)) {
        (acc_x > 0.2f) ? led_on(LED_VERDE) : led_on(LED_VERMELHO);
    } else if (fabs(acc_y) > fabs(acc_z)) {
        (acc_y > 0.2f) ? led_on(LED_VERDE) : led_on(LED_VERMELHO);
    } else {
        (acc_z > 0.8f) ? led_on(LED_AZUL) : led_on(LED_VERMELHO);
    }

    return angulo;
}
