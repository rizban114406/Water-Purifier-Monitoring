byte setServerIPPort(char* newAddress,char* newPort) // Function to Set API Address
{
//  Serial.println(newPort);
  if (strlen(newAddress) == 0 || strlen(newPort) == 0) return 0;
  for (i = 0; i < strlen(newAddress) ; i++)
  {
    if(isDigit(newAddress[i]) || newAddress[i] == '.') continue;
    else return 0;
  }
//  Serial.println(newAddress);
  for (i = 0; i < strlen(newPort) ; i++)
  {
    if(isDigit(newPort[i]) && strlen(newPort) <= 4 && atoi(newPort) > 0) continue;
    else return 0;
  }
  for (i = SERVERIPADDRESS ; i <= SERVERIPADDRESS + sizeof(serverIp) ; i++)
  {
    if(EEPROM.read(i) == 255 || EEPROM.read(i) == 0) break;
    EEPROM.write(i, 0);
  }
  for (i = 0; i < strlen(newAddress); i++) EEPROM.write(i+SERVERIPADDRESS,newAddress[i]); 
  strcpy(serverIp,newAddress);
  for (i = SERVERPORTADDRESS ; i <= SERVERPORTADDRESS + sizeof(serverPort) ; i++)
  {
    if(EEPROM.read(i) == 255 || EEPROM.read(i) == 0) break;
    EEPROM.write(i, 0);
  }
  for (i = 0; i < strlen(newPort); i++) EEPROM.write(i+SERVERPORTADDRESS,newPort[i]); 
  strcpy(serverPort,newPort);
  return 1;
}
void getServerIPPort() // Funtion to Get Stored API Address
{
  char ch;
  memset(serverIp, '\0', sizeof(serverIp));
  memset(serverPort, '\0', sizeof(serverPort));
  for (i = SERVERIPADDRESS; i <= SERVERIPADDRESS + sizeof(serverIp); i++)
  {
    if(EEPROM.read(i) == 255 || EEPROM.read(i) == 0 || (!(isDigit(EEPROM.read(i))) && EEPROM.read(i) != '.')) break;
    ch = EEPROM.read(i);
    serverIp[i-SERVERIPADDRESS] = ch;
  }
  for (i = SERVERPORTADDRESS; i <= SERVERPORTADDRESS + sizeof(serverPort); i++)
  {
    if(EEPROM.read(i) == 255 || EEPROM.read(i) == 0 || !(isDigit(EEPROM.read(i)))) break;
    ch = EEPROM.read(i);
    serverPort[i-SERVERPORTADDRESS] = ch;
  }
}
