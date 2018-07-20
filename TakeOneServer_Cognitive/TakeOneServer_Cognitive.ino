//---------- INCLUDES ---------- //
//------------ LCD -------------
  #include <LiquidCrystal_I2C.h>
  #include <Wire.h>
  LiquidCrystal_I2C lcd(0x3F, 16, 2);

//---------- NODE MCU ---------- 
  #include <ESP8266WiFi.h>
  #include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3
  #include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson/releases/tag/v5.0.7

//---------- PN532 - RFID  -------------

  #if 0
  #include <SPI.h>
  #include <PN532_SPI.h>
  #include <PN532.h>
  #include <NfcAdapter.h>

  PN532_SPI pn532spi(SPI, 10);
  NfcAdapter nfc = NfcAdapter(pn532spi);
  #else
  
  #include <PN532_I2C.h>
  #include <PN532.h>
  #include <NfcAdapter.h>
  
  PN532_I2C pn532_i2c(Wire);
  NfcAdapter nfc = NfcAdapter(pn532_i2c);
  //I2C address of the PN532 chip.  
  #define PN532_I2C_ADDR 0x24
  #endif
  
//------------------------------------------------------------------------------------
// Variables de BUTTONS
//------------------------------------------------------------------------------------
  int           ButtonState;
  int           LastButtonState   = LOW;
  int           LastDebounceTime  = 0;
  int           DebounceDelay     = 50;
  const String  ButtonColor       = "RED";
  
//------------------------------------------------------------------------------------
  // Define I/O Pins
  #define     LED0      2           // WIFI Module LED
  #define     BUTTON    D0          // Connectivity ReInitiate Button
  
//------------------------------------------------------------------------------------
  #define     MAXSC     3           // MAXIMUM NUMBER OF CLIENTS

//=========  VARIABLES =======
  String rfid="0";
  int publishInterval = 1000; // 5 seconds//Send adc every 5sc
  long lastPublishMillis;

//=========  NODEMCU CONNECTION TO IBM =======
  const char* ssid = "HUAWEI-IoT";
  const char* password = "ORTWiFiIoT";
  
  #define ORG "0nfqy4"
  #define DEVICE_TYPE "NodeMCU"
  #define DEVICE_ID "Master"
  #define TOKEN "8feJ33h_!e@!zSEkL+"

//========= IBM IoT PLATFORM CONNECTION ======

  char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
  char authMethod[] = "use-token-auth";
  char token[] = TOKEN;
  char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
  
  const char eventTopic[] = "iot-2/evt/status/fmt/json";
  const char cmdTopic[] = "iot-2/cmd/rfid/fmt/json";
  
//========= ESP8266 WiFi VARIABLES =======
  WiFiServer  CognServer(8080);      // THE SERVER AND THE PORT NUMBER
  WiFiClient  CognClient[MAXSC];     // THE SERVER CLIENTS (Devices)
  WiFiClient  wifiClient;
