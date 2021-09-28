int getDeviceInfoFromEEPROM(byte calledFrom) // Function to check The Stored Values of EEPROM
{
  getDevicePassword();
  getDeviceTypeDelay();
  getServerIPPort();
  initializeBackupState();
  checkForBackUpData();
  if (strlen(deviceType) > 5 && boosterDelayTime  >= 0) return 1;
  else return 0;
}

void getDeviceTypeDelay() // Funtion to Get Stored API Address
{
  char ch;
  memset(deviceType, '\0', sizeof(deviceType));
  char boosterDelayTimeStr[4];
  memset(boosterDelayTimeStr, '\0', sizeof(boosterDelayTimeStr));
  for (i = DEVICETYPEADDRESS; i <= DEVICETYPEADDRESS + sizeof(deviceType); i++)
  {
    if(EEPROM.read(i) == 255 || EEPROM.read(i) == 0) break;
    ch = EEPROM.read(i);
    deviceType[i-DEVICETYPEADDRESS] = ch;
  }
  for (i = BOOSTERDELAYADDRESS; i <= BOOSTERDELAYADDRESS + sizeof(boosterDelayTimeStr); i++)
  {
    if(EEPROM.read(i) == 255 || EEPROM.read(i) == 0 || !(isDigit(EEPROM.read(i)))) break;
    ch = EEPROM.read(i);
    boosterDelayTimeStr[i-BOOSTERDELAYADDRESS] = ch;
  }
  if(strlen(boosterDelayTimeStr) > 0) boosterDelayTime = atoi(boosterDelayTimeStr);
}

byte setDeviceTypeDelay(char* deviceTypeReq, char* boosterDelayTimeStr) // Function to Set API Address
{
//  Serial.println(boosterDelayTimeStr);
  if (strlen(deviceTypeReq) == 0 || strlen(boosterDelayTimeStr) == 0) return 0;
  i = 0;
  for (i = 0; i < strlen(boosterDelayTimeStr) ; i++)
  {
    if(isDigit(boosterDelayTimeStr[i]) && strlen(boosterDelayTimeStr) <= 3 && atoi(boosterDelayTimeStr) > 0) continue;
    else return 0;
  }
  for (i = DEVICETYPEADDRESS ; i <= DEVICETYPEADDRESS + sizeof(deviceType) ; i++)
  {
    if(EEPROM.read(i) == 255 || EEPROM.read(i) == 0) break;
    EEPROM.write(i, 0);
  }
  for (i = 0; i < strlen(deviceTypeReq); i++) EEPROM.write(i+DEVICETYPEADDRESS,deviceTypeReq[i]);
  strcpy(deviceType,deviceTypeReq);

  for (i = BOOSTERDELAYADDRESS ; i <= BOOSTERDELAYADDRESS + strlen(boosterDelayTimeStr) ; i++)
  {
    if(EEPROM.read(i) == 255 || EEPROM.read(i) == 0) break;
    EEPROM.write(i, 0);
  }
  for (i = 0; i < strlen(boosterDelayTimeStr); i++) EEPROM.write(i+BOOSTERDELAYADDRESS,boosterDelayTimeStr[i]);
  boosterDelayTime = atoi(boosterDelayTimeStr);
  return 1;
}

void setPumpStatus(byte pumpStatus)
{
  if (pumpStatus == 1)
  {
    EEPROM.write(PUMPSTATUSADDRESS,pumpStatus);
    digitalWrite(pumpControlPin, HIGH);
  }
  else
  {
    EEPROM.write(PUMPSTATUSADDRESS,pumpStatus);
    digitalWrite(pumpControlPin, LOW);
  }
}


