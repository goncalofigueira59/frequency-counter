# Frequencímetro Digital com Arduino UNO

![Categoria](https://img.shields.io/badge/Categoria-Projeto%20Académico-blue )
![Plataforma](https://img.shields.io/badge/Plataforma-Arduino%20UNO-00979D?logo=arduino )
![Linguagem](https://img.shields.io/badge/Linguagem-C-A8B9CC?logo=c )

> Frequencímetro Digital capaz de medir frequências de 1 Hz a 30 kHz e tensão RMS até 7.5V em sinais sinusoidais, triangulares e quadrados. Desenvolvido como projeto académico para a Unidade Curricular de Projeto Temático em Programação Aplicada (PTPA) da Licenciatura de Eletrónica e Mecânica Industrial da ESTGA - Universidade de Aveiro.

---

## Índice

- Funcionalidades
- Componentes Necessários
- Análise Técnica Detalhada
  - 1. Objetivo e Especificações Iniciais
  - 2. Arquitetura do Hardware
  - 3. Arquitetura do Software
  - 4. Fluxo de Operação
- Esquema Elétrico
- Resultados e Desempenho
- Conclusão e Melhorias Futuras
- Código Fonte
- Licença

---

## Visão Geral
Este projeto consistiu no desenvolvimento de um frequencímetro digital completo, integrando conhecimentos de eletrónica analógica e programação de microcontroladores. O sistema utiliza um Arduino UNO como núcleo de processamento e apresenta os resultados num display LCD 16x2 com interface I2C.

O desafio principal foi ultrapassar as especificações iniciais do enunciado (1-100 Hz) e criar um instrumento de medição robusto e preciso, implementando tanto o circuito condicionador de sinal como o firmware de aquisição e processamento de dados.

---

## Funcionalidades

- Medição de Frequência: Gama operacional de 1 Hz a 30 kHz (300× superior ao requisito inicial).
- Medição de Tensão RMS: Até 7.5V RMS (tensão de pico ~10.6V).
- Formas de Onda Suportadas: Sinusoidal, triangular e quadrada.
- Interface de Saída:** Display LCD 16x2 (via I2C) e Serial Monitor para visualização dos valores e análise em tempo real.
- Proteção de Entrada:** Condicionamento de sinal com divisor resistivo e retificação para proteção das entradas do Arduino.

---

## 🛠️ Componentes Necessários

### Hardware
| Componente | Quantidade |
| :--- | :---: |
| Arduino UNO | 1 |
| LCD 16x2 com Módulo I2C | 1 |
| Resistências 12 kΩ (±5%) | 2 |
| Diodo 1N4007 | 1 |
| Breadboard | 1 |
| Jumper Wires | ~8 |
| Gerador de Funções (TopWard 8110) | 1 |

### Software

| Ferramenta | Utilização |
| :--- | :--- |
| **Arduino IDE** | Desenvolvimento e upload do código |
| **Fritzing** | Criação do esquema elétrico |
| **GeoGebra** | Derivação da curva de calibração do ADC |

---

## Análise Técnica Detalhada

### 1. Objetivo e Especificações Iniciais
O projeto baseou-se no Projeto I da UC de Projeto Temático em Programação Aplicada. As especificações originais exigiam:
- Microcontrolador: Arduino Uno R3 (ATmega328P)
- Faixa de frequência: 1 Hz a 100 Hz
- Formas de onda: quadrada, sinusoidal e triangular
- Saída: Serial Monitor e Painel LCD

O projeto expandiu significativamente estes requisitos, focando-se na criação de um sistema completo de aquisição e condicionamento de sinal.

### 2. Arquitetura do Hardware
O circuito de condicionamento é fundamental para a precisão e segurança do sistema:
- Divisor Resistivo (1:2): Composto por duas resistências de 12 kΩ em série, reduz a amplitude do sinal de entrada para metade, permitindo medir tensões de pico até 10V mantendo-se dentro da gama de 0-5V do ADC do Arduino.
- Retificação de Meia-Onda: Um díodo 1N4007 em série remove o semiciclo negativo do sinal, protegendo o Arduino de tensões negativas e criando transições bem definidas (LOW→HIGH) essenciais para a medição de frequência por interrupção.
- Proteção: O circuito limita a corrente de entrada e previne danos ao microcontrolador.

### 3. Arquitetura do Software
O código foi estruturado modularmente para facilitar a manutenção e compreensão:

#### 3.1 Medição de Frequência (calcularFrequencia())
- Utiliza interrupções no pino digital 2 (attachInterrupt)
- A ISR ONDA0V() incrementa um contador a cada borda de subida
- A cada 1000 ms (controlado por millis()), o valor do contador é lido, resultando diretamente na frequência em Hz

#### 3.2 Medição de Tensão RMS (calcularTensao())
- Adquire 600 amostras no pino analógico A0 com intervalo de 2 ms
- Identifica o valor máximo entre as amostras
- Aplica um polinómio de calibração cúbico para compensar não-linearidades do circuito e ADC:

*_Vp_real = -0.01078·Vp³ + 0.25961·Vp² - 1.00086·Vp + 5.52515_*

- Calcula VRMS para ondas sinusoidais: Vrms = Vp_real / √2
- Inclui validações para descartar leituras com tensão insuficiente (< ~4.66V RMS)

#### 3.3 Interface e Comunicação
- Utiliza a biblioteca *_LiquidCrystal_I2C_* para controlo do display
- Envia dados paralelamente para o Serial Monitor para depuração

### 4. Fluxo de Operação
1. Inicialização (setup()): Configura comunicações (Serial, I2C), pinos e interrupção
2. Ciclo Principal (loop()): Controlado por temporizador de 1 segundo (millis())
- Desativa interrupção temporariamente
- Executa calcularTensao()
- Se tensão válida, executa calcularFrequencia()
- Atualiza display e Serial Monitor
- Reativa interrupção e reinicia contadores
3. ISR ONDA0V(): Executa assincronamente, incrementando o contador de períodos
---

## Esquema Elétrico
O circuito foi projetado e documentado utilizando a ferramenta Fritzing:
https://github.com/seu-usuario/seu-repositorio/blob/main/esquema_frequencimetro.png?raw=true

Legenda:
- Vin: Entrada do sinal a medir
- R1, R2 (12kΩ): Divisor de tensão
- D1 (1N4007): Díodo retificador/protetor

---

## Resultados

O sistema final excedeu as especificações iniciais, apresentando os seguintes resultados:

- **Faixa de Frequência:** 1 Hz a 30 kHz, com a Precisão de ~99.9% (sinais estáveis) aproximadamente.
- **Tensão Máxima:** 7.5V RMS (~10.6V pico)
- **Formas de Onda:** Quadrada, sinusoidal, triangular (RMS preciso apenas para sinusoidal)

Limitações identificadas:
- Cálculo RMS (Vp/√2) preciso apenas para ondas sinusoidais puras
- Resolução do ADC (10 bits) limita precisão em baixas tensões
- Circuito não mede True RMS para formas de onda complexas
---

###Conclusão e Melhorias Futuras

Este projeto serviu como integrador prático de conhecimentos em eletrónica analógica, microcontroladores e programação em C/C++. As principais lições técnicas foram a importância do condicionamento de sinal, técnicas de amostragem e temporização, e métodos de calibração de sistemas de medição.

*Possíveis melhorias técnicas futuras:*
1. Algoritmo True RMS para medição precisa em qualquer forma de onda
2. Circuito de ganho programável para maior resolução em baixas tensões e gama ampliada
3. Otimização de código:
- Deteção de pico em tempo real (sem array de 600 amostras)
- Uso de map() ou operações de bit-shift para conversão mais eficiente
4. Interface gráfica em Python com PySerial para visualização e registo de dados

---

### Código Fonte
O código completo está no repositório, no link. 

Para utilização, copie para a Arduino IDE, instale a biblioteca LiquidCrystal_I2C e carregue para o Arduino UNO.

---

## Licença

Este projeto está licenciado sob a **MIT License**. Veja o ficheiro `LICENSE` para mais detalhes.
