//---------- INCLUDES ---------- 
//------------ LCD -------------
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);

//---------- NODE MCU ---------- 
#include <ESP8266WiFi.h>
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson/releases/tag/v5.0.7

//---------- PN532 -------------

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


//-- VARIABLES --
String rfid="0";
int publishInterval = 1000; // 5 seconds//Send adc every 5sc
long lastPublishMillis;

//-- NODEMCU CONNECTION TO IBM --
const char* ssid = "Antel9CA91";
const char* password = "5029CA91";

#define ORG "0nfqy4"
#define DEVICE_TYPE "NodeMCU"
#define DEVICE_ID "Master"
#define TOKEN "8feJ33h_!e@!zSEkL+"

//-------- IBM IoT PLATFORM CONNECTION --------

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

const char eventTopic[] = "iot-2/evt/status/fmt/json";
const char cmdTopic[] = "iot-2/cmd/rfid/fmt/json";


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

//---------- INFORMATION FROM IBM ---------- 


WiFiClient wifiClient;
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
  
  // Switch on the LED if an 1 was received as first character
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
  if (EntrenaID != ""){
    String toggleHit = String((const char*)data["toggleHit"]);
    String tipoHit = String((const char*)data["tipoHit"]);
    String repHit = String((const char*)data["repHit"]);
    String toggleCruce = String((const char*)data["toggleCruce"]);
    String tipoCruce = String((const char*)data["tipoCruce"]);
    String repCruce = String((const char*)data["repCruce"]);
    String serie = String((const char*)data["serie"]);

  }
}

PubSubClient client(server, 1883, callback, wifiClient);

//---------- INITIAL SETUP AND LOOP ---------- 

void setup() {
  Serial.begin(9600); Serial.println();
  client.setCallback(callback);
  //pinMode(tag, INPUT);
  wifiConnect();
  mqttConnect();

  // The begin call takes the width and height. This
  // Should match the number provided to the constructor.
  lcd.begin(16,2);
  lcd.init();
  
  //Prender luz de fondo
  lcd.backlight();
  
  lcdBienvenidoCT();
  nfc.begin();
  
}

void loop() {
  /*if (millis() - lastPublishMillis > publishInterval) {
    publishData();
    lastPublishMillis = millis();
  }*/
  
  if (!client.loop()) {
    mqttConnect();
  }
}


//---------- INTERNAL FUNCTIONS ---------- 

//---------- wifiConnect() used to connect to an AP
void wifiConnect() {
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("nWiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

//---------- mqttConnect() used to connect to IBM IoT Platform ---------- 

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

//---------- publishData() used to send data to IBM IoT Platform ---------- 

void publishData() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);

  String payload = "{\"d\":{\"adc\":";
  payload += String(sensorValue, DEC);
  payload += "}}";

  //Serial.print("Sending payload: "); Serial.println(payload);
  
  /*if (client.publish(eventTopic, (char*) payload.c_str())) {
    Serial.println("Publish OK");
  } else {
    Serial.println("Publish FAILED");
  }*/
}

//---------- publishRFID() used to send data to IBM IoT Platform ---------- 

void publishRFID() {

  Serial.println("Entre al publishRFID");
  rfid.trim();
  String payload = "{\"rfid\":\""+rfid+"\"}";

  Serial.print("Sending payload: "); Serial.println(payload);
  
  if (client.publish(eventTopic, (char*) payload.c_str())) {
    Serial.println("Publish OK");
  } else {
    Serial.println("Publish FAILED");
  }
}


//---------- Leer RFID 

/*bool readRFID() {
  bool flag = true;
  int leerBoton = digitalRead(tag);
  
  if(leerBoton == LOW){
    Serial.println("Entre al leerboton");
    while(flag) {
      Serial.println("Entre al while");
      if(millis() - lastPublishMillis > publishInterval){
        Serial.println("Entre al if");
        publishRFID();
        lastPublishMillis = millis();
        lcdJugadorLeido();
        flag=false;
        return false;
      }
    }
    flag=true;  
  }

  return true;
}*/


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

