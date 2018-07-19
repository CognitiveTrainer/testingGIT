//------------------------------------------------------------------------------------
// Librerias necesarias
//------------------------------------------------------------------------------------
#include <EEPROM.h>         // Necesaria para guardar los datos al apagar
#include <ESP8266WiFi.h>    // NODEMCU
//------------------------------------------------------------------------------------
// I/O Pins
//------------------------------------------------------------------------------------
#define       LED0      2         // WIFI Module LED
#define       BUTTON    D1        // Connectivity ReInitiate Button
//------------------------------------------------------------------------------------
// Variables de BUTTONS
//------------------------------------------------------------------------------------
int           ButtonState;
int           LastButtonState   = LOW;
int           LastDebounceTime  = 0;
int           DebounceDelay     = 50;
const String  ButtonColor       = "RED";
//------------------------------------------------------------------------------------
// LED Delay Variables
//------------------------------------------------------------------------------------
int           LEDState          = LOW;
unsigned long CurrMillis        = 0;
unsigned long PrevMillis        = 0;
unsigned long Interval          = 1000;
//------------------------------------------------------------------------------------
// WIFI Authentication Variables
//------------------------------------------------------------------------------------
/* This Client Is Going To Connect To A WIFI Server Access Point
   So You Have To Specify Server WIFI SSID & Password In The Code Not Here
   Please See The Function Below Name (WiFi.Begin)
   If WIFI dont need Any password Then WiFi.begin("SSIDNAME")
   If WIFI needs a Password Then WiFi.begin("SSIDNAME", "PASSWORD")
*/
char*         ssid;            // Wifi Name
char*         password;        // Wifi Password
//------------------------------------------------------------------------------------
// Modulo WIFI Puertos y Roles
//------------------------------------------------------------------------------------
/* Este modulo funciona como un cliente
   Se va a conectar al servidor Wifi con la IP 192.168.1.80
*/
int             CognServerPort  = 8080;
IPAddress       CognServer(192, 168, 1, 198);
WiFiClient      CognClient;
//====================================================================================

void setup()
{
  // Setting The Serial Port ----------------------------------------------
  Serial.begin(115200);           // Computer Communication

  // Setting The Mode Of Pins ---------------------------------------------
  pinMode(LED0, OUTPUT);          // WIFI OnBoard LED Light
  pinMode(BUTTON, INPUT_PULLUP);  // Initiate Connectivity
  digitalWrite(LED0, !LOW);       // Turn WiFi LED Off

  // Print Message Of I/O Setting Progress --------------------------------
  Serial.println("\nI/O Pins Modes Set .... Done");

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
  WiFi.begin("Antel9CA91", "5029CA91");      // The SSID That We Want To Connect To

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
}

//====================================================================================

void loop()
{
  CheckWiFiConnectivity();
  
  ReadButton();
  
  if (CognClient.available() > 0) {
    String c = CognClient.readStringUntil('\r');
    if(c.equals("start")){
      Serial.println("Comienza juego");  
    }
  }
  //delay(500);
  //Serial.println("<" + ButtonColor + "-SCORED>");
  //CognClient.println("<" + ButtonColor + "-SCORED>");
  //CognClient.flush();

  //delay(2000);
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
        CheckWiFiConnectivity();
        String retorno = "{errores: 1, tiempo: 2}";
        Serial.println(retorno);
        CognClient.print (retorno);
        CognClient.flush();
      }
    }
  }

  // Last Button State Concidered
  LastButtonState = reading;
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
