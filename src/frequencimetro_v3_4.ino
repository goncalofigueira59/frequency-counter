/*
           ________________________________________________________
          |  ____________________________________________________  |
          | |                                                    | |
          | |         Projeto EMI 1º ano - 1º semestre           | |
          | |               Frequencímetro V3.4                  | |
          | |                                                    | |
          | | Autores:                                           | |
          | | - Aluno 1, 115216;                                 | |
          | | - Gonçalo Figueira, 109438;                        | |
          | | - Aluno 3, 118734;                                 | |
          | | - Aluno 4, 115154;                                 | |
          | | - Aluno 5, 119337;                                 | |
          | |                                                    | |
          | | Placa utilizada: Arduino Uno R3;                   | |
          | |____________________________________________________| |
          |________________________________________________________|

*/

#include <LiquidCrystal_I2C.h>  // Biblioteca do Display com o módulo I2C.

LiquidCrystal_I2C lcd(39, 16, 2);  // Configuração do display.

const char* mensagemTensaoInicial = "Vrms:";      //Constante que armazena o texto "Vrms:" que usado como mensagem inicial para a tensão no LCD.
const char* mensagemFrequenciaInicial = "Freq:";  //Constante que armazena o texto "Freq:" que usado como mensagem inicial para a frequência no LCD.
const char* mensagemTensaoFinal = "V ";           //Constante que armazena o texto "V " que será projetado no LCD para assinalar o valor de tensão.
const char* mensagemFrequenciaFinal = "Hz ";      //Constante que armazena o texto "Hz " que será projetado no LCD para assinalar o valor de frequência.

const int pinosensor = A0;    // Pino A0 que vai fazer a leitura da tensão.
const byte interruptPin = 2;  // Pino 2 que vai fazer a leitura da frequência.

float contador = 0;             // Contador de interrupções.
long tempofrequenciafinal = 0;  // Armazena o tempo da última medição.

// Estrutura para armazenar variáveis relacionadas à frequência.
struct VARIAVEIS_FREQUENCIA {

  unsigned int frequencia = 0;  // Frequência calculada.
};

// Estrutura para armazenar variáveis relacionadas à tensão.
struct VARIAVEIS_TENSAO {

  int valorinstantaneo[625];  // Array para armazenar leituras da tensão.
  float maiorvalor;           // Maior valor de tensao medida pelo arduino.
  float tensaopicoarduino;    // Tensão de pico.
  float tensaopicoreal;       // Tensão de pico real.
  float tensaorms;            // Tensão eficaz (RMS).
};

void ONDA0V() {

  contador++;  // Contador que incrementa a cada interrupção.
}

// Função para calcular tensão.
void calcularTensao() {

  VARIAVEIS_TENSAO tensao; //Declaração de uma estrutura para armazenar variáveis relacionadas à tensão.
 
  for (int i = 0; i < 625; i++) {

    tensao.valorinstantaneo[i] = analogRead(pinosensor);  //Faz leituras da tensão.

    delay(2);  // Espera 2 milissegundos.
  }

  for (int i = 0; i < 625; i++) {

    tensao.maiorvalor = max(tensao.maiorvalor, tensao.valorinstantaneo[i]);  // Identifica o maior valor lido.
  }

  tensao.tensaopicoarduino = (tensao.maiorvalor * 10) / 1024;  //Calcula a tensão de pico.
  tensao.tensaopicoreal = -0.0108 * pow(tensao.tensaopicoarduino, 3) + 0.2596 * pow(tensao.tensaopicoarduino, 2) - 1.0009 * tensao.tensaopicoarduino + 5.5251; 
  // Cálculo da tensão real (tensaopicoreal) baseado na tensão de pico medida pelo Arduino (tensaopicoarduino).
  tensao.tensaorms = tensao.tensaopicoreal / sqrt(2);  //Calcula a tensão RMS.
  tensao.maiorvalor = 0;                               // Reinicia a variável do maior valor.

  lcd.setCursor(5, 0);                  // Posiciona o cursor no LCD.
  lcd.print((String)tensao.tensaorms);  // Imprime o valor da tensao RMS.
  lcd.print(mensagemTensaoFinal);       // Imprime as unidades de Tensão.
  Serial.print("Tensao RMS:");          // Imprime a mensagem indicando que o valor seguinte é a Tensão RMS.
  Serial.print(tensao.tensaorms);       // Imprime o valor da Tensão RMS.
  Serial.println("V");                  // Imprime a unidade de voltagem (V).
  Serial.println();                     // Imprime uma linha em branco para melhor visualização no monitor serial.
}

// Função para calcular frequência.
void calcularFrequencia() {

  VARIAVEIS_FREQUENCIA frequenciamedida;  //Declaração de uma estrutura para armazenar variáveis relacionadas à frequência.

  frequenciamedida.frequencia = contador / 2;  // Calcula a frequência.
  contador = 0;                                // Reinicia o contador.

  lcd.setCursor(5, 1);                             //Posicionamento do Cursor.
  lcd.print((String)frequenciamedida.frequencia);  //Imprime o valor da frequencia no LCD.
  lcd.print(mensagemFrequenciaFinal);              //Imprime as unidades “Hz” no LCD.
  Serial.print("Frequencia:");                     // Imprime a mensagem indicando que o valor seguinte é a Frequência.
  Serial.print(frequenciamedida.frequencia);       // Imprime o valor da frequência medida.
  Serial.println("Hz");                            // Imprime a unidade de frequência (Hz).
}

// Função de Setup.
void setup() {

  Serial.begin(9600);                     // Inicia a comunicação serial a uma taxa de 9600 bps.

  lcd.setBacklight(HIGH);                 // Ativa o backlight (iluminação) do LCD.
  lcd.init();                             // Inicia o LCD.
  lcd.clear();                            // Limpa o conteúdo do LCD.
  lcd.setCursor(0, 0);                    // Posiciona o cursor na primeira linha e na primeira coluna do LCD.
  lcd.print(mensagemTensaoInicial);       // Imprime a mensagem inicial para a tensão no LCD.
  lcd.setCursor(0, 1);                    // Posiciona o cursor na primeira linha e na segunda coluna do LCD.
  lcd.print(mensagemFrequenciaInicial);   // Imprime a mensagem inicial para a frequência no LCD.

  pinMode(pinosensor, INPUT);             // Configura o pino do sensor como entrada.
  attachInterrupt(digitalPinToInterrupt(interruptPin), ONDA0V, CHANGE);  // Configuração da interrupção.
}

//Função de Loop.
void loop() {

  if (millis() - tempofrequenciafinal >= 1000) {

    detachInterrupt(digitalPinToInterrupt(interruptPin));  // Temporariamente desativa a interrupção para nao interferir nas leituras.

    calcularFrequencia();  // Chama a função para calcular a frequência.
    calcularTensao();      // Chama a função para calcular a tensão.

    attachInterrupt(digitalPinToInterrupt(interruptPin), ONDA0V, CHANGE);  // Reativa a interrupção.

    tempofrequenciafinal = millis();  // Atualiza o tempo da última medição.
  }
}
