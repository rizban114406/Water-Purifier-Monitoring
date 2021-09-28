int getDeviceInfoFromEEPROM(byte calledFrom) // Function to check The Stored Values of EEPROM
{
  getDevicePassword();
  getServerIPPort();
  initializeBackupState();
  checkForBackUpData();
  if (strlen(serverIp) > 14 && strlen(serverPort) > 3) return 1;
  else return 0;
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


