/*
           ________________________________________________________
          |  ____________________________________________________  |
          | |                                                    | |
          | |         Projeto EMI 1º ano - 1º semestre           | |
          | |               Frequencímetro V3.4.1                | |
          | |                                                    | |
          | | Autores:                                           | |
          | | - Aluno 1, 115216;                                 | |
          | | - Gonçalo Figueira, 109438;        | |
          | | - Aluno 3, 118734;                                 | |
          | | - Aluno 4, 115154;                                 | |
          | | - Aluno 5, 119337;                                 | |
          | |                                                    | |
          | | Placa utilizada: Arduino Uno R3;                   | |
          | |____________________________________________________| |
          |________________________________________________________|

*/

#include <LiquidCrystal_I2C.h>     // Biblioteca do Display com o módulo I2C.
LiquidCrystal_I2C lcd(39, 16, 2);  // Configuração do display.

const char* mensagemTensaoInicial = "Vrms:";      //Ponteiro constante que armazena o texto "Vrms:" usado como mensagem inicial para a tensão no LCD.
const char* mensagemFrequenciaInicial = "Freq:";  //Ponteiro constante que armazena o texto "Freq:" usado como mensagem inicial para a frequência no LCD.
const char* mensagemTensaoFinal = "V ";           //Ponteiro constante que armazena o texto "V " que será projetado no LCD para assinalar o valor de tensão.
const char* mensagemFrequenciaFinal = "Hz ";      //Ponteiro constante que armazena o texto "Hz " que será projetado no LCD para assinalar o valor de frequência.

const int pinosensor = A0;    // Pino A0 que vai fazer a leitura da tensão.
const byte interruptPin = 2;  // Pino 2 que vai fazer a leitura da frequência.

int inicio_onda = 0;            // Variável que deteta o início da onda, utilizada para calcular a frequência.
long tempofrequenciafinal = 0;  // Variável que armazena o tempo da última medição.

// Estrutura para armazenar variáveis relacionadas à frequência.
typedef struct VARIAVEIS_FREQUENCIA {

  unsigned int frequencia = 0;  // Variável que retêm a frequência calculada.
};

// Estrutura para armazenar variáveis relacionadas à tensão.
typedef struct VARIAVEIS_TENSAO {

  int valorinstantaneo[600];  // Array para armazenar leituras da tensão.
  float maiorvalor;           // Variável para ler o maior valor de tensao medido pelo arduino.
  float tensaopico;           // Variável para ler a Tensão de pico.
  float tensaopicoreal;       // Variável para ler a Tensão de pico real.
  float tensaorms;            // Variável para reter o valor da Tensão eficaz (RMS).
};

void ONDA0V() {

  inicio_onda++;  //Incremento da variável inicio de onda, que serve para calcular a frequência
}

// Função para calcular frequência.
void calcularFrequencia() {

  VARIAVEIS_FREQUENCIA frequenciamedida;           // Declaração de uma estrutura para armazenar variáveis relacionadas à frequência.
  frequenciamedida.frequencia = inicio_onda - 1;   // Ajuste da contagem para obter o valor correto de ondas num intervalo de tempo
  inicio_onda = 0;                                 // Inicia a variável "inicio_onda" como zero.
  lcd.setCursor(5, 1);                             // Posiciona o cursor no LCD.
  lcd.print("         ");                          // Limpa os valores anteriores no LCD antes de imprimir novos valores.
  lcd.setCursor(5, 1);                             // Posicionamento do Cursor.
  lcd.print((String)frequenciamedida.frequencia);  // Imprime o valor da frequencia no LCD.
  lcd.print(mensagemFrequenciaFinal);              // Imprime as unidades “Hz” no LCD.
  Serial.print("Frequencia:");                     // Imprime a mensagem que indica que o valor seguinte é a Frequência.
  Serial.print(frequenciamedida.frequencia);       // Imprime o valor da frequência medida.
  Serial.println("Hz");                            // Imprime a unidade de frequência (Hz).
  Serial.println();                                // Imprime uma linha em branco para melhorar a compreensão da informação no monitor serial.
}

