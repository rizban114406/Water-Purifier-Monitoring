char* generateData()
{
  char runningTime[6];
  char allSensorsValue[100];
  memset(allSensorsValue,'\0',sizeof(allSensorsValue));
  strcat(allSensorsValue,"4");
  strcat(allSensorsValue,",");
  strcat(allSensorsValue,deviceId);
  strcat(allSensorsValue,"|");
  for (int index = 0; index < dataBackUpNumber; index++)
  {  
    if (pumpRunningaTime[index] == 0.00) break;
    else if (index > 0)
    {
      strcat(allSensorsValue,"|");
    }
    strcat(allSensorsValue,"WT:");
    memset(runningTime,'\0',sizeof(runningTime));
    dtostrf(pumpRunningaTime[index],4,2,runningTime);
    strcat(allSensorsValue,runningTime);
  }
//  Serial.print("Data: ");
//  Serial.println(allSensorsValue);
  return allSensorsValue;
}
void addNewValue(float value)
{
  byte index = 0;
  for (index = 0; index < dataBackUpNumber; index++)
  {
    if(pumpRunningaTime[index] == 0.00) break;
  }
  if (index == dataBackUpNumber)
  {
    for (index = 0; index < dataBackUpNumber - 1; index++)
    {
      pumpRunningaTime[index] = pumpRunningaTime[index + 1];       
    }
  }
  pumpRunningaTime[index] = value;
}
void initializeData()
{
  for (int index = 0; index < dataBackUpNumber; index++) pumpRunningaTime[index] = 0.00;
}
void printAllStoredData()
{
  for (byte index = 0; index < dataBackUpNumber; index++)
  {
    if(pumpRunningaTime[index] == 0.00) break;
//      Serial.print("Index i= ");
//      Serial.print(index);
//      Serial.print(" Running Time: ");
//      Serial.println(pumpRunningaTime[index]);
  }
}

