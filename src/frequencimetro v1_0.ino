#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(39,16,2);
const byte interruptPin = 2;
int contador=0;
 long tempo1=0;
 long tempo2=0;
int frequencia=0;


// interrupção
void freq1( ) {
tempo1=millis();

}


void setup()
{
  Serial.begin(9600);
   lcd.setBacklight(HIGH);
 lcd.init();
  lcd.print("Frequencia");
  attachInterrupt(digitalPinToInterrupt(interruptPin), freq1, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPin), freq2, Falling);
  
}

void loop()
{
meioperiodo
  }
} 
