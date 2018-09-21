/******************************************
 *              Greyjoy 3.0               *
 *                                        *
 *    An autonomous sumo fight robot      *
 *                                        *
 * Author: Wederson Silva                 *
 ******************************************/

#include <Servo.h> 

int SL1 = 2;          //Sensor de linha 1
int SL2 = 4;          //Sensor de linha 2
const int TRIG = 13;    //variavel emissor do ultrassom 1
const int ECHO = 12;    //variavel receptor do ultrassom 1
int VALOR_SL1 = 0;                // Captura o valor do sensor de linha da frente
int VALOR_SL2 = 0;                // Captura o valor do sensor de linha de tras
unsigned long TEMPO_PULSO;        // Tempo do pulso do ultrasson
unsigned long DISTANCIA;          // Distancia calculada
int LIMIAR = 800;                 // Limiar de valor para considerar linha branca
int FLAG = 0;                     // FLAG que auxilia na funcao PROCURA
Servo servoLeft, servoRight;      // Servo motor da esquerda e da direita

void LER_SL();                    // Funcao para LER SENSOR DE LINHA
void LER_SOM();                   // Funcao para LER SENSOR ULTRASSONICO
void FRENTE();                    // Funcao que movimente robo para FRENTE
void TRAS();                      // Funcao que movimente robo para TRAS
void ESQUERDA();                  // Funcao que movimente robo para ESQUERDA
void DIREITA();                   // Funcao que movimente robo para DIREITA
void PROCURAR();                  // Funcao que PROCURA O ADVERSARIO
void ESCAPAR();                   // Funcao para ESCAPAR DA LINHA BRANCA
void COMECA();
void PARA();

void setup() {
  Serial.begin(9600);
    
  delay(2000);
  
  servoLeft.attach(6);       
  servoRight.attach(8);
  pinMode(TRIG, OUTPUT);
}

void LER_SL(){        
      VALOR_SL1 = readQD1();      // LER SENSOR DE LINHA FRENTE
      Serial.println(VALOR_SL1);
      VALOR_SL2 = readQD2();      // LER SENSOR DE LINHA TRAS    
      Serial.println(VALOR_SL2);
}


int readQD1() {
  
  pinMode( 2, OUTPUT );     //Pino digital 2 do Vout do linha
  digitalWrite( 2, HIGH );  //breakout como saída para descarregar o capacitor, colocando o pino digital 2 do Arduino
  delayMicroseconds(10);
  pinMode( 2, INPUT );      // pino do Arduino (digital 2) como entrada
  long time = micros();
  
  while (digitalRead(SL1) == HIGH && micros() - time < 3000); 
  int diff = micros() - time;
  return diff;
}

int readQD2() {
 
  pinMode( 4, OUTPUT );     //Pino digital 2 do Vout do linha
  digitalWrite( 4, HIGH );  //breakout como saída para descarregar o capacitor, colocando o pino digital 2 do Arduino
  delayMicroseconds(10);
  pinMode( 4, INPUT );      // pino do Arduino (digital 2) como entrada
  long time2 = micros();

  while (digitalRead(SL2) == HIGH && micros() - time2 < 3000); 
  int diff2 = micros() - time2;

  return diff2;
}

void COMECA()
{
 servoLeft.write(270);
 servoRight.write(270);
 }

void FRENTE()
{
  servoLeft.write(0);
  servoRight.write(0);  
  Serial.println("ATAQUE");
}

void TRAS()
{
  servoLeft.write(180);
  servoRight.write(180);
  Serial.println(" RECUO ");  
}

void ESQUERDA()
{
  servoLeft.write(180);
  servoRight.write(0);  
  Serial.println("Desvio para ESQUERDA");
}

void DIREITA()
{
  servoLeft.write(180);
  servoRight.write(0);  
  Serial.println("Desvio para DIREITA");
}

void PARA()
{
 servoLeft.write(270);
 servoRight.write(270); 
}

void LER_SOM()                    // Funcao para LER SENSOR ULTRASSONICO
{ 
  digitalWrite(TRIG, LOW);        // Desliga o TRIG
  delayMicroseconds(5);           // Espera por 0.005 SEGUNDOS
  digitalWrite(TRIG, HIGH);       // Liga o TRIG
  delayMicroseconds(10);          // Espera por 0.010 SEGUNDOS
  digitalWrite(TRIG, LOW);        // Desliga o TRIG
  
  TEMPO_PULSO = pulseIn(ECHO, HIGH);    // CALCULA O TEMPO DO PULSO DO ULTRASSOM
  DISTANCIA = (TEMPO_PULSO / 2) / 29.1; // CALCULA A DISTANCIA em CENTIMETROS      

  Serial.print("Distância = ");
  Serial.print(DISTANCIA);
  Serial.println(" cm");  
}   

void PROCURAR()                   // Funcao que PROCURA O ADVERSARIO
{     
  FLAG = FLAG + 1;                // FLAG para auxiliar a funcao de PROCURAR
  
  if(FLAG <= 25){                  
    DIREITA();                    // Girar para a DIREITA
  }
  else if((FLAG > 25) && (FLAG <= 32)){ 
    FRENTE();                     // Ir para FRENTE
  }
  else if((FLAG > 32) && (FLAG <= 57)){ 
    ESQUERDA();                   // Girar para a ESQUERDA
  }
  else if ((FLAG > 57) && (FLAG <= 74)){
    FRENTE();                     // Ir para FRENTE
  }
  else{  
    FLAG = random(75);             // Use RANDOM quando FLAG > 6
  }
}

void ESCAPAR()                    // Funcao para ESCAPAR DA LINHA BRANCA
{   
  FLAG = 0;
  
  if((VALOR_SL1 < LIMIAR) && (VALOR_SL2 < LIMIAR)){
    FRENTE();                     // Ir para TRAS  
    delay(250);
  }
  else if((VALOR_SL2 > LIMIAR) && (VALOR_SL1 < LIMIAR)){
    TRAS();
    delay(750);
  }
  else if((VALOR_SL2 < LIMIAR) && (VALOR_SL1 > LIMIAR)){
    TRAS();
    delay(750);
  }    
  else{
    ESQUERDA();
    delay(100);                   
    FLAG = 0;                       // ZERA A FLAG para quando o ROBO PROCURAR O ADVERSARIO
  }  
}

void loop() {

  LER_SL();                       // LER SENSOR DE LINHA
  LER_SOM();                      // LER SENSOR ULTRASSON  

  if((VALOR_SL1 < LIMIAR) || (VALOR_SL2 < LIMIAR))           
  {
    ESCAPAR();                    // FUGIR
  }  
  else if(DISTANCIA < 40){        // SE DISTANCIA < 30 CM
    FRENTE();                     // ATACAR
    Serial.println("FRENTE");
  }
  else if(DISTANCIA > 39){        // SE DISTANCIA > 29 CM
    PROCURAR();                   // PROCURAR ADVERSARIO
    Serial.println("PROCURAR");
  }      
}
