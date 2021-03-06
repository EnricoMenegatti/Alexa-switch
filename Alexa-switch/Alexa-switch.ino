
#define ESPALEXA_MAXDEVICES 1
#define ESPALEXA_ASYNC //it is important to define this before #include <Espalexa.h>!
//#define ESPALEXA_DEBUG

#define RESET_MIN 5

#include "Espalexa.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <ArduinoOTA.h>

//ESP----------------------------------------------------------------------------------------------------------------
bool resetESP = false, allSetup = false, serverAP = false;
bool saveInputPinState, saveOutputPinState;
int outputPin, inputPin;
double thisTime, lastTime;

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
EspalexaDevice* myEspDevice;

//WEBSERVER----------------------------------------------------------------------------------------------------------------
AsyncWebServer server(80);

//FUNCTIONS----------------------------------------------------------------------------------------------------------------
void alphaChanged(EspalexaDevice* d) 
{
  if (d == nullptr) return; //this is good practice, but not required
  
  Serial.print("Alpha value: "); Serial.println(d->getValue());
  Serial.print("Changed to ");
  if (d->getValue())
  {
    Serial.println("ON");
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(outputPin, HIGH);
    saveOutputPinState = true;
  }
  else 
  {
    Serial.println("OFF");
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(outputPin, LOW);
    saveOutputPinState = false;
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
    if(!(String(Device_Name).length() <= 1))
    {
      Serial.print("Device Name: "); Serial.println(Device_Name);
      
      // Define your devices here.
      myEspDevice = new EspalexaDevice(Device_Name, alphaChanged, EspalexaDeviceType::dimmable); //dimmable device
      espalexa.addDevice(myEspDevice);
      espalexa.begin(&server);
  
      allSetup = true;
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

    serverAP = true;
    lastTime = millis();
  }

  if(allSetup) OTA_Setup(Device_Name);
  else OTA_Setup("esp8266");
  
//read I/O
  inputPin = readFile(SPIFFS, "/configInput.txt").toInt();
  outputPin = readFile(SPIFFS, "/configOutput.txt").toInt();
  if(inputPin == 0) inputPin = 16;//D0 default
  if(outputPin == 0) outputPin = 5;//D1 default

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(inputPin, INPUT);
  pinMode(outputPin, OUTPUT);

  saveInputPinState = digitalRead(inputPin);
}


//MAIN---------------------------------------------------------------------------------------------------------------------
void loop()
{ 
  if(allSetup)
  {
    if(saveInputPinState != digitalRead(inputPin)) //on toggle
    {
      if(saveOutputPinState == true) myEspDevice->setValue(0);
      else myEspDevice->setValue(255);
      myEspDevice->doCallback();
      
      saveInputPinState = digitalRead(inputPin);
      Serial.print("INPUT toggle to: "); Serial.println(saveInputPinState);
      Serial.print("OUTPUT toggle to: "); Serial.println(saveOutputPinState);
    }
  }
  else
  {
    if(saveInputPinState != digitalRead(inputPin)) //on toggle
    {
      if(saveOutputPinState == true) 
      {
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(outputPin, LOW);
        saveOutputPinState = false;
      }
      else
      {        
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(outputPin, HIGH);
        saveOutputPinState = true;
      }
      
      saveInputPinState = digitalRead(inputPin);
      Serial.print("INPUT toggle to: "); Serial.println(saveInputPinState);
      Serial.print("OUTPUT toggle to: "); Serial.println(saveOutputPinState);
    }
  }

  if(serverAP && !(String(ssid).length() <= 1 || String(password).length() <= 1))//reset automatico in caso di nessun wifi
  {
    thisTime = millis();
    if(thisTime - lastTime > (RESET_MIN * 60 * 1000)) resetESP = true;
  }

  if(resetESP)
  {
    delay(1);
    ESP.restart(); //ESP.reset();
  }

  ArduinoOTA.handle();
  espalexa.loop();
  delay(1);
}
