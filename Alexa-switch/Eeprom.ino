void Eeprom_read()
{ 
  EE_ind = 0;
  EEPROM.get(EE_ind, Device_Name);

  Serial.println("Eeprom read");
}

void Eeprom_save()
{
  EE_ind = 0;
  EEPROM.put(EE_ind, Device_Name);//scrive valore su eeprom solo se viene modificato

  EEPROM.commit();

  Serial.println("Eeprom save");
}