//========= INFORMATION FROM IBM ======= 
  void callback(char* topic, byte* payload, unsigned int payloadLength) {
    Serial.print("Length: ");
    Serial.println(payloadLength);
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < payloadLength; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
  
    char json[payloadLength + 1];
    strncpy (json, (char*)payload, payloadLength);
    json[payloadLength] = '\0';
    
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& data = jsonBuffer.parseObject((char*)json);
  
    if (!data.success())
    {
      Serial.println("parseObject() failed");
      return;
    }
    
    // Obtenemos datos del payload recibido y lo pasamos a variables
    String rfidStatus = String((const char*)data["rfid"]);
    Serial.print("Estado rfid:");
    Serial.println(rfidStatus);
  
    String EntrenaID = String((const char*)data["EntrenaID"]);
    
    if (rfidStatus == "read") {
      Serial.print("Entre al if");
      lcdLeerJugador();
      rfid = leerTAG();
      bool flag=true;
      while(flag) {
        //Serial.println("Entre al while");
        if(millis() - lastPublishMillis > publishInterval){
          //Serial.println("Entre al if");
          rfid.replace(" ","");
          publishRFID();
          lastPublishMillis = millis();
          lcdJugadorLeido();
          int tiempoEspera = millis();
          int tresSegundos = 3000;
          while(millis() - tiempoEspera > tresSegundos){
              
          }
          lcdEsperandoEntrenamiento();
          flag=false;
        }  
      } 
    }
    
    if (!EntrenaID.equals("")){
      String toggleHit = String((const char*)data["toggleHit"]);
      String tipoHit = String((const char*)data["tipoHit"]);
      String repHit = String((const char*)data["repHit"]);
      String toggleCruce = String((const char*)data["toggleCruce"]);
      String tipoCruce = String((const char*)data["tipoCruce"]);
      String repCruce = String((const char*)data["repCruce"]);
      String serie = String((const char*)data["serie"]);

      for(int i=0; i<serie.toInt(); i++){
        if(toggleCruce.equals("1")){
          startCruce(EntrenaID, tipoCruce, repCruce);
        }
        if(toggleHit.equals("1")){
          startHit(EntrenaID, tipoHit, repHit);
        }
      }
    }
  }
  
  PubSubClient client(server, 1883, callback, wifiClient);
  
//====================================================================================

  void setup()
  {
    // Setting The Serial Port
    Serial.begin(115200);           // Computer Communication
    client.setCallback(callback);

    // The begin call takes the width and height. This
    // Should match the number provided to the constructor.
    lcd.begin(16,2);
    lcd.init();
    
    //Prender luz de fondo
    lcd.backlight();
    
    lcdBienvenidoCT();
    nfc.begin();
    
    // Setting The Mode Of Pins
    pinMode(LED0, OUTPUT);          // WIFI OnBoard LED Light
    pinMode(BUTTON, INPUT_PULLUP);  // Initiate Connectivity
    
    // Print Message Of I/O Setting Progress
    Serial.println();
    Serial.println("I/O Pins Modes Set .... Done");

    ConnectWiFi();
    mqttConnect();
    
    // Starting Server
    CognServer.begin();
    CognServer.setNoDelay(true);
    Serial.println("Server Started");
  }

//====================================================================================

  void loop()
  { 
    // Checking For Available Clients
    AvailableClients();
    
    if (!client.loop()) {
      mqttConnect();
    }
  }

//====================================================================================

  void AvailableClients()
  {   
    if (CognServer.hasClient())
    {
      // Read LED0 Switch To Low If High.
      if(digitalRead(LED0) == HIGH) digitalWrite(LED0, LOW);
      
      for(uint8_t i = 0; i < MAXSC; i++)
      {
        //find free/disconnected spot
        if (!CognClient[i] || !CognClient[i].connected())
        {
          // Checks If Previously The Client Is Taken
          if(CognClient[i])
          {
            CognClient[i].stop();
          }
          // Checks If Clients Connected To The Server
          if(CognClient[i] = CognServer.available())
          {
            Serial.println("New Client: " + String(i));
            String tempRetorno = AvailableMessage();
            Serial.println(tempRetorno);
          }
          // Continue Scanning
          continue;
        }
      }
      //no free/disconnected spot so reject
      WiFiClient CognClient = CognServer.available();
      CognClient.stop();
    }
    else
    {
      // This LED Blinks If No Clients Where Available
      digitalWrite(LED0, HIGH);
      delay(250);
      digitalWrite(LED0, LOW);
      delay(250);
    }
  }

