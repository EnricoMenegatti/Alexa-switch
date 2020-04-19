
void Start_Server() // Start a HTTP server with a file read handler and an upload handler
{
  server.on("/", handleRoot);
  server.on("/Info", handleRoot);
  
  server.on("/SubmitWiFi", HTTP_POST, handleSubWiFi);
  server.on("/SubmitDevice", HTTP_POST, handleSubDevice);
  server.on("/SubmitReset", HTTP_POST, handleSubReset);
  server.on("/SubmitRipr", HTTP_POST, handleSubReset);

  server.on("/WiFi", handleWiFi);
  server.on("/Device", handleDevice);
  server.on("/Advanced", handleAdvanced);
  
  server.on("/Edit.html", HTTP_POST, []() 
  {
    server.send(200, "text/plain", "");
  }, handleFileUpload); // go to 'handleFileUpload'
  
  server.onNotFound(handleNotFound); // if someone requests any other file or page, go to function 'handleNotFound'
                                     // and check if the file exists
}

void handleNotFound() // if the requested file or page doesn't exist, return a 404 not found error
{
  if(!handleFileRead(server.uri())) // check if the file exists in the flash memory (SPIFFS), if so, send it
  {
    if (!espalexa.handleAlexaApiCall(server.uri(),server.arg(0))) //if you don't know the URI, ask espalexa whether it is an Alexa control request
    {
      //whatever you want to do with 404s
      server.send(404, "text/plain", "Not found");
    }
  }
}

void handleSubWiFi() 
{
  Serial.println("WiFi Config Submit");
  String s = server.arg("plain");
  Serial.println(s);

  server.arg("ssid").toCharArray(ssid, 40);
  server.arg("pw").toCharArray(password, 40);

  if(fileWiFiConfig('write'))
  {
    server.sendHeader("Location", "/WiFi"); // Redirect the client to the success page
    server.send(303); 
  } 
  else 
  {
    server.send(500, "text/plain", "500: couldn't create file");
  }
}

void handleSubDevice() 
{
  Serial.println("Device Config Submit");
  String s = server.arg("plain");
  Serial.println(s);
  
  server.arg("name").toCharArray(Device_Name, 40);

  if(fileDeviceConfig('write'))
  {
    server.sendHeader("Location", "/Device"); // Redirect the client to the success page
    server.send(303); 
  } 
  else 
  {
    server.send(500, "text/plain", "500: couldn't create file");
  }
}

void handleSubReset()
{
  Serial.println("Riprisino Submit");

  if(fileWiFiConfig('delete') && fileDeviceConfig('delete'))
  {
    server.sendHeader("Location", "/Advanced"); // Redirect the client to the success page
    server.send(303); 
  } 
  else 
  {
    server.send(500, "text/plain", "500: couldn't delete file");
  }
}
