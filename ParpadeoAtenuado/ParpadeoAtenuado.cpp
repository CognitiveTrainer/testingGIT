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
  pinMode(pinLed, OUTPUT);
  
  _pinLed = pinLed;
  _delayParpadeo = delayParpadeo;
  _tCiclo = tCiclo;

}




void ParpadeoAtenuado::Atenuar()
{
  analogWrite(_pinLed, brightness);

  brightness = brightness + fadeAmount;
  
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  delay(_tCiclo);

}



void ParpadeoAtenuado::Parpadeo()
{
  digitalWrite(_pinLed, HIGH);
  delay(_delayParpadeo);
  digitalWrite(_pinLed, LOW);
  delay(_delayParpadeo);
  }

