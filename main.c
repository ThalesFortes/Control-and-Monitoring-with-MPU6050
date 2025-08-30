#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "mpu6500.h"
#include "inc/ssd1306.h"
#include "inc/ssd1306_fonts.h"
#include "servo.h"
#include "leds.h"

#define I2C_PORT i2c0
#define I2C_SDA 0
#define I2C_SCL 1

int main() {
    stdio_init_all();
    while (!stdio_usb_connected()) sleep_ms(100);
    printf("Sistema iniciado: MPU6500 + Servo + LEDs\n");

    // Inicialização I2C
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicialização periféricos
    ssd1306_Init();
    ssd1306_Fill(Black);
    ssd1306_UpdateScreen();
    mpu6500_init(I2C_PORT);
    leds_init();
    servo_init();

    char buffer[32];

    while (1) {
        mpu6500_data_t dados;
        mpu6500_read_raw(I2C_PORT, &dados);

        float acc_x = dados.accel[0] / 16384.0f;
        float acc_y = dados.accel[1] / 16384.0f;
        float acc_z = dados.accel[2] / 16384.0f;

        float angulo = servo_mover_por_eixos(acc_x, acc_y, acc_z);

        // Determinar direção
        const char* direcao = "Neutro";
        if (acc_x > 0.5f) direcao = "Dir";
        else if (acc_x < -0.5f) direcao = "Esq";
        else if (acc_y > 0.5f) direcao = "Frente";
        else if (acc_y < -0.5f) direcao = "Tras";
        else if (acc_z > 0.5f) direcao = "Cima";
        else if (acc_z < -0.5f) direcao = "Baixo";

        printf("Acc X: %.2f | Y: %.2f | Z: %.2f | Servo: %.0f° | Dir: %s",
            acc_x, acc_y, acc_z, angulo, direcao);

        if (angulo >= ANGULO_ALERTA) {
            printf(" ⚠ ALERTA: Angulo acima do limite (%.0f°)!\n", ANGULO_ALERTA);

            ssd1306_Fill(Black);
            ssd1306_SetCursor(10, 20);
            ssd1306_WriteString("⚠ ALERTA", Font_11x18, White);
            ssd1306_SetCursor(10, 40);
            ssd1306_WriteString("INCLINACAO!", Font_11x18, White);
            ssd1306_UpdateScreen();

            sleep_ms(1000); 
            continue;      
        } else {
            printf("\n");
        }

        // Exibe dados normais no OLED
        char buffer[32];
        ssd1306_Fill(Black);
        ssd1306_SetCursor(0, 0);
        ssd1306_WriteString("Monitor MPU/SERVO", Font_7x10, White);

        snprintf(buffer, sizeof(buffer), "Servo: %.0f graus", angulo);
        ssd1306_SetCursor(0, 12);
        ssd1306_WriteString(buffer, Font_7x10, White);

        snprintf(buffer, sizeof(buffer), "X: %.2f", acc_x);
        ssd1306_SetCursor(0, 24);
        ssd1306_WriteString(buffer, Font_7x10, White);

        snprintf(buffer, sizeof(buffer), "Y: %.2f Dir:%s", acc_y, direcao);
        ssd1306_SetCursor(0, 36);
        ssd1306_WriteString(buffer, Font_7x10, White);

        snprintf(buffer, sizeof(buffer), "Z: %.2f", acc_z);
        ssd1306_SetCursor(0, 48);
        ssd1306_WriteString(buffer, Font_7x10, White);

        ssd1306_UpdateScreen();
        sleep_ms(500);
    }
}
