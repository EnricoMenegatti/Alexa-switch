
#define ESPALEXA_MAXDEVICES 1
#define ESPALEXA_ASYNC //it is important to define this before #include <Espalexa.h>!
//#define ESPALEXA_DEBUG

#include "Espalexa.h"
#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

//WI-FI----------------------------------------------------------------------------------------------------------------
bool wifiConnected = false;
char ssid[40] = "";//Vodafone-Menegatti_plus
char password[40] = "";//Menegatti13

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
AsyncWebServer server(80);

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

//SETUP--------------------------------------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  Serial.println("Setup...");
  
  SPIFFS_Setup();
  if(fileWiFiConfig('read'))
  {  
    Serial.print("SSID: "); Serial.println(ssid);
    Serial.print("Password: "); Serial.println(password);
  }
  else Serial.println("Config file NOT found!!");
  
  if(WiFiSTA_Setup())//try to connect Wi-Fi
  {
    Start_Server();

    if(fileDeviceConfig('read'))
    {
      Serial.print("Device Name: "); Serial.println(Device_Name);
      
      pinMode(LED_BUILTIN, OUTPUT);
      pinMode(D1, OUTPUT);
      
      // Define your devices here.
      espalexa.addDevice(Device_Name, alphaChanged, EspalexaDeviceType::onoff); //non-dimmable device
      espalexa.begin(&server);
    }
    else 
    {
      Serial.println("Device config NOT found!!");
      server.begin(); // start the HTTP server
    }
  }
  else //start AP Wi-Fi
  {
    WiFiAP_Setup();
    Start_Server();
    server.begin(); // start the HTTP server
  }
}


//MAIN---------------------------------------------------------------------------------------------------------------------
void loop()
{
  //server.handleClient();
  espalexa.loop();
  delay(1);
}
