void readVoltageSensorValue()
{
  float registorOne = 30000;
  float registorTwo = 7500;
  float analogValue = analogRead(volatageSensorPin);
  float voultOut = (analogValue * 5.0) / 1024.0;
  float voltIn = (voultOut / (registorTwo / (registorOne + registorTwo)));
//  Serial.print("Current Voltage From Pump: ");
//  Serial.println(voltIn);
  if (voltIn > 15 && pumpStartStatus == false)
  {
    pumpStartStatus = true;
    pumpStartTime = millis();
//    Serial.print("Pump has started.\n Water Start Time: ");
//    Serial.println(pumpStartTime);
  }
  else if (voltIn < 15 && pumpStartStatus == true)
  {
    pumpStartStatus = false;
    float waterConTime = float(((float(millis() - pumpStartTime))-delayTime)/1000);
//    Serial.print("Stopped!!! Pump Running Time: ");
//    Serial.println(waterConTime,2);
    addNewValue(waterConTime);
//    printAllStoredData();
    dataSendingFlag = true;
  }
}
void sosButtonStatus()
{
  byte sosButtonCurrentState = digitalRead(sosBtnPin);
  if ( sosButtonCurrentState != sosButtonState )
  {
    if( sosButtonCurrentState == LOW)
    {
      sendNotiStat = true;
      turnONBuzzer();
    }
    sosButtonState = sosButtonCurrentState;
  }
}

