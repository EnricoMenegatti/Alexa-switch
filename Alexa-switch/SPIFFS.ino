
void SPIFFS_Setup()
{
  SPIFFS.begin(); // Start the SPI Flash File System (SPIFFS)
  Serial.println("SPIFFS started. Contents:");
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) // List the file system contents
    { 
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("\tFS File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    Serial.printf("\n");
  }
}

String formatBytes(size_t bytes) // convert sizes in bytes to KB and MB
{ 
  if (bytes < 1024) 
  {
    return String(bytes) + "B";
  } 
  
  else if (bytes < (1024 * 1024)) 
  {
    return String(bytes / 1024.0) + "KB";
  } 
  
  else if (bytes < (1024 * 1024 * 1024)) 
  {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  }
}

bool handleFileRead(String path) // send the right file to the client (if it exists)
{
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "Config.html"; // If a folder is requested, send the index file
  
  String contentType = getContentType(path); // Get the MIME type
  String pathWithGz = path + ".gz";
  
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) // If the file exists, either as a compressed archive, or normal
  {
    if (SPIFFS.exists(pathWithGz)) // If there's a compressed version available
      path += ".gz"; // Use the compressed verion
      
    File file = SPIFFS.open(path, "r"); // Open the file
    size_t sent = server.streamFile(file, contentType); // Send it to the client
    file.close(); // Close the file again
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path); // If the file doesn't exist, return false
  return false;
}

void handleFileUpload() // upload a new file to the SPIFFS
{
  Serial.print("++++++++++++FILE UPLOAD++++++++++++");
  HTTPUpload& upload = server.upload();
  String path;
  
  if(upload.status == UPLOAD_FILE_START)
  {
    path = upload.filename;
    if(!path.startsWith("/")) path = "/"+path;
    
    if(!path.endsWith(".gz")) // The file server always prefers a compressed version of a file
    {
      String pathWithGz = path+".gz"; // So if an uploaded file is not compressed, the existing compressed
      if(SPIFFS.exists(pathWithGz)) // version of that file must be deleted (if it exists)
        SPIFFS.remove(pathWithGz);
    }
    Serial.print("handleFileUpload Name: "); Serial.println(path);
    fsUploadFile = SPIFFS.open(path, "w"); // Open the file for writing in SPIFFS (create if it doesn't exist)
    path = String();
  } 
  else if(upload.status == UPLOAD_FILE_WRITE)
  {
    if(fsUploadFile)
    fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
  } 
  else if(upload.status == UPLOAD_FILE_END)
  {
    if(fsUploadFile) // If the file was successfully created
    { 
      fsUploadFile.close(); // Close the file again
      Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
      server.sendHeader("Location","/success.html"); // Redirect the client to the success page
      server.send(303);
    } 
    else 
    {
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}

String getContentType(String filename) // determine the filetype of a given filename, based on the extension
{ 
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

boolean fileWiFiConfig(int fMode) //save wifi configuration parameter inside a SPIFFS file
{
  const char* path = "/wifiConfig";
  
  switch(fMode)
  {
    case 'write': //write file----------------------------------------------------------------------------
    {
      File new_file = SPIFFS.open(path, "w+");
      if (!new_file)
      {
        Serial.println("WiFi file open failed!");
        return false;
      }
      
      Serial.println("Writing to SPIFFS WiFi...");
      new_file.print("SSID: "); new_file.print(ssid); new_file.println(";");
      new_file.print("Password: "); new_file.print(password); new_file.println(";");
      new_file.close();
      Serial.println("Write to SPIFFS WiFi OK!");
      return true;
    }
    break;
  
    case 'read': //read file-------------------------------------------------------------------------------
    {
      int i, j;
      File read_file = SPIFFS.open(path, "r");
      if (!read_file)
      {
        Serial.println("WiFi file open failed!");
        return false;
      }
      Serial.println("Reading to SPIFFS WiFi...");
      String s = read_file.readString();
      Serial.println(s);
      read_file.close();
  
      int n = s.length();
      char char_array[n + 1]; 
      strcpy(char_array, s.c_str());

      for(i = 6; char_array[i] != ';'; i++)
      {
        Serial.print(char_array[i]);
        ssid[j] = char_array[i];
        j++;
      }

      j = 0;
      for(i += 13; char_array[i] != ';'; i++)
      {
        Serial.print(char_array[i]);
        password[j] = char_array[i];
        j++;
      }
      
      Serial.println("Read to SPIFFS WiFi OK!");
      return true;
    }
    break;
  
    case 'delete': //delete file--------------------------------------------------------------------------
    {
      if(!SPIFFS.remove(path))
      {
        Serial.println("WiFi file NOT removed!");
        return false;
      }
      Serial.println("WiFi file removed!");
      return true;
    }
    break;
  }
}

boolean fileDeviceConfig(int fMode) //save wifi configuration parameter inside a SPIFFS file
{
  const char* path = "/deviceConfig";
  
  switch(fMode)
  {
    case 'write': //write file----------------------------------------------------------------------------
    {
      File new_file = SPIFFS.open(path, "w+");
      if (!new_file)
      {
        Serial.println("Device file open failed!");
        return false;
      }
      
      Serial.println("Writing to SPIFFS Device...");
      new_file.print("Device name: "); new_file.print(Device_Name); new_file.println(";");
      new_file.close();
      Serial.println("Write to SPIFFS Device OK!");
      return true;
    }
    break;
  
    case 'read': //read file-------------------------------------------------------------------------------
    {
      int i, j;
      File read_file = SPIFFS.open(path, "r");
      if (!read_file)
      {
        Serial.println("Device file open failed!");
        return false;
      }
      Serial.println("Reading to SPIFFS Device...");
      String s = read_file.readString();
      Serial.println(s);
      read_file.close();
  
      int n = s.length();           //string to char
      char char_array[n + 1];       //
      strcpy(char_array, s.c_str());//

      for(i = 13; char_array[i] != ';'; i++)
      {
        Serial.print(char_array[i]);
        Device_Name[j] = char_array[i];
        j++;
      }

      Serial.println("Read to SPIFFS Device OK!");
      return true;
    }
    break;
  
    case 'delete': //delete file--------------------------------------------------------------------------
    {
      if(!SPIFFS.remove(path))
      {
        Serial.println("Device file NOT removed!");
        return false;
      }
      Serial.println("Device file removed!");
      return true;
    }
    break;
  }
}
 
