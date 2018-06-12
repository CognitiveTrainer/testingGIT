#include <LiquidCrystal_I2C.h>

#include <Wire.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  // The begin call takes the width and height. This
  // Should match the number provided to the constructor.
  lcd.begin(16,2);
  lcd.init();
  
  //Prender luz de fondo
  lcd.backlight();

}

void loop() {
  
  lcdLeerJugador();
  delay(1000);
  lcdJugadorLeido();
  delay(1000);
}

void lcdLeerJugador(){
  //Limpiamos pantalla
  lcd.init();
  lcd.clear();
  
  lcd.home();

  lcd.print("Escanee tarjeta");
  
  // Move the cursor characters to the right and
  // zero characters down (line 1).
  lcd.setCursor(0, 1);
  
  lcd.print("de jugador...");
}

void lcdJugadorLeido(){
  //Limpiamos pantalla
  lcd.init();
  lcd.clear();
  
  lcd.home();

  lcd.print("Se leyo el");
  
  // Move the cursor characters to the right and
  // zero characters down (line 1).
  lcd.setCursor(0, 1);
  
  lcd.print("deportista: 12345");
}


