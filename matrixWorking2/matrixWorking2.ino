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
const int displayCount = 3;		// Number of displays; usually 4 or 8
const int scrollDelay = 1000;		// Scrolling speed - pause in ms
const int cantMatriz =3;
const int cantColumna =8;
int numeroColumna[3][8];

FC16 display = FC16(csPin, displayCount);

void setup() {
	display.begin();			// turn on display
	display.setIntensity(8);	// set medium brightness
	display.clearDisplay();		// turn all LED off
  int auxiliar=0;
  for(int i=0;i<3;i++){
    for (int j=0;j<8;j++){
      numeroColumna[i][j]=auxiliar;
      auxiliar++;
    }
  }
}

void loop() {

  randomSeed(millis());
  int numAleatorio = random(3);


  if(numAleatorio==0){
    displayTick(0);
    displayDiamond(1);
    displayDiamondInvert(2);
  } else if(numAleatorio==1){
    displayCross(0);
    displayDiamond(1);
    displayDiamondInvert(2);
  } else if(numAleatorio==2){
    displayCross(0);
    displayDiamond(1);
    displayDiamondInvert(2);
  }

  

  
  delay(scrollDelay);  
}

void displayTick(int pos){
  
  display.setColumn(numeroColumna[pos][0],0x60);
  display.setColumn(numeroColumna[pos][1],0x30);
  display.setColumn(numeroColumna[pos][2],0x18);
  display.setColumn(numeroColumna[pos][3],0x0C);
  display.setColumn(numeroColumna[pos][4],0x06);
  display.setColumn(numeroColumna[pos][5],0x03);
  display.setColumn(numeroColumna[pos][6],0x06);
  display.setColumn(numeroColumna[pos][7],0x0C); 

  display.update();
}

void displayCross(int pos){
  
  display.setColumn(numeroColumna[pos][0],0x81);
  display.setColumn(numeroColumna[pos][1],0x42);
  display.setColumn(numeroColumna[pos][2],0x24);
  display.setColumn(numeroColumna[pos][3],0x18);
  display.setColumn(numeroColumna[pos][4],0x18);
  display.setColumn(numeroColumna[pos][5],0x24);
  display.setColumn(numeroColumna[pos][6],0x42);
  display.setColumn(numeroColumna[pos][7],0x81); 

  
  display.update();
}


void displaySquare(int pos){
  
  display.setColumn(numeroColumna[pos][0],0xFF);
  display.setColumn(numeroColumna[pos][1],0xFF);
  display.setColumn(numeroColumna[pos][2],0xC3);
  display.setColumn(numeroColumna[pos][3],0xC3);
  display.setColumn(numeroColumna[pos][4],0xC3);
  display.setColumn(numeroColumna[pos][5],0xC3);
  display.setColumn(numeroColumna[pos][6],0xFF);
  display.setColumn(numeroColumna[pos][7],0xFF); 

  display.update();
}

void displayAlien(int pos){
  
  display.setColumn(numeroColumna[pos][0],0x18);
  display.setColumn(numeroColumna[pos][1],0x3C);
  display.setColumn(numeroColumna[pos][2],0x66);
  display.setColumn(numeroColumna[pos][3],0xDB);
  display.setColumn(numeroColumna[pos][4],0xDB);
  display.setColumn(numeroColumna[pos][5],0x66);
  display.setColumn(numeroColumna[pos][6],0x3C);
  display.setColumn(numeroColumna[pos][7],0x18); 

  display.update();
}

void displayDiamond(int pos){
  
  display.setColumn(numeroColumna[pos][0],0x00);
  display.setColumn(numeroColumna[pos][1],0x66);
  display.setColumn(numeroColumna[pos][2],0x66);
  display.setColumn(numeroColumna[pos][3],0x06);
  display.setColumn(numeroColumna[pos][4],0x06);
  display.setColumn(numeroColumna[pos][5],0x66);
  display.setColumn(numeroColumna[pos][6],0x66);
  display.setColumn(numeroColumna[pos][7],0x00); 

  display.update();
}

void displayCT(int pos){
  
  display.setColumn(numeroColumna[pos][0],0x08);
  display.setColumn(numeroColumna[pos][1],0x0F);
  display.setColumn(numeroColumna[pos][2],0x0F);
  display.setColumn(numeroColumna[pos][3],0x08);
  display.setColumn(numeroColumna[pos][4],0x90);
  display.setColumn(numeroColumna[pos][5],0x90);
  display.setColumn(numeroColumna[pos][6],0x90);
  display.setColumn(numeroColumna[pos][7],0xF0); 
  
  display.update();
}

