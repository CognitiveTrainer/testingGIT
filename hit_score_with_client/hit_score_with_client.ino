//------------------------------------------------------------------------------------
// Librerias necesarias
//------------------------------------------------------------------------------------
  #include <EEPROM.h>         // Necesaria para guardar los datos al apagar
  #include <ESP8266WiFi.h>    // NODEMCU
  #include <dummy.h>
  
  #include <FC16.h>

//================= DISPLAY VARIABLES
  const int csPin = D8;      // CS pin used to connect FC16
  const int displayCount = 3;   // Number of displays; usually 4 or 8
  const int scrollDelay = 1000;   // Scrolling speed - pause in ms
  const int cantMatriz = 3;
  const int cantColumna = 8;
  int numeroColumna[3][8];

  FC16 display = FC16(csPin, displayCount);

  const char* ssid = "HUAWEI-IoT";
  const char* password = "ORTWiFiIoT";

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

// Definimos pines de sensores y tiras
  #define SENSOR_PIN_1 16 // D0
  #define SENSOR_PIN_2 5  // D1
  #define SENSOR_PIN_3 4  // D2
  
  #define LED0 LED_BUILTIN         // WIFI Module LED

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
  IPAddress       CognServer(192, 168, 1, 102);
  WiFiClient      CognClient;
  
  void setup() { 
    Serial.begin(115200);
  
    display.begin();      // turn on display
    display.setIntensity(8);  // set medium brightness
    display.clearDisplay();   // turn all LED off
    
    int auxiliar=0;
    for(int i=0;i<3;i++){
      for (int j=0;j<8;j++){
        numeroColumna[i][j]=auxiliar;
        auxiliar++;
      }
    }
    
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
    displayCT(0);
    displayCT(1);
    displayCT(2);
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
        hitSimon(repeticion.toInt());
        tiempoEsperaLed(400);
  
      } else if(tipo.equals("Simple")){
        hitSimple(repeticion.toInt());
        tiempoEsperaLed(400);
  
      }
    }
  }

// ----------------------------------------------------------------
// -- Funcion hitSimple() para el modo de entrenamiento Simple --
// ----------------------------------------------------------------

  void hitSimple(int repeticion){
    Serial.println("Comienzo");
    Serial.println(repeticion);
    int error = 0;
    int timeTotal = 0;
    for(int i=0; i<repeticion; i++){
      ESP.wdtFeed();
      
      randomSeed(millis());
      int numAleatorio = random(3);
      
      int golpe = -1;
      int correcto = 0;
    
      Serial.print("Numero aleatorio: ");
      Serial.println(numAleatorio);
      
      if(numAleatorio==0){
        displayTick(0);
        displayCross(1);
        displayCross(2);
      } else if(numAleatorio==1){
        displayCross(0);
        displayTick(1);
        displayCross(2);
      } else if(numAleatorio==2){
        displayCross(0);
        displayCross(1);
        displayTick(2);
      }
      
      // Comenzamos a contar el tiempo
      timeStart = millis();
    
      golpe=-1;
      
      // Vemos por cual de los agujeros cruzó la pelota
      while(golpe<0){
        ESP.wdtFeed();
        if(digitalRead(SENSOR_PIN_1)==1){
          golpe=0;
        } else if(digitalRead(SENSOR_PIN_2)==1){
          golpe=1;
        } else if(digitalRead(SENSOR_PIN_3)==1){
          golpe=2;
        }
      }
      displayCT(golpe);
      
      tiempoEsperaLed(400); 
      // Guardamos el tiempo que tardó en finalizar el juego
      timeTotal = timeTotal + (millis() - timeStart);
      
      if(golpe== numAleatorio){
        correcto++;
      } else{
        error++;
      }
      
      Serial.print("Golpe: ");
      Serial.println(golpe);
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
// -- Funcion hitSimon() para el modo de entrenamiento Simon --
// --------------------------------------------------------------

void hitSimon(int repeticion){
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

  displayCT(0);
  displayCT(1);
  displayCT(2);
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
    displayCT(0);
    displayCT(1);
    displayCT(2);
    tiempoEsperaLed(1000);
    Serial.println(sequence[i]);
    if(sequence[i]==0){
      displayDiamond(0);
      displayClear(1);
      displayClear(2);
    } else if(sequence[i]==1){
      displayClear(0);
      displaySquare(1);
      displayClear(2);
    } else if(sequence[i]==2){
      displayClear(0);
      displayClear(1);
      displayAlien(2);
    }
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
    
    displayDiamond(0);
    displaySquare(1);
    displayAlien(2);
    
    Serial.println("Secuencia: ");
    Serial.println(sequenceRead);
    Serial.println("Nivel: ");
    Serial.println(level);
    
    int golpe=-1;
    while(flag==false){
      ESP.wdtFeed();
      if(digitalRead(SENSOR_PIN_1)==1){
        golpe=0;
        displayDiamondInvert(0);
        flag=true;
      } else if(digitalRead(SENSOR_PIN_2)==1){
        golpe=1;
        displaySquareInvert(1);
        flag=true;
      } else if(digitalRead(SENSOR_PIN_3)==1){
        golpe=2;
        displayAlienInvert(2);
        flag=true;
      }
    }
    tiempoEsperaLed(400);
    if(golpe==sequence[sequenceRead]){
      sequenceRead++;      
    } else {
      error=true;
      return error;
    }
  } 
  return error;
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

//=================================================

void endGame(){
  for(int i=0;i<3;i++){
    displayCrossInvert(0);
    displayCrossInvert(1);
    displayCrossInvert(2);
    
    tiempoEsperaLed(250);
    
    displayClear(0);
    displayClear(1);
    displayClear(2);
    
    tiempoEsperaLed(250);
  }
}

void endGameCorrect(){
  for(int i=0;i<3;i++){
    displayTickInvert(0);
    displayTickInvert(1);
    displayTickInvert(2);
    
    tiempoEsperaLed(250);
    
    displayClear(0);
    displayClear(1);
    displayClear(2);
    
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
    Serial.println("Imprimiendo: Conectado");
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
