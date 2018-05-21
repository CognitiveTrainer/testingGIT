/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Starts WiFi server with fix IP and listens
 *  Receives and sends messages to the client
 *  Communicates: wifi_client_01.ino
 */
#include <SPI.h>
#include <ESP8266WiFi.h>

byte ledPin = 2;
char ssid[] = "Antel9CA91";               // SSID of your home WiFi
char pass[] = "5029CA91";               // password of your home WiFi
WiFiServer server(80);                    

IPAddress ip(192, 168, 1, 80);            // IP address of the server
IPAddress gateway(192,168,1,1);           // gateway of your network
IPAddress subnet(255,255,255,0);          // subnet mask of your network

void setup() {
  Serial.begin(115200);                   // only for debug
  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.begin(ssid, pass);                 // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  server.begin();                         // starts the server
  /*
  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());  // some parameters from the network
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
  Serial.print("Networks: "); Serial.println(WiFi.scanNetworks());
  */
  pinMode(ledPin, OUTPUT);
}

void loop () {  
  int a = Serial.read();
  
  if(a==49){
    boolean clientConnected=false;
    clientConnected = checkClientConnected();
    if(clientConnected){
      Serial.println(clientConnected);
    }else{
      Serial.println(clientConnected);
    }
  }
  delay(1000);
}

boolean checkClientConnected(){
  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      client.stop();
      return true;
    }
    client.stop();                // terminates the connection with the client
    return false;
  }
}


