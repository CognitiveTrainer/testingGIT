#include <LiquidCrystal_I2C.h>

#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

  Serial.begin(115200);

  //Use predefined PINS consts

  Wire.begin(D2, D1);

 // The begin call takes the width and height. This
 // Should match the number provided to the constructor.
  lcd.begin(16,2);

  //Prender luz de fondo

  lcd.backlight();

  lcd.home();

  lcd.print("Escanee tarjeta");
  
  // Move the cursor characters to the right and
  // zero characters down (line 1).
  lcd.setCursor(0, 1);
  
  lcd.print("de jugador...");

}

void loop() { 
  // do nothing here 
}

