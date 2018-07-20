//------------------------------------------------------------------------------------
// Librerias necesarias
//------------------------------------------------------------------------------------
#include <EEPROM.h>         // Necesaria para guardar los datos al apagar
#include <ESP8266WiFi.h>    // NODEMCU
#include <dummy.h>

// Definimos librerias y variables para tiras led (FastLED)
#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ESP8266_D1_PIN_ORDER
#include "FastLED.h"

  const char* ssid = "Fedeb";
  const char* password = "maife3220";

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

#define LED0 LED_BUILTIN         // WIFI Module LED

// Define the array of leds
CRGB LEDS_STRIPS[NUM_STRIPS][NUM_LEDS];

// Variables para cualquier entrenamiento
int timeStart = 0;
int timeStop = 0;

// Variables para entrenamiento Simon
const int sequenceMax = 5;
int sequence[sequenceMax];
int sequenceUser[sequenceMax];
const int timeTotalShow=2000;
int level;

//------------------------------------------------------------------------------------
// Modulo WIFI Puertos y Roles
//------------------------------------------------------------------------------------
/* Este modulo funciona como un cliente
   Se va a conectar al servidor Wifi con la IP 192.168.1.80
*/
int             CognServerPort  = 8080;
IPAddress       CognServer(192, 168, 43, 209);
WiFiClient      CognClient;

void setup() { 
      //       
      FastLED.addLeds<NEOPIXEL, STRIP_PIN_1>(LEDS_STRIPS[0], NUM_LEDS);
      FastLED.addLeds<NEOPIXEL, STRIP_PIN_2>(LEDS_STRIPS[1], NUM_LEDS);
      FastLED.addLeds<NEOPIXEL, STRIP_PIN_3>(LEDS_STRIPS[2], NUM_LEDS);

      FastLED.setBrightness( BRIGHTNESS );

      Serial.begin(115200);

      // Starting To Connect --------------------------------------------------
      if (WiFi.status() == WL_CONNECTED)
      {
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);
        delay(50);
      }

      /* in this part it should load the ssid and password
     from eeprom they try to connect using them */

      WiFi.mode(WIFI_STA);            // To Avoid Broadcasting An SSID
      WiFi.begin(ssid, password);      // The SSID That We Want To Connect To
      
      // Printing Message For User That Connetion Is On Process ---------------
      Serial.println("!--- Connecting To " + WiFi.SSID() + " ---!");
      
      // WiFi Connectivity ----------------------------------------------------
      CheckWiFiConnectivity();        // Checking For Connection
      
      // Stop Blinking To Indicate Connected ----------------------------------
      digitalWrite(LED0, !HIGH);
      Serial.println("!-- Client Device Connected --!");
      
      // Printing IP Address --------------------------------------------------
      Serial.println("Connected To      : " + String(WiFi.SSID()));
      Serial.println("Signal Strenght   : " + String(WiFi.RSSI()) + " dBm");
      Serial.print  ("Server IP Address : ");
      Serial.println(CognServer);
      Serial.print  ("Server Port Num   : ");
      Serial.println(CognServerPort);
      // Printing MAC Address
      Serial.print  ("Device MC Address : ");
      Serial.println(String(WiFi.macAddress()));
      // Printing IP Address
      Serial.print  ("Device IP Address : ");
      Serial.println(WiFi.localIP());

      // Conecting The Device As A Client -------------------------------------
      CognRequest();

      showCyanAll();
      
}

void loop() { 
  CheckWiFiConnectivity();

  if (!CognClient.connected()) {
    CognRequest();
  }
  
  if (CognClient.available() > 0) {
    String tipoRep = CognClient.readStringUntil('\r');
    String tipo = getValue(tipoRep,',',0);
    String repeticion = getValue(tipoRep,',',1);
      Serial.println(tipo);
      Serial.println(repeticion);
    if(tipo.equals("Simon")){
      cruceSimon(repeticion.toInt());
      tiempoEsperaLed(400);
      showCyanAll();
    } else if(tipo.equals("Simple")){
      cruceSimple(repeticion.toInt());
      tiempoEsperaLed(400);
      showCyanAll();
    }
  }
}

