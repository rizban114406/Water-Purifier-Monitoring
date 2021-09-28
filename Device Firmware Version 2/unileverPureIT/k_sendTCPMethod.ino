byte tcpConnectionStatus() // This Function is Used to Send Device Data
{
  if(gsmONOFF == 0) gsmStartTime = turnOnGSM();
  responseFlag = sendATcommand("AT+CIPMUX=0", "OK", "ERROR", 1000);
  if (responseFlag == 1)
  {
    for (i = 0; i < 2; i++)
    {
      responseFlag = sendATcommand("AT+CIPSTATUS", "INITIAL", "ERROR", 500);
      if (responseFlag == 1) break;
      delay(2000);
    }
    if (responseFlag == 1)
    {
      responseFlag = sendATcommand("AT+CSTT=\"internet\",\"\",\"\"", "OK", "ERROR", 30000);
      //snprintf(aux_str, sizeof(aux_str), "AT+CSTT=\"%s\",\"%s\",\"%s\"", apn, user_name, password);
      if (responseFlag == 1)
      {
        for (i = 0; i < 2; i++)
        {
          responseFlag = sendATcommand("AT+CIPSTATUS", "START", "ERROR", 500);
          if (responseFlag == 1) break;
          delay(2000);
        }
        if (responseFlag == 1)
        {
          responseFlag = sendATcommand("AT+CIICR", "OK", "ERROR", 30000);
          if (responseFlag == 1)
          {
            for (i = 0; i < 2; i++)
            {
              responseFlag = sendATcommand("AT+CIPSTATUS", "GPRSACT", "ERROR", 500);
              if (responseFlag == 1) break;
              delay(2000);
            }
            if (responseFlag == 1)
            {
              responseFlag = sendATcommand("AT+CIFSR", ".", "ERROR", 10000);
              if (responseFlag == 1)
              {
                for (i = 0; i < 2; i++)
                {
                  responseFlag = sendATcommand("AT+CIPSTATUS", "IP STATUS", "ERROR", 500);
                  if (responseFlag == 1) break;
                  delay(2000);
                }
                if (responseFlag == 1)
                {
                  char aux_str[50];
                  snprintf(aux_str, sizeof(aux_str), "AT+CIPSTART=\"TCP\",\"%s\",\"%s\"", serverIp, serverPort);
                  responseFlag = sendATcommand(aux_str, "CONNECT OK", "CONNECT FAIL", 30000);
                  if (responseFlag == 1) return 1;
                }
              }
            }
          }       
        }
      }
    }
  }
  turnOFFGSM();
  return 0;
//  sendATcommand("AT+CIPSHUT", "OK", "ERROR", 10000);
}

void sendLastValueTCP(char* send_data)
{
//  Serial.println(send_data);
  char aux_str[strlen(send_data)+16];
  sprintf(aux_str,"AT+CIPSEND=%d", strlen(send_data));
  responseFlag = sendATcommand(aux_str, ">", "ERROR", 10000);
  if (responseFlag == 1)
  {
    responseFlag = sendATcommand(send_data, "SEND OK", "ERROR", 10000);
    if (responseFlag == 1)
    {
      dataSendingFlag = false;
      initializeData();
    }
    else
    {
//      turnOFFGSM();
//      sendATcommand("AT+CIPCLOSE", "CLOSE OK", "ERROR", 10000);
    }
  }
  else
  {
//    sendATcommand("AT+CIPCLOSE", "CLOSE OK", "ERROR", 10000);
//    turnOFFGSM();
//    Serial.println("Data Not Send Successfully");
  }
}
