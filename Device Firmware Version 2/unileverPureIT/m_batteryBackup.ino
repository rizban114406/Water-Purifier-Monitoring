int batteryBackUpCheck()
{
  int batteryStatus = analogRead(batteryBackUpPin);
  if ((batteryStatus < volatageThreshold) && (EEPROM.read(0) == 1))
  {
//    Serial.println("Data Back UP Required");
    writeDataOnEEPROM();
    dataSendingFlag == false;
    turnOFFGSM();
  }
  else if (batteryStatus > volatageThreshold && gsmStatus == 0)
  {
//    Serial.println("Inside Network Reconfigure Condition");
//    gsmStatus = setNetworkGSM();
    gsmStatus = tcpConnectionStatus();
    checkForBackUpData();
  }
  return batteryStatus;
}

