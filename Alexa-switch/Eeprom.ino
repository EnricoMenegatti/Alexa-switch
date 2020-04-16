void Eeprom_read()
{ 
  EE_ind = 0;
  EEPROM.get(EE_ind, ssid);

  EE_ind += sizeof(ssid);
  EEPROM.get(EE_ind, password);

  EE_ind += sizeof(password);
  EEPROM.get(EE_ind, Device_Name);

  Serial.println("Eeprom read");
}

void Eeprom_save()
{
  EE_ind = 0;
  EEPROM.put(EE_ind, ssid);//scrive valore su eeprom solo se viene modificato

  EE_ind += sizeof(ssid);
  EEPROM.put(EE_ind, password);

  EE_ind += sizeof(password);
  EEPROM.put(EE_ind, Device_Name);
  
  EEPROM.commit();

  Serial.println("Eeprom saved");
}
