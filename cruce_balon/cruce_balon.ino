#include <dummy.h>

// Definimos librerias y variables para tiras led (FastLED)
#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ESP8266_D1_PIN_ORDER
#include "FastLED.h"

// Static pins NodeMCU
//    static const uint8_t D0   = 16;
//    static const uint8_t D1   = 5;
//    static const uint8_t D2   = 4;
//    static const uint8_t D3   = 0;
//    static const uint8_t D4   = 2;
//    static const uint8_t D5   = 14;
//    static const uint8_t D6   = 12;
//    static const uint8_t D7   = 13;
//    static const uint8_t D8   = 15;
//    static const uint8_t D9   = 3;
//    static const uint8_t D10  = 1;

// Cantidad de leds por tira y numero total de tiras
#define NUM_LEDS 40
#define NUM_STRIPS 3

// Definimos pines de sensores y tiras
#define SENSOR_PIN_1 16 // D0
#define SENSOR_PIN_2 5  // D1
#define SENSOR_PIN_3 4  // D2

#define STRIP_PIN_1 0   // D3
#define STRIP_PIN_2 2   // D4
#define STRIP_PIN_3 14  // D5

// Definimos brillo
#define BRIGHTNESS  64

// Define the array of leds
CRGB LEDS_STRIPS[NUM_STRIPS][NUM_LEDS];

// Variables para cualquier entrenamiento
int timeStart = 0;
int timeStop = 0;
int timeTotal = 0;

// Variables para entrenamiento Simon
const int sequenceMax = 5;
int sequence[sequenceMax];
int sequenceUser[sequenceMax];
const int timeTotalShow=2000;
int level;

void setup() { 
      // 
      FastLED.addLeds<NEOPIXEL, STRIP_PIN_1>(LEDS_STRIPS[0], NUM_LEDS);
      FastLED.addLeds<NEOPIXEL, STRIP_PIN_2>(LEDS_STRIPS[1], NUM_LEDS);
      FastLED.addLeds<NEOPIXEL, STRIP_PIN_3>(LEDS_STRIPS[2], NUM_LEDS);

      FastLED.setBrightness( BRIGHTNESS );

      Serial.begin(115200);
}

void loop() { 
  //cruceSimple();
  cruceSimon();
}

// ----------------------------------------------------------------
// -- Funcion cruceSimple() para el modo de entrenamiento Simple --
// ----------------------------------------------------------------

void cruceSimple(){
  Serial.print("Comienzo");
    
  randomSeed(millis());
  int numAleatorio = random(3);
  
  int cruce = -1;
  int correcto = 0;
  int error = 0;

  Serial.print("Numero aleatorio: ");
  Serial.println(numAleatorio);
  
  if(numAleatorio==0){
    turnOnGreen(0);
    turnOnRed(1);
    turnOnRed(2);
  } else if(numAleatorio==1){
    turnOnRed(0);
    turnOnGreen(1);
    turnOnRed(2);
  } else if(numAleatorio==2){
    turnOnRed(0);
    turnOnRed(1);
    turnOnGreen(2);
  }
  
  FastLED.show();
  // Comenzamos a contar el tiempo
  timeStart = millis();

  cruce=-1;
  
  // Vemos por cual de los agujeros cruzó la pelota
  while(cruce<0){
    ESP.wdtFeed();
    if(digitalRead(SENSOR_PIN_1)==0){
      cruce=0;
    } else if(digitalRead(SENSOR_PIN_2)==0){
      cruce=1;
    } else if(digitalRead(SENSOR_PIN_3)==0){
      cruce=2;
    }
  }
  turnOnBlue(cruce);
  FastLED.show();
  tiempoEsperaLed(400); 
  // Guardamos el tiempo que tardó en finalizar el juego
  timeTotal = millis() - timeStart;
  int timeTotalSec = timeTotal/1000;

  
  if(cruce == numAleatorio){
    correcto++;
  } else{
    error++;
  }
  
  
  Serial.print("Cruce: ");
  Serial.println(cruce);
  Serial.print("Correcto: ");
  Serial.println(correcto);
  Serial.print("Error: ");
  Serial.println(error);
  Serial.print("Tiempo: ");
  Serial.println(timeTotalSec);
  
  delay(500);
}

// --------------------------------------------------------------
// -- Funcion cruceSimon() para el modo de entrenamiento Simon --
// --------------------------------------------------------------

