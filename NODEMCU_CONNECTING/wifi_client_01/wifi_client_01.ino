/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Sends and receives message from the server in every 2 seconds
 *  Communicates: wifi_server_01.ino
 */ 
#include <SPI.h>
#include <ESP8266WiFi.h>

byte ledPin = 2;
char ssid[] = "Antel9CA91";           // SSID of your home WiFi
char pass[] = "5029CA91";            // password of your home WiFi

unsigned long askTimer = 0;

IPAddress server(192,168,1,80);       // La IP estatica del servidor
WiFiClient cliente;

void setup() {
  // -------------------------------------------------------
  // Configuracion de pines --------------------------------
  // -------------------------------------------------------
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(115200);               // only for debug

  // -------------------------------------------------------
  // Conectando al wifi ------------------------------------
  // -------------------------------------------------------
  
  WiFi.begin(ssid, pass);             
  Serial.println("!-- Conectando a " + String(WiFi.SSID()) + "--!");
  
  WiFi.mode(WIFI_STA);                // Evitamos que se genere un SSID

  // Estado de la conexión ---------------------------------
  checkWiFiStatus();  
  

  // Mostrando información del wifi ------------------------
  Serial.println("!-- Cliente Conectado--!");
  Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());

  
  // -------------------------------------------------------
  // Conectamos como cliente
  // -------------------------------------------------------
  connectionAsClient();
}

void loop () {
  /*
  boolean a = client.connect(server, 80);   // Connection to the server
  Serial.println(a);
  while(flag){
    if(String answer = client.readStringUntil('\r');   // receives the answer from the sever){
      
    }
  Serial.println(".");
  client.println("Hello server! Are you sleeping?\r");  // sends the message to the server
  
  Serial.println("from server: " + answer);
  }
  client.flush();
  delay(1000);*/
}


void checkWiFiStatus(){
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(50);
  }  
}

void connectionAsClient(){
  // Nos desconectamos del cliente
  cliente.stop();

  if(cliente.connect(server,80)){
    Serial.println();
    cliente.println("<soy yo a manga larga - conectado>");
  }
}