// Função para calcular tensão.
void calcularTensao() {

  VARIAVEIS_TENSAO tensao;  //Declaração de uma estrutura para armazenar variáveis relacionadas à tensão.

  for (int i = 0; i < 600; i++) {

    tensao.valorinstantaneo[i] = analogRead(pinosensor);  //Faz leituras da tensão com intervalos de 2 milissegundos.

    delay(2);  // Espera 2 milissegundos.
  }

  for (int i = 0; i < 600; i++) {

    tensao.maiorvalor = max(tensao.maiorvalor, tensao.valorinstantaneo[i]);  // Identifica o maior valor lido.
  }

  tensao.tensaopico = (tensao.maiorvalor * 10) / 1024;                                                                                                                        //Calcula a tensão de pico.
  tensao.tensaopicoreal = -0.0107816075962 * pow(tensao.tensaopico, 3) + 0.2596137655497 * pow(tensao.tensaopico, 2) - 1.0008593991265 * tensao.tensaopico + 5.525145958787;  //Cálculo da tensão real (tensaopicoreal) baseado na tensão de pico medida pelo Arduino (tensaopicoarduino).
  tensao.tensaorms = tensao.tensaopicoreal / sqrt(2);                                                                                                                         //Calcula o valor da tensão eficaz.

  if (tensao.maiorvalor < 1) {

    lcd.setCursor(5, 0);             // Posiciona o cursor no LCD.
    lcd.print("         ");          // Limpa os valores anteriores no LCD antes de imprimir novos valores.
    lcd.setCursor(5, 0);             // Posiciona o cursor no LCD.
    lcd.print("0");                  // Imprime o valor da tensao RMS.
    lcd.print(mensagemTensaoFinal);  // Imprime a unidade da Tensão.
    Serial.print("Tensao RMS:");     // Imprime a mensagem "Tensão RMS: " indicando que o valor seguinte é a Tensão Eficaz (RMS).
    Serial.print("0");               // Imprime o valor da Tensão RMS.
    Serial.println("V");             // Imprime a unidade de voltagem (V).
    lcd.setCursor(5, 1);             // Posiciona o cursor no LCD.
    lcd.print("         ");          // Limpa os valores anteriores no LCD antes de imprimir novos valores.
    lcd.setCursor(5, 1);             // Posiciona o cursor no LCD.
    lcd.print("V Baixa");            // Exibe no LCD a mensagem "V Baixa" quando a tensão é considerada baixa para a medição.

    Serial.print("Frequencia:");  // Imprime no monitor serial a mensagem indicando que o valor seguinte é a frequência medida.
    Serial.print("V Baixa");      // Imprime no monitor serial a informação de que a frequência está associada a uma tensão baixa.

  }

  else {

    lcd.setCursor(5, 0);     // Posiciona o cursor no LCD.
    lcd.print("         ");  // Limpa os valores anteriores do LCD antes de imprimir novos valores.
    lcd.setCursor(5, 0);     // Posiciona o cursor no LCD.

    lcd.print((String)tensao.tensaorms);  // Imprime o valor da tensao RMS.
    lcd.print(mensagemTensaoFinal);       // Imprime as unidades de Tensão.
    Serial.print("Tensao RMS:");          // Imprime a mensagem que indica que o valor seguinte é a Tensão RMS.
    Serial.print(tensao.tensaorms);       // Imprime o valor da Tensão RMS.
    Serial.println("V");                  // Imprime a unidade de voltagem (V).

    if (tensao.tensaorms < 4.66) {

      lcd.setCursor(5, 1);          // Posiciona o cursor no LCD.
      lcd.print("V Baixa");         // Imprime "V Baixa" no LCD quando a tensão é considerada baixa para a medição
      Serial.print("Frequencia:");  // Imprime no monitor serial a mensagem indicando que o valor seguinte é a frequência.
      Serial.print("V Baixa");      // Imprime no monitor serial a informação de que a frequência está associada a uma tensão baixa.
    }

    else {

      calcularFrequencia();  // Chama a função para calcular a frequência.
    }
  }
}

    // Função de Setup.
void setup() {

  Serial.begin(9600);  // Inicia a comunicação serial a uma taxa de 9600 bps.

  lcd.setBacklight(HIGH);                // Ativa o backlight (iluminação) do LCD.
  lcd.init();                            // Inicia o LCD.
  lcd.clear();                           // Limpa o conteúdo do LCD.
  lcd.setCursor(0, 0);                   // Posiciona o cursor na primeira linha e na primeira coluna do LCD.
  lcd.print(mensagemTensaoInicial);      // Imprime a mensagem inicial para a tensão no LCD.
  lcd.setCursor(0, 1);                   // Posiciona o cursor na primeira linha e na segunda coluna do LCD.
  lcd.print(mensagemFrequenciaInicial);  // Imprime a mensagem inicial para a frequência no LCD.

  pinMode(pinosensor, INPUT);                                            // Configura o pino do sensor como entrada.
  attachInterrupt(digitalPinToInterrupt(interruptPin), ONDA0V, RISING);  // Configuração da interrupção.
}

//Função de Loop.
void loop() {

  if (millis() - tempofrequenciafinal >= 1000) {

    detachInterrupt(digitalPinToInterrupt(interruptPin));  // Temporariamente desativa a interrupção para nao interferir nas leituras.

    calcularTensao();  // Chama a função para calcular a tensão.

    attachInterrupt(digitalPinToInterrupt(interruptPin), ONDA0V, RISING);  // Reativa a interrupção.

    tempofrequenciafinal = millis();  // Atualiza o tempo da última medição.
  }
}
