void loop() {
  floatingSensorValue();
  sosButtonStatus();
  readVoltageSensorValue();
  int batteryStatus = batteryBackUpCheck();
  if (batteryStatus > volatageThreshold && dataSendingFlag == true && gsmStatus == 1) 
  {
//    Serial.println("Inside Data Sending Condition");
    char* dataToSend = generateData();
//    sendLastValues(dataToSend,"103.9.185.218/unileverpureit/public/api/device/v1/addInSecond");
    sendLastValueTCP(dataToSend);
  }
  else if (batteryStatus > volatageThreshold && sendNotiStat == true && gsmStatus == 1)
  {
//    Serial.println("Inside Notification Sending Condition");
    char sosMessage[15];
    memset(sosMessage,'\0',sizeof(sosMessage));
    strcat(sosMessage,"4");
    strcat(sosMessage,",");
    strcat(sosMessage,deviceId);
//    Serial.println(sosMessage);
//    sendNotification(sosMessage,"103.9.185.218/unileverpureit/public/api/device/v1/sos");
    sendLastValueTCP(sosMessage);
    char message[100];
    snprintf(message, sizeof(message), "Device With ID: %s Is Facing Some Problems.\nPlease Take Necessary Actions.",deviceId);
    smsSIMNumber = "\"01755542967\"";
    sendReturnSms(message);
  }

  if (batteryStatus > volatageThreshold)
  {
    confStatus = checkSMSToConfigure();
  }
  delay(delayTime);
}
