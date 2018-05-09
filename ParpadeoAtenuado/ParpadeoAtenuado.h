/*
  Test.h - Test library for Wiring - description
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// ensure this library description is only included once
#ifndef ParpadeoAtenuado_h
#define ParpadeoAtenuado_h

// include types & constants of Wiring core API
#include "Arduino.h"

// library interface description
class ParpadeoAtenuado
{
  // user-accessible "public" interface
  public:
    ParpadeoAtenuado(int pinLed, int delayParpadeo, int tCiclo);
    void Atenuar();
    void Parpadeo();
	
  private:
    int _pinLed;
    int _delayParpadeo;
    int _tCiclo;
    
    int brightness = 0;    // how bright the LED is
    int fadeAmount = 5;    // how many points to fade the LED by
};

#endif


