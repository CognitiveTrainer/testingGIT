//---------- INCLUDES ---------- 
//------------ LCD -------------
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);

//---------- NODE MCU ---------- 
#include <ESP8266WiFi.h>
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson/releases/tag/v5.0.7

//---------- RFID READER -------- 
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

//-- NODEMCU CONNECTION TO IBM --
const char* ssid = "Fedeb";
const char* password = "maife3220";

#define ORG "9zt3lq"
#define DEVICE_TYPE "NodeMCU"
#define DEVICE_ID "Master"
#define TOKEN "1nm?kVqYp(-2T(h7I("

//-------- IBM IoT PLATFORM CONNECTION --------

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

const char eventTopic[] = "iot-2/evt/status/fmt/json";
const char cmdTopic[] = "iot-2/cmd/rfid/fmt/json";


//---------- INFORMATION FROM IBM ---------- 

WiFiClient wifiClient;
void callback(char* topic, byte* payload, unsigned int payloadLength) {
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

  // Obtener el nombre del método invocado, esto lo envia el switch de la puerta y el knob del motor que están en el dashboard
  String rfidStatus = String((const char*)data["rfid"]);
  Serial.print("Estado rfid:");
  Serial.println(rfidStatus);
  
  // Switch on the LED if an 1 was received as first character
  if (rfidStatus == "read") {
    Serial.print("Entre al if");
    lcdLeerJugador();  
  } else {
    Serial.print("No entre al if");
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}
PubSubClient client(server, 1883, callback, wifiClient);



int publishInterval = 1000; // 5 seconds//Send adc every 5sc
long lastPublishMillis;

//---------- INITIAL SETUP AND LOOP ---------- 

void setup() {
  Serial.begin(9600); Serial.println();
  client.setCallback(callback);
  pinMode(LED_BUILTIN, OUTPUT);
  wifiConnect();
  mqttConnect();

  // The begin call takes the width and height. This
  // Should match the number provided to the constructor.
  lcd.begin(16,2);
  lcd.init();
  
  //Prender luz de fondo
  lcd.backlight();
}

void loop() {
  if (millis() - lastPublishMillis > publishInterval) {
    publishData();
    lastPublishMillis = millis();
  }

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
  
  lcd.print("de jugador...");
}

//---------- lcdLeerJugador used to show text on the display

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


//---------- leerTAG used to read RFID tag

void leerTAG(){
    nfc.begin();
    Serial.println("\nScan a NFC tag\n");
    while (!nfc.tagPresent())
    {   
      delay(200);
    }
    NfcTag tag = nfc.read();
    tag.print();
    return;
}

