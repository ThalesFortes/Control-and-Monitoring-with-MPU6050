#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

#define SERVO_PIN 2
#define ANGULO_ALERTA 120.0f   // Limite de inclinação para alerta

void servo_init(void);
uint16_t servo_angulo_para_pwm(float angulo);
float servo_mover_por_eixos(float acc_x, float acc_y, float acc_z);

#endif
