
void Start_Server() // Start a HTTP server with a file read handler and an upload handler
{
  //definisci la cartella da cui lo SPIFFS preleva i file.
  server
    .serveStatic("/", SPIFFS, "/")
    .setDefaultFile("Info.html")
    .setTemplateProcessor(processor);

  server.on("/main.css", [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/main.css", "text/css");
  });
  
  server.on("/SubmitWiFi", HTTP_POST, handleSubWiFi);
  server.on("/SubmitDevice", HTTP_POST, handleSubDevice);
  server.on("/SubmitReset", handleSubReset);
  server.on("/SubmitRipr", handleSubRipr);

  
  
  server.onNotFound([](AsyncWebServerRequest *request){
    String path = request->url().c_str();
    Serial.println("handleFileRead: " + path);
    
   /* //String contentType = getContentType(path); // Get the MIME type
    String pathWithGz = path + ".gz";
    String pathWithHtml = path + ".html";
    
    if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path) || SPIFFS.exists(pathWithHtml)) // If the file exists, either as a compressed archive, or normal
    {
      if (SPIFFS.exists(pathWithGz)) // If there's a compressed version available
        path += ".gz"; // Use the compressed verion
  
      else if (SPIFFS.exists(pathWithHtml))
        path += ".html";
  
      request->send(SPIFFS, path);
    
      Serial.println(String("\tSent file: ") + path);
    }
    else Serial.println(String("\tFile Not Found: ") + path); // If the file doesn't exist, return false
  });*/
  });
}

String processor(const String& var)
{
  if(var == "SSID") return String(ssid);
  else if(var == "PASSWORD") return String(password);
  else if(var == "NAME") return String(Device_Name);
  return String();
}

void handleSubWiFi(AsyncWebServerRequest *request) 
{
  Serial.println("WiFi Config Submit");
  String s = request->arg("plain");
  Serial.println(s);

  request->arg("ssid").toCharArray(ssid, 40);
  request->arg("pw").toCharArray(password, 40);

  if(fileWiFiConfig('write'))
  {
    request->send(SPIFFS, "/WiFi");
    request->send(303); 
  } 
  else 
  {
    request->send(500, "text/plain", "500: couldn't create file");
  }
}

void handleSubDevice(AsyncWebServerRequest *request) 
{
  Serial.println("Device Config Submit");
  String s = request->arg("plain");
  Serial.println(s);
  
  request->arg("name").toCharArray(Device_Name, 40);

  if(fileDeviceConfig('write'))
  {
    request->send(SPIFFS, "/Device.html");
    request->send(303); 
  } 
  else 
  {
    request->send(500, "text/plain", "500: couldn't create file");
  }
}

void handleSubReset(AsyncWebServerRequest *request)
{
  Serial.println("Resetting ESP");
  request->send(303, "text/plain", "/");
  delay(1);
  ESP.restart(); //ESP.reset();
}

void handleSubRipr(AsyncWebServerRequest *request)
{
  Serial.println("Riprisino Submit");

  if(fileWiFiConfig('delete') && fileDeviceConfig('delete'))
  {
    request->send(303, "Location", "/SuccessRipr");
  } 
  else 
  {
    request->send(500, "text/plain", "500: couldn't delete file");
  }
}
