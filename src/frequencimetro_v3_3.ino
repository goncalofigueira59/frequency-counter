/*
           ________________________________________________________
          |  ____________________________________________________  |
          | |                                                    | |
          | |         Projeto EMI 1º ano - 1º semestre           | |
          | |               Frequencímetro V3.3                  | |
          | |                                                    | |
          | | Autores:                                           | |
          | | - Aluno 1, 115216;                                 | |
          | | - Gonçalo Figueira, 109438;                        | |
          | | - Aluno 2, 118734;                                 | |
          | | - Aluno 3, 115154;                                 | |
          | | - Aluno 4, 119337;                                 | |
          | |                                                    | |
          | |          Placa utilizada: Arduino Uno R3;          | |
          | |____________________________________________________| |
          |________________________________________________________|

*/

#include <LiquidCrystal_I2C.h>  // Biblioteca do Display com o módulo I2C

LiquidCrystal_I2C lcd(39, 16, 2);  // Configuração do display

const char* mensagemTensaoInicial = "Vrms:";
const char* mensagemFrequenciaInicial = "Freq:";
const char* mensagemTensaoFinal = "V ";
const char* mensagemFrequenciaFinal = "Hz ";

const int pinosensor = A0;    // Pino A0 que vai fazer a leitura da tensão
const byte interruptPin = 2; // Pino 2 que vai fazer a leitura da frequência

float contador = 0;             // Contador de interrupções
long tempofrequenciafinal = 0; // Armazena o tempo da última medição

struct VARIAVEIS_FREQUENCIA {

  unsigned int frequencia = 0;  // Frequência calculada
};

struct VARIAVEIS_TENSAO {

  int valorinstantaneo[625];   // Array para armazenar leituras da tensão
  float maiorvalor;           //Maior valor de tensao medida pelo o arduino
  float tensaopicoarduino;   // Tensão de pico
  float tensaopicoreal;
  float tensaorms;  // Tensão RMS
};

void ONDA0V() {

  contador++;  // Contador que incrementa a cada interrupção
}

// Função para calcular tensão
void calcularTensao() {

  VARIAVEIS_TENSAO tensao;
  for (int i = 0; i < 625; i++) {

    tensao.valorinstantaneo[i] = analogRead(pinosensor);  //Faz leituras da tensão

    delay(2);  // Espera 2 milissegundos
  }

  for (int i = 0; i < 625; i++) {

    tensao.maiorvalor = max(tensao.maiorvalor, tensao.valorinstantaneo[i]);  // Identifica o maior valor lido
  }

  tensao.tensaopicoarduino = (tensao.maiorvalor * 10) / 1024;  //Calcula a tensão de pico
  tensao.tensaopicoreal = -0.0108 * pow(tensao.tensaopicoarduino, 3) + 0.2596 * pow(tensao.tensaopicoarduino, 2) - 1.0009 * tensao.tensaopicoarduino + 5.5251;
  tensao.tensaorms = tensao.tensaopicoreal / sqrt(2);   //Calcula a tensão RMS
  tensao.maiorvalor = 0;                               // Reinicia a variável do maior valor

  lcd.setCursor(5, 0);                   //Posiciona o cursor no LCD
  lcd.print((String)tensao.tensaorms);  //Imprime o valor da tensao RMS
  lcd.print(mensagemTensaoFinal);      //Imprime as unidades de voltagem,
  Serial.print("Tensao RMS:");
  Serial.print(tensao.tensaorms);
  Serial.println("V");
  Serial.println();
}

// Função para calcular frequência
void calcularFrequencia() {

  VARIAVEIS_FREQUENCIA frequenciamedida;

  frequenciamedida.frequencia = contador / 2;   // Calcula a frequência
  contador = 0;                                // Reinicia o contador

  lcd.setCursor(5, 1);                              //Posicionamento do Cursor
  lcd.print((String)frequenciamedida.frequencia);  //Imprime o valor da frequencia no LCD
  lcd.print(mensagemFrequenciaFinal);             //Imprime as unidades “Hz” no LCD
  Serial.print("Frequencia:");
  Serial.print(frequenciamedida.frequencia);
  Serial.println("Hz");
}

void setup() {

  Serial.begin(9600);

  lcd.setBacklight(HIGH);
  lcd.init();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(mensagemTensaoInicial);
  lcd.setCursor(0, 1);
  lcd.print(mensagemFrequenciaInicial);

  pinMode(pinosensor, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ONDA0V, CHANGE);  // Configuração da interrupção
}

void loop() {

  if (millis() - tempofrequenciafinal >= 1000) {

    detachInterrupt(digitalPinToInterrupt(interruptPin));  // Temporariamente desativa a interrupção para nao interferir nas leituras

    calcularFrequencia();   // Chama a função para calcular a frequência
    calcularTensao();      // Chama a função para calcular a tensão

    attachInterrupt(digitalPinToInterrupt(interruptPin), ONDA0V, CHANGE);  // Reativa a interrupção

    tempofrequenciafinal = millis();  // Atualiza o tempo da última medição
  }
}
