
void Start_Server() // Start a HTTP server with a file read handler and an upload handler
{
  webSocket.begin();                 // start the websocket server
  webSocket.onEvent(WSS_Event); // if there's an incomming websocket message, go to function 'WSS_Event'
  
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
      if(payload[0] == 'E' && payload[1] == 'S')//read ssid, pw & device name then save in eeprom
      {
        int i, j, temp;
        for(i = 0; i < 40; i++)
        {
          ssid[i] = '\0';
          password[i] = '\0';
          Device_Name[i] = '\0';
        }
        for(i = 3; payload[i] != '*'; i++)
        {
          ssid[j] = payload[i];
          j++;
        }
        
        temp = i;
        j = 0;
        for(i = temp + 1; payload[i] != '*'; i++)
        {
          password[j] = payload[i];
          j++;
        }
        
        temp = i;
        j = 0;
        for(i = temp + 1; payload[i] != '*'; i++)
        {
          Device_Name[j] = payload[i];
          j++;
        }
        
        Serial.print("SSID: "); Serial.println(ssid); Serial.println(sizeof(ssid));
        Serial.print("Password: "); Serial.println(password); Serial.println(sizeof(password));
        Serial.print("Device Name: "); Serial.println(Device_Name); Serial.println(sizeof(Device_Name));

        Eeprom_save();
      }
    }
    break;
  }
}
