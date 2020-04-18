
void Start_Server() // Start a HTTP server with a file read handler and an upload handler
{
  server.on("/", handleRoot);
  
  //server.on("/PostParam.html", handlePlain);
  server.on("/Submit", HTTP_POST, handlePlain);
  
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

void handleRoot() 
{
  char root[2048];
  snprintf(root, 2048,
    "<html>\
        <head>\
            <title>ESP-Alexa switch</title>\
            <link href='main.css' rel='stylesheet' type='text/css'>\
        </head>\
        \
        <body>\
            <center>\
                <H1>Configurazione ESP-Alexa switch</H1>\
                \
                <form class='form-1' method='post' action='/Submit'>\
                    <H2>Wi-Fi</H2>\
                    <label for='ssid'>SSID</label>\
                    <input type='text' id='ssid' name='ssid' maxlength='40' value=%s>\
                    <br><br>\
                    \
                    <label for='pw'>Password</label>\
                    <input type='password' id='pw' name='pw' maxlength='40' value=%s>\
                    <br><br><br>\
                    \
                    <H2>Dispositivo</H2>\
                    <label for='name'>Nome</label>\
                    <input type='text' id='name' name='name' maxlength='40' value=%s>\
                    \
                    <input class='button' type='reset' value='Reimposta'>\
                    <input class='button' type='submit' value='Salva'>\
                </form>\
                \
                <div>\
                  <p id='debug'></p>\
                </div>\
            </center>\
        </body>\
    </html>", ssid, password, Device_Name);
    server.send(200, "text/html", root);
}

void handlePlain() 
{
  Serial.println("-------------");
  String s = server.arg("plain");
  Serial.println(s);

  server.arg("ssid").toCharArray(ssid, 40);
  server.arg("pw").toCharArray(password, 40);
  server.arg("name").toCharArray(Device_Name, 40);

  if(fileWiFiConfig('write') && fileDeviceConfig('write'))
  {
    server.sendHeader("Location", "/"); // Redirect the client to the success page
    server.send(303); 
  } 
  else 
  {
    server.send(500, "text/plain", "500: couldn't create file");
  }
}