//====================================================================================

  String AvailableMessage()
  { 
    ESP.wdtFeed();
    //Serial.println("Available messages?");
    //check clients for data
    for(uint8_t i = 0; i < MAXSC; i++)
    {
      if (CognClient[i] && CognClient[i].connected() && CognClient[i].available())
      {
        Serial.print("Cliente: ");
        Serial.println(i);
        Serial.print("Available: ");
        Serial.println(CognClient[i].available());
        while(CognClient[i].available())
          {
            Serial.print("Sigo en available: ");
            Serial.println(CognClient[i].available());
            String Message = CognClient[i].readStringUntil('\r');
            CognClient[i].flush();
            Serial.println(Message);
            return Message;
          }
      } 
    }
    return "False";
  }

//====================================================================================

  void ConnectWiFi(){
    // Starting To Connect --------------------------------------------------
    if(WiFi.status() == WL_CONNECTED)
    {
      WiFi.disconnect();
      WiFi.mode(WIFI_OFF);
      delay(50);
    }

    /* in this part it should load the ssid and password 
     * from eeprom they try to connect using them */

    IPAddress ip(192, 168, 1, 102);            // IP address of the server
    IPAddress gateway(192,168,43,1);           // gateway of your network
    IPAddress subnet(255,255,255,0);          // subnet mask of your network    
    
    WiFi.mode(WIFI_STA);            // To Avoid Broadcasting An SSID
    WiFi.config(ip,gateway,subnet);
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
    // Printing MAC Address
    Serial.print  ("Device MC Address : ");
    Serial.println(String(WiFi.macAddress()));
    // Printing IP Address
    Serial.print  ("Device IP Address : ");
    Serial.println(WiFi.localIP());
  }

