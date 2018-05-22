//-- Libraries Included --------------------------------------------------------------
  #include <ESP8266WiFi.h>
  #include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3
//------------------------------------------------------------------------------------
  // Define I/O Pins
  #define     LED0      2           // WIFI Module LED
  #define     BUTTON    D1          // Connectivity ReInitiate Button
//------------------------------------------------------------------------------------
  // Authentication Variables
  char*       Cognssid;              // SERVER WIFI NAME
  char*       Cognpassword;          // SERVER PASSWORD
//------------------------------------------------------------------------------------
  #define     MAXSC     2           // MAXIMUM NUMBER OF CLIENTS
  
  WiFiServer  CognServer(8080);      // THE SERVER AND THE PORT NUMBER
  WiFiClient  CognClient[MAXSC];     // THE SERVER CLIENTS (Devices)
  //WiFiClient  CognClient;     // THE SERVER CLIENTS (Devices)
//====================================================================================
  // WATSON DEFINES AND VARIABLES
  #define ORG "uhr4b7"
  #define DEVICE_TYPE "ESP8266"
  #define DEVICE_ID "Server"
  #define TOKEN "cjUoIZbut&al1t5COa"
    
  char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
  char topic[] = "iot-2/evt/status/fmt/json";
  char authMethod[] = "use-token-auth";
  char token[] = TOKEN;
  char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
  
  PubSubClient client(server, 1883, NULL, CognClient[1]);
  
//====================================================================================

void setup()
  {
    // Setting The Serial Port
    Serial.begin(115200);           // Computer Communication
    
    // Setting The Mode Of Pins
    pinMode(LED0, OUTPUT);          // WIFI OnBoard LED Light
    pinMode(BUTTON, INPUT_PULLUP);  // Initiate Connectivity
    
    // Print Message Of I/O Setting Progress
    Serial.println();
    Serial.println("I/O Pins Modes Set .... Done");

    ConnectWiFi();
    
    // Starting Server
    CognServer.begin();
    CognServer.setNoDelay(true);
    Serial.println("Server Started");
  }

void loop() 
  {
    // Checking For Available Clients
    AvailableClients();
    // Checking For Available Client Messages
    AvailableMessage();
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

  void AvailableMessage()
  {
    //Serial.println("Available messages?");
    //check clients for data
    for(uint8_t i = 0; i < MAXSC; i++)
    {
      if (CognClient[i] && CognClient[i].connected() && CognClient[i].available())
      {
          while(CognClient[i].available())
          {
            String Message = CognClient[i].readStringUntil('\r');
            CognClient[i].flush();
            Serial.println("Client No " + String(i) + " - " + Message);
            Message.trim();
            sendToWatson(Message, "Dispositivo1");
          }
      } else {
        //Serial.println("No messages");
      }
    }
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

    IPAddress ip(192, 168, 43, 81);            // IP address of the server
    IPAddress gateway(192,168,43,1);           // gateway of your network
    IPAddress subnet(255,255,255,0);          // subnet mask of your network    
    
    WiFi.mode(WIFI_STA);            // To Avoid Broadcasting An SSID
    //WiFi.config(ip,gateway,subnet);
    WiFi.begin("Fedeb", "maife3220");      // The SSID That We Want To Connect To

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

void sendToWatson(String counter, String dispositivo)
  {
    if (!client.connected()) {
      Serial.print("Reconnecting client to ");
      Serial.println(server);
      while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
      Serial.println();
    }
    if(counter != ""){
      String payload = "{\"d\":{\"Name\":\"";
      payload += dispositivo;
      payload += "\",\"counter\":\"";
      payload += counter; 
      payload += "\"}}";
      
      Serial.print("Sending payload: ");
      Serial.println(payload);
      
      if (client.publish(topic, (char*) payload.c_str())) {
        Serial.println("Publish ok");
      } else {
        Serial.println("Publish failed");
      }
    }
  }
