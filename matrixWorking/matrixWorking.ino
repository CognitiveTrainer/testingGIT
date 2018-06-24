/******************************************************************************
** TEXT SCROLL SAMPLE FOR ALTAIRIS ESP8266-FC16 LIBRARY
**
** This sample will scroll text on the display. Available characters are
** defined in FC16_Font.h. In addition to the ASCII table, the following glyphs
** are defined (inspired by CP437, but not fully compatible):
** \x01 :-)
** \x02 :-(
** \x03 heart
** \x04 diamond
** \x05 club
** \x06 spade
** \x07 bullet
** \x08 empty bullet
** \x09 big bullet
** \x0A big bullet inverse
** \x0B checkbox empty
** \x0C checkbox cross
** \x0D checkbox check
** \x0E checkbox full
** \x0F sunny
** \x10 triangle right
** \x11 triangle left
** \x18 arrow up
** \x19 arrow down
** \x1A arrow right
** \x1B arrow left
** \x7F full-width space (8 pixels)
**
** Hardware setup (ESP8266 <-> FC16):
** 5V <-> VCC, GND <-> GND, D7 <-> DIN, D8 <-> CS, D5 <-> CLK
******************************************************************************/

#include <FC16.h>

const int csPin = D8;			// CS pin used to connect FC16
const int displayCount = 1;		// Number of displays; usually 4 or 8
const int scrollDelay = 1000;		// Scrolling speed - pause in ms

FC16 display = FC16(csPin, displayCount);

void setup() {
	display.begin();			// turn on display
	display.setIntensity(8);	// set medium brightness
	display.clearDisplay();		// turn all LED off
}

void loop() {
  displayCross();
  // Wait for 30 ms
  delay(scrollDelay);
  
  displayTick();
  // Wait for 30 ms
  delay(scrollDelay);  
}

void displayTick(){
  
  display.setColumn(0,0x60);
  display.setColumn(1,0x30);
  display.setColumn(2,0x18);
  display.setColumn(3,0x0C);
  display.setColumn(4,0x06);
  display.setColumn(5,0x03);
  display.setColumn(6,0x06);
  display.setColumn(7,0x0C); 

  display.update();
}

void displayCross(){
  
  display.setColumn(0,0x81);
  display.setColumn(1,0x42);
  display.setColumn(2,0x24);
  display.setColumn(3,0x18);
  display.setColumn(4,0x18);
  display.setColumn(5,0x24);
  display.setColumn(6,0x42);
  display.setColumn(7,0x81); 

  display.update();
}

