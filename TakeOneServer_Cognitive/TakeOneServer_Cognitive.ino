//-- Libraries Included --------------------------------------------------------------
  #include <ESP8266WiFi.h>
//------------------------------------------------------------------------------------
  // Define I/O Pins
  #define     LED0      2           // WIFI Module LED
  #define     LED1      D0          // Connectivity With Client #1
  #define     LED2      D2          // Connectivity With Client #2
  #define     BUTTON    D1          // Connectivity ReInitiate Button
//------------------------------------------------------------------------------------
  // Authentication Variables
  char*       Cognssid;              // SERVER WIFI NAME
  char*       Cognpassword;          // SERVER PASSWORD
//------------------------------------------------------------------------------------
  #define     MAXSC     2           // MAXIMUM NUMBER OF CLIENTS

  /*
   * MAXSC => Maximum Server Clients
   * They Are 6 Because
   * 1. Blue Fighter Suit
   * 2. Red Fighter Suit
   * 3. Corner Referee 1
   * 4. Corner Referee 2
   * 5. Corner Referee 3
   * 6. Corner Referee 4
   */
  
  WiFiServer  CognServer(8080);      // THE SERVER AND THE PORT NUMBER
  WiFiClient  CognClient[MAXSC];     // THE SERVER CLIENTS (Devices)
//====================================================================================

  void setup()
  {
    // Setting The Serial Port
    Serial.begin(115200);           // Computer Communication
    
    // Setting The Mode Of Pins
    pinMode(LED0, OUTPUT);          // WIFI OnBoard LED Light
    pinMode(LED1, OUTPUT);          // Indicator For Client #1 Connectivity
    pinMode(LED2, OUTPUT);          // Indicator For Client #2 Connectivity
    pinMode(BUTTON, INPUT_PULLUP);  // Initiate Connectivity
    
    // Print Message Of I/O Setting Progress
    Serial.println();
    Serial.println("I/O Pins Modes Set .... Done");

    // Setting Up A Wifi Access Point
    //SetWifi("TAKEONE", "");

    ConnectWiFi();
    
    // Starting Server
    CognServer.begin();
    CognServer.setNoDelay(true);
    Serial.println("Server Started");
  }

//====================================================================================
  
  void loop()
  {
    // Checking For Available Clients
    //AvailableClients();
    // Checking For Available Client Messages
    AvailableMessage();
  }

//====================================================================================
  
  void SetWifi(char* Name, char* Password)
  {
    // Stop Any Previous WIFI
    WiFi.disconnect();

    // Setting The Wifi Mode
    WiFi.mode(WIFI_AP_STA);
    Serial.println("WIFI Mode : AccessPoint Station");
    
    // Setting The AccessPoint Name & Password
    Cognssid      = Name;
    Cognpassword  = Password;
    
    // Starting The Access Point
    WiFi.softAP(Cognssid, Cognpassword);
    Serial.println("WIFI < " + String(Cognssid) + " > ... Started");
    
    // Wait For Few Seconds
    delay(1000);
    
    // Getting Server IP
    IPAddress IP = WiFi.softAPIP();
    
    // Printing The Server IP Address
    Serial.print("AccessPoint IP : ");
    Serial.println(IP);

    // Printing MAC Address
    Serial.print("AccessPoint MC : ");
    Serial.println(String(WiFi.softAPmacAddress()));

    // Starting Server
    CognServer.begin();
    CognServer.setNoDelay(true);
    Serial.println("Server Started");
  }

//====================================================================================

  void AvailableClients()
  {   
    if (CognServer.hasClient())
    {
      // Read LED0 Switch To Low If High.
      if(digitalRead(LED0) == HIGH) digitalWrite(LED0, LOW);
      
      // Light Up LED1
      digitalWrite(LED1, HIGH);
      
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
          }
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
    
    WiFi.mode(WIFI_STA);            // To Avoid Broadcasting An SSID
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
  }
