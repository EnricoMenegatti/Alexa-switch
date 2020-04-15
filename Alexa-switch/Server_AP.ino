
void SERVER_Setup() 
{
  WiFi.softAPConfig(IP_AP, GTW_AP, mask_AP);
  WiFi.softAP(ssid_AP, password_AP);

  Serial.println();
  Serial.print("AP SSID: ");
  Serial.println(ssid_AP);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  webSocket.begin();                 // start the websocket server
  webSocket.onEvent(WSS_Event); // if there's an incomming websocket message, go to function 'WSS_Event'

  Start_Server();
  
  Serial.println("Server setup OK");
}

void Start_Server() // Start a HTTP server with a file read handler and an upload handler
{
  server.on("/Edit.html", HTTP_POST, []() 
  {
    server.send(200, "text/plain", "");
  }, handleFileUpload); // go to 'handleFileUpload'
  
  server.onNotFound(handleNotFound); // if someone requests any other file or page, go to function 'handleNotFound'
                                     // and check if the file exists
  server.begin(); // start the HTTP server
}

void handleNotFound() // if the requested file or page doesn't exist, return a 404 not found error
{
  if(!handleFileRead(server.uri())) // check if the file exists in the flash memory (SPIFFS), if so, send it
  {
    server.send(404, "text/plain", "404: File Not Found");
  }
}

void WSS_Event(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) 
{ // When a WebSocket message is received
  switch (type) 
  {
    case WStype_DISCONNECTED:             // if the websocket is disconnected
      Serial.printf("[%u] Disconnected!\n", num);
    break;
      
    case WStype_CONNECTED: 
    {              // if a new websocket connection is established
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
    }
    break;
    
    case WStype_TEXT:                     // if new text data is received
    {
      Serial.printf("[%u] get Text: %s\n", num, payload);
      
    }
    break;
  }
}
