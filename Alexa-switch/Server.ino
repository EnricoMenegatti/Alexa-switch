
void Start_Server() // Start a HTTP server with a file read handler and an upload handler
{
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