//====================================================================================

  void CheckWiFiConnectivity()
  {
    while(WiFi.status() != WL_CONNECTED)
    {
      for(int i=0; i < 10; i++)
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

  void ReadButton()
  {
    // Reading The Button
    int reading = digitalRead(BUTTON);
  
    // If It Doest Match The Previous State
    if (reading != LastButtonState)
    {
      LastDebounceTime = millis();
    }
  
    // To Iliminate Debounce
    if ((millis() - LastDebounceTime) > DebounceDelay)
    {
      if (reading != ButtonState)
      {
        ButtonState = reading;
  
        if (ButtonState == LOW)
        {
          String pepe = AvailableMessage();
          
        }
      }
    }
  
    // Last Button State Concidered
    LastButtonState = reading;
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


//=========  mqttConnect() used to connect to IBM IoT Platform =========

  void mqttConnect() {
    if (!!!client.connected()) {
      Serial.print("Reconnecting MQTT client to "); Serial.println(server);
      while (!!!client.connect(clientId, authMethod, token)) {
        Serial.print(".");
        delay(500);
      }
      if (client.subscribe(cmdTopic)) {
        Serial.println("subscribe to responses OK");
      } else {
        Serial.println("subscribe to responses FAILED");
      }
      Serial.println();
    }
  }

//=========  publishRFID() used to send data to IBM IoT Platform ========= 

  void publishRFID() {
    if (!client.loop()) {
      mqttConnect();
    }
    
    Serial.println("Entre al publishRFID");
    rfid.trim();
    String payload = "{\"TipoCarga\":\"rfid\", \"rfid\":\""+rfid+"\"}";
  
    Serial.print("Sending payload: "); Serial.println(payload);
    
    int pepe = client.publish(eventTopic, (char*) payload.c_str());

    Serial.println(pepe);
    
    if (pepe) {
      Serial.println("Publish OK");
    } else {
      Serial.println("Publish FAILED");
    }
  }

//=========  publishCruceTiempoError() used to send data to IBM IoT Platform ========= 

  void publishCruceTiempoError(String EntrenaID, String error, String tiempo) {
    CheckWiFiConnectivity();
    
    if (!client.loop()) {
      mqttConnect();
    }
    
    Serial.println("Entre al publishCruceTiempoError");
    
    String payload = "{\"TipoCarga\":\"Entrenamiento\", \"EntrenaID\":\""+EntrenaID+"\", \"error\":\""+error+"\", \"tiempo\":\""+tiempo+"\"}";
    
    Serial.print("Sending payload: "); Serial.println(payload);
    
    if (client.publish(eventTopic, (char*) payload.c_str())) {
      Serial.println("Publish OK");
    } else {
      Serial.println("Publish FAILED");
    }
  }

//=============================================//
//=============================================//

//---------- lcdLeerJugador used to show text on the display
  void lcdJugadorLeido(){
    //Limpiamos pantalla
    lcd.init();
    lcd.clear();
    
    lcd.home();
  
    lcd.print("Deportista");
    
    // Move the cursor characters to the right and
    // zero characters down (line 1).
    lcd.setCursor(0, 1);
    
    lcd.print(rfid);
  }

//---------- lcdLeerJugador used to show text on the display 

  void lcdLeerJugador(){
    //Limpiamos pantalla
    lcd.init();
    lcd.clear();
    
    lcd.home();
  
    lcd.print("Escanee tarjeta");
    
    // Move the cursor characters to the right and
    // zero characters down (line 1).
    lcd.setCursor(0, 1);
    
    lcd.print("de deportista...");
  }

//---------- lcdEsperandoEntrenamiento used to show text on the display

  void lcdEsperandoEntrenamiento(){
    //Limpiamos pantalla
    lcd.init();
    lcd.clear();
    
    lcd.home();
  
    lcd.print("Esperando");
    
    // Move the cursor characters to the right and
    // zero characters down (line 1).
    lcd.setCursor(0, 1);
    
    lcd.print("entrenamiento");
  }

//---------- lcdBienvenidoCT used to show text on the display

  void lcdBienvenidoCT(){
    //Limpiamos pantalla
    lcd.init();
    lcd.clear();
    
    lcd.home();
  
    lcd.print("Bienvenido a");
    
    // Move the cursor characters to the right and
    // zero characters down (line 1).
    lcd.setCursor(0, 1);
    
    lcd.print("CognitiveTrainer");
  }

//---------- lcdErroresTiempo used to show error and time on the display

  void lcdErroresTiempo(String error, String tiempo){
    //Limpiamos pantalla
    lcd.init();
    lcd.clear();
    
    lcd.home();
  
    lcd.print("Tiempo: ");
    lcd.print(tiempo);
    lcd.print("\"");
    // Move the cursor characters to the right and
    // zero characters down (line 1).
    lcd.setCursor(0, 1);
    
    lcd.print("Errores: ");
    lcd.print(error);
  }  

//------------ leerTAG used to read tag

  String leerTAG(){
    //nfc.begin();
    Serial.println("\nScan a NFC tag\n");
    bool flag=true;
    while (flag)
    { 
      if(nfc.tagPresent()){
        NfcTag tag = nfc.read();
        //tag.print();
        Serial.println(tag.getUidString());
        flag = false;
        return tag.getUidString();
      }
    }
    return "0";
  }

//---------- lcdLeerJugador used to show text on the display
  void lcdJugarSimonCruce(){
    //Limpiamos pantalla
    lcd.init();
    lcd.clear();
    
    lcd.home();
  
    lcd.print("Comienza cruce");
    
    // Move the cursor characters to the right and
    // zero characters down (line 1).
    lcd.setCursor(0, 1);

    lcd.print("Modo: Simon");
    
    lcd.print(rfid);
  }

//---------- lcdLeerJugador used to show text on the display
  void lcdJugarSimonHit(){
    //Limpiamos pantalla
    lcd.init();
    lcd.clear();
    
    lcd.home();
  
    lcd.print("Comienza Hit");
    
    // Move the cursor characters to the right and
    // zero characters down (line 1).
    lcd.setCursor(0, 1);

    lcd.print("Modo: Simon");
    
    lcd.print(rfid);
  }

//---------- lcdLeerJugador used to show text on the display
  void lcdJugarSimpleCruce(){
    //Limpiamos pantalla
    lcd.init();
    lcd.clear();
    
    lcd.home();
  
    lcd.print("Comienza Cruce");
    
    // Move the cursor characters to the right and
    // zero characters down (line 1).
    lcd.setCursor(0, 1);

    lcd.print("Modo: Simple");
    
    lcd.print(rfid);
  }

//---------- lcdLeerJugador used to show text on the display
  void lcdJugarSimpleHit(){
    //Limpiamos pantalla
    lcd.init();
    lcd.clear();
    
    lcd.home();
  
    lcd.print("Comienza Hit");
    
    // Move the cursor characters to the right and
    // zero characters down (line 1).
    lcd.setCursor(0, 1);

    lcd.print("Modo: Simple");
    
    lcd.print(rfid);
  }

//======= COMIENZO DE FUNCIONES QUE ENVIO A PLACAS ======//

  void startHit(String EntrenaID, String modo, String repeticiones) {
    int errorTiempo[1][repeticiones.toInt()];
    int errorTotal=0;
    int tiempoTotal=0;
    
    if(modo.equals("1")){
      CheckWiFiConnectivity();
      String simple = "Simple,";
      String RepChar = simple + repeticiones;
      
      char repc[20];
      RepChar.toCharArray(repc,10);
      strcpy (repc, RepChar.c_str());
      lcdJugarSimpleHit();
      Serial.println(RepChar);
      Serial.println(repc);
      CognClient[1].write(repc);
      CognClient[1].flush();  
    } else {
      CheckWiFiConnectivity();
      String simon = "Simon,";
      String RepChar = simon + repeticiones;
      Serial.println(RepChar);
      lcdJugarSimonHit();
      char repc[20];
      RepChar.toCharArray(repc,10);
      strcpy (repc, RepChar.c_str());
      
      Serial.println(repc);
      CognClient[1].write(repc);
      CognClient[1].flush();  
    }
    String retornoCruce = AvailableMessage();
    while(retornoCruce.equals("False")){
      ESP.wdtFeed();
      retornoCruce = AvailableMessage();
    }
    String error = getValue(retornoCruce,',',0);
    Serial.println(error);
    String tiempo = getValue(retornoCruce,',',1);
    Serial.println(tiempo);
    
    float tiempoLCD = tiempo.toInt()/1000;
    lcdErroresTiempo(error, String(tiempoLCD)); 

    publishCruceTiempoError(EntrenaID, error, tiempo);
  }

  void startCruce(String EntrenaID, String modo, String repeticiones) {
    int errorTiempo[1][repeticiones.toInt()];
    int errorTotal=0;
    int tiempoTotal=0;
    
    if(modo.equals("1")){
      CheckWiFiConnectivity();
      String simple = "Simple,";
      String RepChar = simple + repeticiones;
      
      char repc[20];
      RepChar.toCharArray(repc,10);
      strcpy (repc, RepChar.c_str());
      
      lcdJugarSimpleCruce();
      
      Serial.println(RepChar);
      Serial.println(repc);
      CognClient[0].write(repc);
      CognClient[0].flush();  
    } else {
      CheckWiFiConnectivity();
      String simon = "Simon,";
      String RepChar = simon + repeticiones;
      Serial.println(RepChar);
      
      char repc[20];
      RepChar.toCharArray(repc,10);
      strcpy (repc, RepChar.c_str());
      lcdJugarSimonCruce();
      Serial.println(repc);
      CognClient[0].write(repc);
      CognClient[0].flush();  
    }
    String retornoCruce = AvailableMessage();
    while(retornoCruce.equals("False")){
      ESP.wdtFeed();
      retornoCruce = AvailableMessage();
    }
    String error = getValue(retornoCruce,',',0);
    Serial.println(error);
    String tiempo = getValue(retornoCruce,',',1);
    Serial.println(tiempo);
    
    float tiempoLCD = tiempo.toInt()/1000;
    lcdErroresTiempo(error, String(tiempoLCD)); 

    publishCruceTiempoError(EntrenaID, error, tiempo);
  }
