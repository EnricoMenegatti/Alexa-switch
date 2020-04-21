
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

      int firstEqual = s.indexOf(':');
      int lastEqual = s.lastIndexOf(':');
      int firstDiv = s.indexOf(';');
      int lastDiv = s.lastIndexOf(';');
      String S_ssid = s.substring(firstEqual + 2, firstDiv);
      String S_pw = s.substring(lastEqual + 2, lastDiv);
      S_ssid.toCharArray(ssid, 40);
      S_pw.toCharArray(password, 40);
      
      Serial.println("Read to SPIFFS WiFi OK!");
      return true;
    }
    break;
  
    case 'delete': //delete file--------------------------------------------------------------------------
    {
      if(!SPIFFS.exists(path))
      {
        Serial.println("WiFi file NOT exist!");
        return true;
      }
      
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
  const char* path = "/DeviceConfig";
  
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

      int firstEqual = s.indexOf(':');
      int firstDiv = s.indexOf(';');
      String S_name = s.substring(firstEqual + 2, firstDiv);
      if(S_name.length() <= 1) return false;
      
      S_name.toCharArray(Device_Name, 40);
      
      Serial.println("Read to SPIFFS Device OK!");
      return true;
    }
    break;
  
    case 'delete': //delete file--------------------------------------------------------------------------
    {
      if(!SPIFFS.exists(path))
      {
        Serial.println("Device file NOT exist!");
        return true;
      }
      
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
 
