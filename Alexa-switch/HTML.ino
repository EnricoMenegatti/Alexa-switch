
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

void handleAdvanced() 
{
  char advanced[2048];
  snprintf(advanced, 2048,
    "<html>\
        <head>\
            <title>ESP-Alexa switch</title>\
            <link href='main.css' rel='stylesheet' type='text/css'>\
            <script src='main.js' type='text/javascript'></script>\
        </head>\
        \
        <body onload='Startup()'>\
            <center>\
                <H1>Configurazione ESP-Alexa switch</H1>\
                \
                <ul class='sidenav'>\
                    <li><a class='content' href='/Info'>Info</a></li>\
                    <li><a class='content' href='/WiFi'>WiFi</a></li>\
                    <li><a class='content' href='/Device'>Dispositivo</a></li>\
                    <li><a class='active' href='/Advanced'>Avanzate</a></li>\
                </ul>\
                \
                <div class='form-1'>\
                    <H2>Avanzate</H2>\
                    <table>\
                        <tr>\
                            <td class='first' rowspan='2'><label>Reset</label></td>\
                            <td class='second'><p class='adv'>Abilita</p></td>\
                            <td class='third' rowspan='2'>\
                                <form method='post' action='/SubmitReset'>\
                                    <input class='button' id='reset' type='submit' value='Reset'>\
                                </form>\
                            </td>\
                        </tr>\
                        <tr>\
                            <td class='second'><input type='checkbox' id='checkReset' onclick='Reset()'></td>\
                        </tr>\
                        <tr>\
                            <td class='first' rowspan='2'><label>Ripristino dati di Fabbrica</label></td>\
                            <td class='second'><p class='adv'>Abilita</p></td>\
                            <td class='third' rowspan='2'>\
                                <form method='post' action='/SubmitRipr'>\
                                    <input class='button' id='ripristino' type='submit' value='Ripristino'>\
                                </form>\
                            </td>\
                        </tr>\
                        <tr>\
                            <td class='second'><input type='checkbox' id='checkRipr' onclick='Ripristino()'></td>\
                        </tr>\
                    </table>\
                </div>\
            </center>\
        </body>\
    </html>");
    server.send(200, "text/html", advanced);
}