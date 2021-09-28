void initializeBackupState()
{
  if (EEPROM.read(0) != 0 && EEPROM.read(0) != 1) EEPROM.write(0,1);
  if (char(EEPROM.read(1)) != 'S' && EEPROM.read(0) == 1) EEPROM.write(1,'S');
}
void writeDataOnEEPROM()
{
  byte index = 0;
  for (index = 0; index < dataBackUpNumber; index++)
  {
    if(pumpRunningaTime[index] == 0.00) break;
    if (EEPROM.read(0) == 1) EEPROM.write(0,0);
    
    char runningTime[6];
    memset(runningTime,'\0',sizeof(runningTime));
    dtostrf(pumpRunningaTime[index],4,2,runningTime);
    for (i = BACKUPADDRESS; i <= (dataBackUpNumber * strlen(runningTime)); i++)
    {
      if(char(EEPROM.read(i)) == 'S') break;
    }
    for (int dataIndex = 0; dataIndex < strlen(runningTime); dataIndex++)
    {
      EEPROM.write(i,runningTime[dataIndex]);
      i++;
    }
    EEPROM.write(i,'-');
    i++;
    EEPROM.write(i,'S');
  } 
  initializeData();
//  Serial.println("Data Backup Successful");
}

byte checkForBackUpData()// 0 = Data Backup available, 1 = Not Available
{
  if (EEPROM.read(0) == 0)
  {
//    Serial.println("Data Backup Available");
    byte dataIndex = 0;
    char runningTime[6];
    byte readIndex = 0;
    memset(runningTime,'\0',sizeof(runningTime));
    for (i = BACKUPADDRESS; i <= (dataBackUpNumber * strlen(runningTime)); i++)
    {
      char ch = char(EEPROM.read(i));
//      Serial.print(ch);
      if(ch == 'S') break;
      else if(ch == '-')
      {
        pumpRunningaTime[dataIndex] = atof(runningTime);
        memset(runningTime,'\0',sizeof(runningTime));
        dataIndex++;
        readIndex = 0;
        continue;
      }
      runningTime[readIndex] = ch;
      readIndex++;
//      delay(1000);
    }
    EEPROM.write(1,'S');
    EEPROM.write(0,1);
    dataSendingFlag = true;
  }
//  else
//  {
//    Serial.println("Data Backup Not Available");
//  }
//    printAllStoredData();
}