void displayClear(int pos){
  
  display.setColumn(numeroColumna[pos][0],0x00);
  display.setColumn(numeroColumna[pos][1],0x00);
  display.setColumn(numeroColumna[pos][2],0x00);
  display.setColumn(numeroColumna[pos][3],0x00);
  display.setColumn(numeroColumna[pos][4],0x00);
  display.setColumn(numeroColumna[pos][5],0x00);
  display.setColumn(numeroColumna[pos][6],0x00);
  display.setColumn(numeroColumna[pos][7],0x00); 
  
  display.update();
}

void displayAll(int pos){
  
  display.setColumn(numeroColumna[pos][0],0xFF);
  display.setColumn(numeroColumna[pos][1],0xFF);
  display.setColumn(numeroColumna[pos][2],0xFF);
  display.setColumn(numeroColumna[pos][3],0xFF);
  display.setColumn(numeroColumna[pos][4],0xFF);
  display.setColumn(numeroColumna[pos][5],0xFF);
  display.setColumn(numeroColumna[pos][6],0xFF);
  display.setColumn(numeroColumna[pos][7],0xFF); 
  
  display.update();
}

void displayTickInvert(int pos){
  
  display.setColumn(numeroColumna[pos][0],0x9F);
  display.setColumn(numeroColumna[pos][1],0xCF);
  display.setColumn(numeroColumna[pos][2],0xE7);
  display.setColumn(numeroColumna[pos][3],0xF3);
  display.setColumn(numeroColumna[pos][4],0xF9);
  display.setColumn(numeroColumna[pos][5],0xFC);
  display.setColumn(numeroColumna[pos][6],0xF9);
  display.setColumn(numeroColumna[pos][7],0xF3); 

  display.update();
}

void displayCrossInvert(int pos){
  
  display.setColumn(numeroColumna[pos][0],0x7E);
  display.setColumn(numeroColumna[pos][1],0xBD);
  display.setColumn(numeroColumna[pos][2],0xDB);
  display.setColumn(numeroColumna[pos][3],0xE7);
  display.setColumn(numeroColumna[pos][4],0xE7);
  display.setColumn(numeroColumna[pos][5],0xDB);
  display.setColumn(numeroColumna[pos][6],0xBD);
  display.setColumn(numeroColumna[pos][7],0x7E); 

  display.update();
}

void displaySquareInvert(int pos){
  
  display.setColumn(numeroColumna[pos][0],0x00);
  display.setColumn(numeroColumna[pos][1],0x00);
  display.setColumn(numeroColumna[pos][2],0x3C);
  display.setColumn(numeroColumna[pos][3],0x3C);
  display.setColumn(numeroColumna[pos][4],0x3C);
  display.setColumn(numeroColumna[pos][5],0x3C);
  display.setColumn(numeroColumna[pos][6],0x00);
  display.setColumn(numeroColumna[pos][7],0x00); 

  display.update();
}

void displayAlienInvert(int pos){
  
  display.setColumn(numeroColumna[pos][0],0xE7);
  display.setColumn(numeroColumna[pos][1],0xC3);
  display.setColumn(numeroColumna[pos][2],0x99);
  display.setColumn(numeroColumna[pos][3],0x24);
  display.setColumn(numeroColumna[pos][4],0x24);
  display.setColumn(numeroColumna[pos][5],0x99);
  display.setColumn(numeroColumna[pos][6],0xC3);
  display.setColumn(numeroColumna[pos][7],0xE7); 

  display.update();
}

void displayDiamondInvert(int pos){
  
  display.setColumn(numeroColumna[pos][0],0xFF);
  display.setColumn(numeroColumna[pos][1],0x99);
  display.setColumn(numeroColumna[pos][2],0x99);
  display.setColumn(numeroColumna[pos][3],0xF9);
  display.setColumn(numeroColumna[pos][4],0xF9);
  display.setColumn(numeroColumna[pos][5],0x99);
  display.setColumn(numeroColumna[pos][6],0x99);
  display.setColumn(numeroColumna[pos][7],0xFF); 

  display.update();
}