// ----------------------------------------------------------------
// -- Funcion cruceSimple() para el modo de entrenamiento Simple --
// ----------------------------------------------------------------

void cruceSimple(int repeticion){
  Serial.println("Comienzo");
  Serial.println(repeticion);
  int error = 0;
  int timeTotal = 0;
  for(int i=0; i<repeticion; i++){
    ESP.wdtFeed();
    
    randomSeed(millis());
    int numAleatorio = random(3);
    
    int cruce = -1;
    int correcto = 0;
  
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
    timeTotal = timeTotal + (millis() - timeStart);
    
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
  }

  Serial.print("Error total: ");
  Serial.println(error);
  
  retornoErrorTiempo(error,timeTotal);
  
  delay(500);
}

// --------------------------------------------------------------
// -- Funcion cruceSimon() para el modo de entrenamiento Simon --
// --------------------------------------------------------------

void cruceSimon(int repeticion){
  Serial.println("Simon entro");
  bool error=false;
  int timeTotal = 0;
  // Pongo nivel actual en 0
  level=0;
  
  generateSequence();
  timeStart=millis();
  while(level<repeticion && error==false){
    level++;
    Serial.print("Nivel: ");
    Serial.println(level);
    showSequence();
    error = getSequence();
  }

  timeTotal=millis()-timeStart;
  
  if(error){
    retornoErrorTiempo(1, timeTotal);
    endGame();
  }
  else{
    retornoErrorTiempo(0, timeTotal);
    endGameCorrect();
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
void turnOnCyan(int pos){
  //Serial.println("Turn on Cyan");
  for(int i=0;i<NUM_LEDS;i++)
    LEDS_STRIPS[pos][i] = CRGB::Cyan;
}

void showWhiteAll(){
  turnOnWhite(0);
  turnOnWhite(1);
  turnOnWhite(2);
  FastLED.show();
}

void showCyanAll(){
  turnOnCyan(0);
  turnOnCyan(1);
  turnOnCyan(2);
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

void endGameCorrect(){
  for(int i=0;i<3;i++){
    turnOnGreen(0);
    turnOnGreen(1);
    turnOnGreen(2);
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

//====================================================================================

void CheckWiFiConnectivity()
{
  while (WiFi.status() != WL_CONNECTED)
  {
    for (int i = 0; i < 10; i++)
    {
      digitalWrite(LED0, !HIGH);
      delay(250);
      digitalWrite(LED0, !LOW);
      delay(250);
      Serial.print(".");
    }
    Serial.println("");
  }
}


//====================================================================================

void CognRequest()
{
  // First Make Sure You Got Disconnected
  CognClient.stop();

  // If Sucessfully Connected Send Connection Message
  if (CognClient.connect(CognServer, CognServerPort))
  {
    Serial.println("Conectado");
    CognClient.print ("Conectado");
  }
}

//====================================================================================


void retornoErrorTiempo(int errorRet, int tiempoRet){
  CheckWiFiConnectivity();
  Serial.println(errorRet);
  Serial.println(tiempoRet);
  String retorno = "";
  retorno = retorno + errorRet;
  retorno = retorno + ",";
  retorno = retorno + tiempoRet;
  Serial.println(retorno);
  CognClient.print (retorno);
  CognClient.flush();
}


//====================================================================================

  String getValue(String data, char separator, int index)
  {
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length()-1;
  
    for(int i=0; i<=maxIndex && found<=index; i++){
      if(data.charAt(i)==separator || i==maxIndex){
          found++;
          strIndex[0] = strIndex[1]+1;
          strIndex[1] = (i == maxIndex) ? i+1 : i;
      }
    }
  
    return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
  }
