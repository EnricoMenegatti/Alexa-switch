
#define ESPALEXA_MAXDEVICES 1

#include "Espalexa.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

//WI-FI----------------------------------------------------------------------------------------------------------------
bool wifiConnected = false;
const char* ssid = "Vodafone-Menegatti_plus";
const char* password = "Menegatti13";

//EEPROM----------------------------------------------------------------------------------------------------------------
int EE_ind;
char Device_Name[20] = "";

//ALEXA----------------------------------------------------------------------------------------------------------------
Espalexa espalexa;

//WEBSERVER----------------------------------------------------------------------------------------------------------------
ESP8266WebServer server(80);

//FUNCTIONS----------------------------------------------------------------------------------------------------------------
void alphaChanged(EspalexaDevice* d) 
{
  if (d == nullptr) return; //this is good practice, but not required
  
  Serial.print("A changed to ");
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


// connect to wifi – returns true if successful or false if not
boolean connectWifi()
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
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else 
  {
    Serial.println("Connection failed.");
  }
  delay(100);
  return state;
}

//SETUP--------------------------------------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);

  EEPROM.begin(512);
  Eeprom_read();
  //Eeprom_save();
  Serial.print("Device name: ");
  Serial.println(Device_Name);
  Serial.print("Name lenght: ");
  Serial.println(strlen(Device_Name));
  if(strlen(Device_Name) == 0)
  {
    
  }
  else
  {
    // Initialise wifi connection
    wifiConnected = connectWifi();
    
    if(wifiConnected){
      server.on("/", HTTP_GET, [](){
      server.send(200, "text/plain", "This is an example index page your server may send.");
      });
      server.on("/test", HTTP_GET, [](){
      server.send(200, "text/plain", "This is a second subpage you may have.");
      });
      server.onNotFound([](){
        if (!espalexa.handleAlexaApiCall(server.uri(),server.arg(0))) //if you don't know the URI, ask espalexa whether it is an Alexa control request
        {
          //whatever you want to do with 404s
          server.send(404, "text/plain", "Not found");
        }
      });
  
      // Define your devices here.
      espalexa.addDevice("Prova", alphaChanged, EspalexaDeviceType::onoff); //non-dimmable device
  
      espalexa.begin(&server); //give espalexa a pointer to your server object so it can use your server instead of creating its own
      //server.begin(); //omit this since it will be done by espalexa.begin(&server)
    } 
    else
    {
      while (1)
      {
        Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
        delay(2500);
      }
    }
  }
   
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(D1, OUTPUT);
}
 
//MAIN---------------------------------------------------------------------------------------------------------------------
void loop()
{
   //server.handleClient() //you can omit this line from your code since it will be called in espalexa.loop()
   espalexa.loop();
   delay(1);
}
