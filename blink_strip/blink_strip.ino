#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 120

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 4
#define CLOCK_PIN 13

#define BRIGHTNESS  64

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
      // Uncomment/edit one of the following lines for your leds arrangement.
      // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
      //FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
      //FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  	   FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
      // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<GW6205, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<GW6205_400, DATA_PIN, RGB>(leds, NUM_LEDS);
      
      // FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<P9813, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<APA102, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<DOTSTAR, RGB>(leds, NUM_LEDS);

      // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      FastLED.setBrightness( BRIGHTNESS );
}

void loop() { 
  // Turn the LED on, then pause

  
  turnOnGreen(0);
  randomSeed(millis());
  int numAleatorio = random(3);

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

  delay(500);
  
  /* Now turn the LED off, then pause
  for(int i=0;i<NUM_LEDS;i++)
  leds[i] = CRGB::Black;
  FastLED.show();
  delay(500);*/
}

void turnOnGreen(int pos){
  int posStart[3]={0,40,80};
  int posEnd[3]={40,80,120};    
  for(int i=posStart[pos];i<posEnd[pos];i++)
    leds[i] = CRGB::Green;
}

void turnOnRed(int pos){
  int posStart[3]={0,40,80};
  int posEnd[3]={40,80,120};    
  for(int i=posStart[pos];i<posEnd[pos];i++)
    leds[i] = CRGB::Red;
}


