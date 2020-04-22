
#define ESPALEXA_MAXDEVICES 1
#define ESPALEXA_ASYNC //it is important to define this before #include <Espalexa.h>!
//#define ESPALEXA_DEBUG

#include "Espalexa.h"
#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

//ESP----------------------------------------------------------------------------------------------------------------
bool resetESP = false;
int outputPin, inputPin;

//WI-FI----------------------------------------------------------------------------------------------------------------
bool wifiConnected = false;
char ssid[40] = "";//Vodafone-Menegatti_plus
char password[40] = "";//Menegatti13

const char* ssid_AP = "Alexa-switch";
const char* password_AP = "";
IPAddress IP_AP(192,168,4,1);
IPAddress GTW_AP(192,168,4,1);
IPAddress mask_AP = (255, 255, 255, 0);

//ALEXA----------------------------------------------------------------------------------------------------------------
char Device_Name[40] = "";
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
    digitalWrite(outputPin, HIGH);
  }
  else 
  {
    Serial.println("OFF");
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(outputPin, LOW);
  }
}

//SETUP--------------------------------------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  Serial.println("Setup...");
  
  SPIFFS_Setup();

  readFile(SPIFFS, "/configSSID.txt").toCharArray(ssid, 40);
  readFile(SPIFFS, "/configPassword.txt").toCharArray(password, 40);
  if(String(ssid).length() <= 1 || String(password).length() <= 1) 
  {
    Serial.println("Error reading WiFi files!");
  }

  Serial.print("SSID: "); Serial.println(ssid);
  Serial.print("Password: "); Serial.println(password);
  
  if(WiFiSTA_Setup())//try to connect Wi-Fi
  {
    Start_Server();

    readFile(SPIFFS, "/configDevice.txt").toCharArray(Device_Name, 40);
    Serial.print("Device Name: "); Serial.println(Device_Name);
    if(!String(Device_Name).length() <= 1)
    {
      inputPin = readFile(SPIFFS, "/configInput.txt").toInt();
      outputPin = readFile(SPIFFS, "/configOutput.txt").toInt();

      pinMode(LED_BUILTIN, OUTPUT);
      pinMode(outputPin, OUTPUT);
      
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
  if(resetESP)
  {
    delay(1);
    ESP.restart(); //ESP.reset();
  }
  
  espalexa.loop();
  delay(1);
}
