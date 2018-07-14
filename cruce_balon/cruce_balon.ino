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

// How many leds in your strip?
#define NUM_LEDS 40
#define NUM_STRIPS 3

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define SENSOR_PIN_1 16 // D0
#define SENSOR_PIN_2 5  // D1
#define SENSOR_PIN_3 4  // D2

#define STRIP_PIN_1 0   // D3
#define STRIP_PIN_2 2   // D4
#define STRIP_PIN_3 14  // D5

#define CLOCK_PIN   13

#define BRIGHTNESS  64

// Define the array of leds
CRGB LEDS_STRIPS[NUM_STRIPS][NUM_LEDS];

void setup() { 
      // 
      FastLED.addLeds<NEOPIXEL, STRIP_PIN_1>(LEDS_STRIPS[0], NUM_LEDS);
      FastLED.addLeds<NEOPIXEL, STRIP_PIN_2>(LEDS_STRIPS[1], NUM_LEDS);
      FastLED.addLeds<NEOPIXEL, STRIP_PIN_3>(LEDS_STRIPS[2], NUM_LEDS);

      FastLED.setBrightness( BRIGHTNESS );

      Serial.begin(115200);
}

void loop() { 

  Serial.print("Comienzo");
    
  randomSeed(millis());
  int numAleatorio = random(3);
  
  int timeStart = 0;
  int timeStop = 0;
  int timeTotal = 0;
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

void turnOnGreen(int pos){
  for(int i=0;i<NUM_LEDS;i++)
    LEDS_STRIPS[pos][i] = CRGB::Green;
}

void turnOnRed(int pos){    
  for(int i=0;i<NUM_LEDS;i++)
    LEDS_STRIPS[pos][i] = CRGB::Red;
}


