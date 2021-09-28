byte setNetworkGSM() // Function to Configure GSM Network
{
  if(gsmONOFF == 0) gsmStartTime = turnOnGSM();
  if (((millis()/1000) - gsmStartTime) > 16)
  {
    boolean gsmConnection = false;
    for (int j = 0 ; j < 2 ; j++)
    {
      if(sendATcommand("AT+CREG?", "+CREG: 0,1", "ERROR", 2000) != 0)
      {
        gsmConnection = true;
        break;
      }
    }
    if (gsmConnection == true)
    {
      sendATcommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", "OK", "ERROR", 2000);
      sendATcommand("AT+SAPBR=3,1,\"APN\",\"internet\"", "OK", "ERROR", 2000);
      sendATcommand("AT+SAPBR=3,1,\"USER\",\"\"", "OK", "ERROR", 2000);
      sendATcommand("AT+SAPBR=3,1,\"PWD\",\"\"", "OK", "ERROR", 2000);
      if(sendATcommand("AT+SAPBR=1,1", "OK", "ERROR", 10000) != 0)
      {
//        Serial.println("GSM Status is OK.\nGSM Status: 1");
        return 1;
      }
    }
    turnOFFGSM();
  }
//  Serial.println("GSM Status is Not OK.\nGSM Status: 0");
  return 0;
}

byte sendNotification(char* dataToSendToServer, char* deviceAPI)
{
  responseFlag = sendATcommand("AT+HTTPINIT", "OK", "ERROR", 10000);
  if (responseFlag == 1)
  {
    responseFlag = sendATcommand("AT+HTTPPARA=\"CID\",1", "OK", "ERROR", 5000);
    if (responseFlag == 1)
    {
      char aux_str_local[200];
      snprintf(aux_str_local, sizeof(aux_str_local), "AT+HTTPPARA=\"URL\",\"http://%s?data=%s\"",deviceAPI,dataToSendToServer);
      responseFlag = sendATcommand(aux_str_local, "OK", "ERROR", 6000);
      if (responseFlag == 1)
      {
        responseFlag = sendATcommand("AT+HTTPACTION=0", "+HTTPACTION: 0,200", "ERROR", 10000);
        if (responseFlag == 1)
        {
          sendNotiStat = false;
//          Serial.println("Notification Sent Successfully");
        }
      }
    }
  }
  if (responseFlag == 1) sendATcommand("AT+HTTPTERM", "OK", "ERROR", 5000);
  else turnOFFGSM();
}

byte sendLastValues(char* dataToSendToServer, char* deviceAPI)
{
//  Serial.print("Requested Task: ");
//  Serial.println(task);
//  Serial.print("Data To Be Sent: ");
//  Serial.println(dataToSendToServer);
  responseFlag = sendATcommand("AT+HTTPINIT", "OK", "ERROR", 10000);
  if (responseFlag == 1)
  {
    responseFlag = sendATcommand("AT+HTTPPARA=\"CID\",1", "OK", "ERROR", 5000);
    if (responseFlag == 1)
    {
      char aux_str_local[200];
      snprintf(aux_str_local, sizeof(aux_str_local), "AT+HTTPPARA=\"URL\",\"http://%s?data=%s\"",deviceAPI,dataToSendToServer);
      responseFlag = sendATcommand(aux_str_local, "OK", "ERROR", 6000);
      if (responseFlag == 1)
      {
        responseFlag = sendATcommand("AT+HTTPACTION=0", "+HTTPACTION: 0,200", "ERROR", 10000);
        if (responseFlag == 1)
        {
          dataSendingFlag = false;
          initializeData();
//          Serial.println("Data Sent Successfully");
        }
      }
    }
  }  
  if (responseFlag == 1) sendATcommand("AT+HTTPTERM", "OK", "ERROR", 5000);
  else turnOFFGSM();
}

