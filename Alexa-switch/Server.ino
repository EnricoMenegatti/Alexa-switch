
void Start_Server() // Start a HTTP server with a file read handler and an upload handler
{
  server.on("/", handleRoot);
  server.on("/Info", handleRoot);
  
  server.on("/SubmitWiFi", HTTP_POST, handleSubWiFi);
  server.on("/SubmitDevice", HTTP_POST, handleSubDevice);

  server.on("/WiFi", handleWiFi);
  server.on("/Device", handleDevice);
  
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
                <ul class='sidenav'>\
                    <li><a class='active' href='/Info'>Info</a></li>\
                    <li><a class='content' href='/WiFi'>WiFi</a></li>\
                    <li><a class='content' href='/Device'>Dispositivo</a></li>\
                    <li><a class='content' href='/Advanced'>Avanzate</a></li>\
                </ul>\
                \
            </center>\
        </body>\
    </html>");
  server.send(200, "text/html", root);
}

void handleWiFi() 
{
  char wifi[2048];
  snprintf(wifi, 2048,
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
                <ul class='sidenav'>\
                    <li><a class='content' href='/Info'>Info</a></li>\
                    <li><a class='active' href='/WiFi'>WiFi</a></li>\
                    <li><a class='content' href='/Device'>Dispositivo</a></li>\
                    <li><a class='content' href='/Advanced'>Avanzate</a></li>\
                </ul>\
                \
                <form class='form-1' method='post' action='/SubmitWiFi'>\
                    <H2>Wi-Fi</H2>\
                    <label for='ssid'>SSID</label>\
                    <input type='text' id='ssid' name='ssid' maxlength='40' value=%s>\
                    <br><br>\
                    \
                    <label for='pw'>Password</label>\
                    <input type='password' id='pw' name='pw' maxlength='40' value=%s>\
                    \
                    <input class='button' type='reset' value='Annulla'>\
                    <input class='button' type='submit' value='Salva'>\
                </form>\
            </center>\
        </body>\
    </html>", ssid, password);
    server.send(200, "text/html", wifi);
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
    server.sendHeader("Location", "/"); // Redirect the client to the success page
    server.send(303); 
  } 
  else 
  {
    server.send(500, "text/plain", "500: couldn't create file");
  }
}

void handleDevice() 
{
  char device[2048];
  snprintf(device, 2048,
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
                <ul class='sidenav'>\
                    <li><a class='content' href='/Info'>Info</a></li>\
                    <li><a class='content' href='/WiFi'>WiFi</a></li>\
                    <li><a class='active' href='/Device'>Dispositivo</a></li>\
                    <li><a class='content' href='/Advanced'>Avanzate</a></li>\
                </ul>\
                \
                <form class='form-1' method='post' action='/SubmitDevice'>\
                    <H2>Dispositivo</H2>\
                    <label for='name'>Nome</label>\
                    <input type='text' id='name' name='name' maxlength='40' value=%s>\
                    \
                    <input class='button' type='reset' value='Annulla'>\
                    <input class='button' type='submit' value='Salva'>\
                </form>\
            </center>\
        </body>\
    </html>", Device_Name);
    server.send(200, "text/html", device);
}

void handleSubDevice() 
{
  Serial.println("Device Config Submit");
  String s = server.arg("plain");
  Serial.println(s);
  
  server.arg("name").toCharArray(Device_Name, 40);

  if(fileDeviceConfig('write'))
  {
    server.sendHeader("Location", "/"); // Redirect the client to the success page
    server.send(303); 
  } 
  else 
  {
    server.send(500, "text/plain", "500: couldn't create file");
  }
}