void cruceSimon(){
  Serial.println("Simon entro");
  bool error=false;
  
  // Pongo nivel actual en 0
  level=0;
  
  generateSequence();
  
  while(level<=sequenceMax && error==false){
    level++;
    Serial.print("Nivel: ");
    Serial.println(level);
    showSequence();
    error = getSequence();
  }
  if(error){
    endGame();
  }
}

void generateSequence()
{
  Serial.print("Generar Secuencia...");
  randomSeed(millis()); //in this way is really random!!!
  for (int i=0; i<sequenceMax; i++){
    sequence[i] = random(3);
    Serial.println(sequence[i]);
  } 
}

void showSequence(){
  Serial.print("Mostrar secuencia");
  for (int i=0; i<level; i++){
    showWhiteAll();
    tiempoEsperaLed(1000);
    Serial.println(sequence[i]);
    if(sequence[i]==0){
      turnOnRed(0);
      turnOnWhite(1);
      turnOnWhite(2);
    } else if(sequence[i]==1){
      turnOnWhite(0);
      turnOnGreen(1);
      turnOnWhite(2);
    } else if(sequence[i]==2){
      turnOnWhite(0);
      turnOnWhite(1);
      turnOnBlue(2);
    }
    FastLED.show();
    tiempoEsperaLed(1000);
  }
}

bool getSequence(){
  int sequenceRead=0;
  bool error=false;
  
  Serial.println("Obtener Secuencia...");
  
  while(sequenceRead<level && error==false){
    bool flag = false;
    
    ESP.wdtFeed();
    
    turnOnYellow(0);
    turnOnYellow(1);
    turnOnYellow(2);
    FastLED.show();
    
    Serial.println("Secuencia: ");
    Serial.println(sequenceRead);
    Serial.println("Nivel: ");
    Serial.println(level);
    
    int cruce=-1;
    while(flag==false){
      ESP.wdtFeed();
      if(digitalRead(SENSOR_PIN_1)==0){
        cruce=0;
        turnOnRed(0);
        flag=true;
      } else if(digitalRead(SENSOR_PIN_2)==0){
        cruce=1;
        turnOnGreen(1);
        flag=true;
      } else if(digitalRead(SENSOR_PIN_3)==0){
        cruce=2;
        turnOnBlue(2);
        flag=true;
      }
    }
    FastLED.show();
    tiempoEsperaLed(400);
    if(cruce==sequence[sequenceRead]){
      sequenceRead++;      
    } else {
      error=true;
      return error;
    }
  } 
  return error;
}

void turnOnGreen(int pos){
  //Serial.println("Turn on green");
  for(int i=0;i<NUM_LEDS;i++)
    LEDS_STRIPS[pos][i] = CRGB::Green;
}

void turnOnRed(int pos){  
  //Serial.println("Turn on red");  
  for(int i=0;i<NUM_LEDS;i++)
    LEDS_STRIPS[pos][i] = CRGB::Red;
}

void turnOnBlue(int pos){    
  //Serial.println("Turn on blue");
  for(int i=0;i<NUM_LEDS;i++)
    LEDS_STRIPS[pos][i] = CRGB::Blue;
}

void turnOnWhite(int pos){
  //Serial.println("Turn on white");
  for(int i=0;i<NUM_LEDS;i++)
    LEDS_STRIPS[pos][i] = CRGB::White;
}

void turnOnYellow(int pos){
  //Serial.println("Turn on yellow");
  for(int i=0;i<NUM_LEDS;i++)
    LEDS_STRIPS[pos][i] = CRGB::Yellow;
}

void turnOnBlack(int pos){
  //Serial.println("Turn on black");
  for(int i=0;i<NUM_LEDS;i++)
    LEDS_STRIPS[pos][i] = CRGB::Black;
}

void showWhiteAll(){
  turnOnWhite(0);
  turnOnWhite(1);
  turnOnWhite(2);
  FastLED.show();
}

void endGame(){
  for(int i=0;i<3;i++){
    turnOnRed(0);
    turnOnRed(1);
    turnOnRed(2);
    FastLED.show(); 
    tiempoEsperaLed(250);
    turnOnBlack(0);
    turnOnBlack(1);
    turnOnBlack(2);
    FastLED.show(); 
    tiempoEsperaLed(250);
  }
}

void tiempoEsperaLed(int stopTimeWait){
  int startTimeWait = millis();
  while(stopTimeWait>millis()-startTimeWait){
    ESP.wdtFeed();
  }
}

