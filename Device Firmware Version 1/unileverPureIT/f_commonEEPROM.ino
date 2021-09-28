byte setDevicePassword(char* newPassword) // Function to Set Password
{
  if (strlen(newPassword) == 0) return 0;
  for (i = 0; i < strlen(newPassword) ; i++)
  {
    if(((newPassword[i] >= 'a' && newPassword[i] <= 'z') || (newPassword[i] >= 'A' && newPassword[i] <= 'Z') || (isdigit(newPassword[i]))) && i < 7) continue;
    else return 0;
  }
  for (i = PASSWORDADDRESS ; i <= PASSWORDADDRESS + sizeof(devicePassword) ; i++)
  {
    if(EEPROM.read(i) == 255 || EEPROM.read(i) == 0) break;
    EEPROM.write(i, 0);
  }
  for (i = 0; i < strlen(newPassword); i++) EEPROM.write(i+PASSWORDADDRESS,newPassword[i]);
  strcpy(devicePassword,newPassword);
  return 1;
}

void getDevicePassword() // Function to Get Stored Password
{
  char getPassword[6];
  char ch;
  memset(getPassword, '\0', sizeof(getPassword));
  memset(devicePassword, '\0', sizeof(devicePassword));
  for (i = PASSWORDADDRESS; i <= PASSWORDADDRESS + sizeof(devicePassword); i++)
  {
    //Serial.println(i);
    if(EEPROM.read(i) == 255 || EEPROM.read(i) == 0) break;
    ch = EEPROM.read(i);
    getPassword[i-PASSWORDADDRESS] = ch; 
  }
  if(strlen(getPassword) > 0) strcpy(devicePassword,getPassword);
  else strcpy(devicePassword,"123456");
}


