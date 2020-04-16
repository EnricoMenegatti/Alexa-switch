
#define ESPALEXA_MAXDEVICES 1

#include "Espalexa.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include <EEPROM.h>
#include <Arduino.h>

#define ESP8266 

//WI-FI----------------------------------------------------------------------------------------------------------------
bool wifiConnected = false;
char ssid[40] = "";
char password[40] = "";

const char* ssid_AP = "Alexa-switch";
const char* password_AP = "";
IPAddress IP_AP(192,168,4,1);
IPAddress GTW_AP(192,168,4,1);
IPAddress mask_AP = (255, 255, 255, 0);

//EEPROM----------------------------------------------------------------------------------------------------------------
int EE_ind;
char Device_Name[40] = "";

//ALEXA----------------------------------------------------------------------------------------------------------------
Espalexa espalexa;

//WEBSERVER----------------------------------------------------------------------------------------------------------------
ESP8266WebServer server(80);
WebSocketsServer webSocket(81);
File fsUploadFile;


//FUNCTIONS----------------------------------------------------------------------------------------------------------------
void alphaChanged(EspalexaDevice* d) 
{
  if (d == nullptr) return; //this is good practice, but not required
  
  Serial.print("Changed to ");
  if (d -> getValue())
  {
    Serial.println("ON");
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(D1, HIGH);
  }
  else 
  {
    Serial.println("OFF");
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(D1, LOW);
  }
}

boolean WiFiSTA_Setup() 
{
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
    if (i > 20)
    {
      state = false; 
      break;
    }
    i++;
  }
  Serial.println("");
  if (state)
  {
    Serial.print("Connected to "); Serial.println(ssid);
    Serial.print("IP address: "); Serial.println(WiFi.localIP());
  }
  else 
  {
    Serial.println("Connection failed.");
  }
  return state;
}

void WiFiAP_Setup() 
{
  WiFi.softAPConfig(IP_AP, GTW_AP, mask_AP);
  WiFi.softAP(ssid_AP, password_AP);

  Serial.println();
  Serial.print("AP SSID: ");
  Serial.println(ssid_AP);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}


//SETUP--------------------------------------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  Serial.println("Setup...");

  EEPROM.begin(512);
  Eeprom_read();

  Serial.print("SSID: "); Serial.println(ssid);
  Serial.print("Password: "); Serial.println(password);
  Serial.print("Device Name: "); Serial.println(Device_Name);

  if(WiFiSTA_Setup())//try to connect Wi-Fi
  {
    Start_Server();
    SPIFFS_Setup();
    
    // Define your devices here.
    espalexa.addDevice(Device_Name, alphaChanged, EspalexaDeviceType::onoff); //non-dimmable device
    espalexa.begin();
  }
  else //start AP Wi-Fi
  {
    WiFiAP_Setup();
    Start_Server();
    SPIFFS_Setup();
  }

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(D1, OUTPUT);
}
 
//MAIN---------------------------------------------------------------------------------------------------------------------
void loop()
{
  webSocket.loop();
  server.handleClient();
  espalexa.loop();
  delay(1);
}
