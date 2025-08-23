# 🎛️ Controle e Monitoramento de Movimento com MPU6050 + Servo + LEDs + OLED

Este projeto implementa um **sistema embarcado** utilizando o **Raspberry Pi Pico W**, que realiza:

- Leitura de movimento (aceleração e inclinação) através do **sensor IMU MPU6500** via **I²C**.
- Controle de um **servo motor** conforme a inclinação detectada.
- Indicação visual de movimento e **alerta de inclinação** através de **LEDs** e de um **display OLED SSD1306**.


---

## ⚙️ Funcionalidades

1. **Leitura de Movimento (MPU6500)**
   - Captura valores brutos dos eixos **X, Y e Z**.
   - Converte para aceleração em `g`.

2. **Controle de Servo Motor**
   - O ângulo do servo é calculado a partir da inclinação no eixo X.
   - Movimento suave até o ângulo desejado.
   - Conversão automática de ângulo → PWM.

3. **Sistema de LEDs**
   - **LED Verde** → Inclinação positiva (frente/direita).  
   - **LED Vermelho** → Inclinação negativa (trás/esquerda).  
   - **LED Azul** → Posição neutra ou vertical.  
   - **LED Vermelho piscando** → ⚠ **ALERTA de inclinação excessiva**.

4. **Display OLED SSD1306**
   - Exibe em tempo real:
     - Ângulo do servo.
     - Valores dos eixos X, Y, Z.
     - Direção atual (Frente, Trás, Dir, Esq, Cima, Baixo, Neutro).
   - Em caso de alerta, substitui a tela por:
     ```
     ⚠ ALERTA
     INCLINACAO!
     ```

---

## 🖥️ Hardware Utilizado

- **Raspberry Pi Pico W**  
- **MPU6500 (IMU 6 eixos)**  
- **Servo Motor SG90 (ou similar)**  
- **OLED SSD1306 (I²C, 128x64)**  
- **LEDs RGB (pinos GPIO 11, 12, 13)**  
- Resistores, jumpers, protoboard.  

---

## 📌 Ligações

| Componente | Pino Pico W |
|------------|-------------|
| I²C SDA    | GP0         |
| I²C SCL    | GP1         |
| Servo      | GP2 (PWM)   |
| LED Verde  | GP11        |
| LED Azul   | GP12        |
| LED Verm.  | GP13        |

---

## 🛠️ Como Compilar e Executar

1. Clone o projeto:
   ```bash
   git clone https://github.com/ThalesFortes/Control-and-Monitoring-with-MPU6050.git
   cd projeto-mpu6050-servo

    Configure o SDK do Pico e o CMake.

    Compile:

    mkdir build && cd build
    cmake ..
    make


    Grave o .uf2 no Raspberry Pi Pico W.

## ⚠ Sistema de Alerta de Inclinação

O ângulo calculado é comparado com um limite definido:

#define ANGULO_ALERTA 120.0f


Se o limite for ultrapassado:

O LED vermelho pisca 3 vezes.

O display OLED mostra mensagem de alerta.

O terminal serial imprime:

⚠ ALERTA: Angulo acima do limite (120°)!

##  📊 Exemplo de Saída no Serial
Sistema iniciado: MPU6500 + Servo + LEDs
Acc X: 0.12 | Y: -0.34 | Z: 0.95 | Servo: 87° | Dir: Frente
Acc X: 0.56 | Y: -0.10 | Z: 0.82 | Servo: 123° | Dir: Dir ⚠ ALERTA: Angulo acima do limite (120°)!

## 🚀 Melhorias Futuras

Implementar controle via MQTT/Wi-Fi (com Pico W).

Gravar dados em cartão SD.

Ajustar limite de alerta dinamicamente.

Acrescentar filtro digital (Kalman/Complementar) para suavizar leituras.