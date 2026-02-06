unsigned long tempo1;
unsigned long tempo2;
float meioper;
float periodo;
float som_meioper;
float med_meioper;
float frequencia;
int contagem = 0;
int valorsensor;
int pinosensor = A0;
int valinst[500];
double maiorval;
float tensaopico;
float tensaorms;

void setup() {
  Serial.begin(9600);
  pinMode(pinosensor, INPUT);
}

void loop() {
  maiorval = 0;
  valorsensor = analogRead(pinosensor);
  //Serial.println(valorsensor);
  if (valorsensor > maiorval && valorsensor < maiorval)  {
    tempo1 = millis();

    delay(1);
    valorsensor = analogRead(pinosensor);
    while (valorsensor < maiorval|| valorsensor > maiorval) {
      valorsensor = analogRead(pinosensor);
    }
    tempo2 = millis();

    meioper = tempo2 - tempo1;


    /*while(contagem<=100){
    
      
 som_meioper=som_meioper+meioper;
     Serial.println( som_meioper);
contagem++;
  }*/
    //med_meioper=som_meioper/100;

  
    periodo = meioper * 2;
    frequencia = 1000 / periodo;
    Serial.print("Frequencia:");
    Serial.print(frequencia);
    Serial.println("Hz");
  }
    for (int i = 0; i < 500; i++) {
      valinst[i] = analogRead(pinosensor);
    }
      
    }
tensaopico = map(maiorval,514,1024,0,354);
tensaorms=tensaopico*1.414;
Serial.print("tensão:");
Serial.print(tensaorms);
Serial.println("V");
  
}
