/*
Projeto EMI 1º anos 1º semestre
Frequencimetro V3.0
Autores: Aluno 1; Aluno 2; Gonçalo Figueira; Aluno 3; Aluno 4;
Placa usada: Arduino Uno;
*/

#include <LiquidCrystal_I2C.h>     //biblioteca do Display com o modulo i2c
LiquidCrystal_I2C lcd(39, 16, 2);  //Configurção do display

int pinosensor = A0;          //Pino que vai fazer a leitura da tensão
const byte interruptPin = 2;  //pino que vai fazer a leitura da frequencia

int tempoesp = 1000;
float contador = 0;
long tempof = 0;
float frequencia = 0;

unsigned int valinst[500];
long tempov = 0;
float maiorval;
float tensaopico;
float tensaorms = 0;

void freq() {
  contador++;
}

void setup() {
  Serial.begin(9600);
  lcd.setBacklight(HIGH);
  lcd.init();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vrms:");
  lcd.setCursor(0, 1);
  lcd.print("Freq:");

  pinMode(pinosensor, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), freq, CHANGE);
}

void loop() {

  if (millis() - tempof >= tempoesp) {
    detachInterrupt(digitalPinToInterrupt(interruptPin));
    frequencia = contador / 2;
    contador = 0;
    lcd.setCursor(5, 1);
    lcd.print((String)frequencia);
    lcd.print("Hz  ");

    for (int i = 0; i < 500; i++) {
      valinst[i] = analogRead(pinosensor);
      delay(2);
    }

    for (int i = 0; i < 500; i++) {

      if (maiorval < valinst[i]) {
        maiorval = valinst[i];
      }
    }
    tensaopico = ((maiorval * 10) / 1024) * 1.121;
    tensaorms = tensaopico / 1.4;

    lcd.setCursor(5, 0);
    lcd.print((String)tensaorms);
    lcd.print("V ");

    maiorval = 0;
    attachInterrupt(digitalPinToInterrupt(interruptPin), freq, CHANGE);
    tempof = millis();
  }
}
