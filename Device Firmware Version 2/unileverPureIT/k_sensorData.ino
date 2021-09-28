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
    waterStartTime = millis();
//    Serial.print("Pump has started.\n Water Start Time: ");
//    Serial.println(waterStartTime);
  }
  else if (voltIn < 15 && pumpStartStatus == true)
  {
    pumpStartStatus = false;
    float waterConTime = float(((float(millis() - waterStartTime))-delayTime)/1000);
//    Serial.print("Stopped!!! Pump Running Time: ");
//    Serial.println(waterConTime,2);
    addNewValue(waterConTime);
//    printAllStoredData();
    dataSendingFlag = true;
  }
}
void floatingSensorValue()
{
  byte floatingSensorCurrentStatus = digitalRead(floatingSensorPin);

  if (floatingSensorCurrentStatus != floatingSensorStatus && confStatus == 1)
  {
    if(floatingSensorCurrentStatus == LOW)
    {
      digitalWrite(floatingSensorRelay, HIGH);
      boosterPumpFlag = true;
      boosterStartTime = (millis()/1000);
    }
    else if(floatingSensorCurrentStatus == HIGH)
    {
      boosterPumpFlag = false;
      digitalWrite(floatingSensorRelay, LOW);
      digitalWrite(boosterPumpPin, LOW);
    }
    floatingSensorStatus = floatingSensorCurrentStatus;
  }
  if (boosterPumpFlag == true && ((millis()/1000) - boosterStartTime) >  (boosterDelayTime-2))
  {
    boosterPumpFlag = false;
    digitalWrite(boosterPumpPin, HIGH);
  }
  //  Serial.print("boosterPumpFlag: ");
//  Serial.println(boosterPumpFlag);
//  Serial.print("boosterStartTime: ");
//  Serial.println(boosterStartTime);
//  Serial.print("boosterDelayTime: ");
//  Serial.println(boosterDelayTime);
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

