/*
  Test.h - Test library for Wiring - implementation
  Copyright (c) 2006 John Doe.  All right reserved.
*/

#include "Arduino.h"

#include "ParpadeoAtenuado.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances




ParpadeoAtenuado::ParpadeoAtenuado(int pinLed, int delayParpadeo, int tCiclo)
{
  // initialize this instance's variables
  //_pinLed = pinLed;
  _delayParpadeo = delayParpadeo;
  _tCiclo = tCiclo;

  int brightness = 0;    // how bright the LED is
  int fadeAmount = 5;    // how many points to fade the LED by

  // do whatever is required to initialize the library
  pinMode(_pinLed, OUTPUT);
}




void ParpadeoAtenuado::Atenuar(void)
{
  analogWrite(_pinLed, brightness);

  brightness = brightness + fadeAmount;
  
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  delay(tCiclo);

}



void ParpadeoAtenuado::Parpadeo(void)
{
  digitalWrite(_pinLed, HIGH);
  delay(delayParpadeo);
  digitalWrite(_pinLed, LOW);
  delay(delayParpadeo);
}

