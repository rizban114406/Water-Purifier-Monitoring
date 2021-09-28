int checkSMSToConfigure() // Function to Deal with SMS Commands
{
  char SMS[102];
  char message[100];
  if (gsmONOFF == 0)
  {
//    Serial.println("Check SMS");
    gsmStartTime = turnOnGSM();
  }
  if (((millis()/1000) - gsmStartTime) > 10)
  {
    responseFlag = sendATcommand("AT+CMGF=1", "OK", "ERROR", 1000);
    if (responseFlag == 1)
    {
      responseFlag = sendATcommand("AT+CMGL=\"ALL\"", "AT+CMGL=\"ALL\"", "ERROR", 2000);
      if (responseFlag == 1)
      {
        byte smsNumber = 0;
        memset(SMS,'\0',sizeof(SMS));
        responseFlag = 0;
        byte smsCharCount = 0;
        while(mySerial.available() == 0);
        float waitForMsg = millis()/1000;
        do
        {
          if(mySerial.available() > 0)
          {
            SMS[smsCharCount] = char(mySerial.read());
            if(SMS[smsCharCount] == '\n' && smsCharCount > 5)
            {
              char *smsRestPart;
              if (strstr(SMS,"+CMGL:") != NULL) // Check if there is any incoming message
              {
                strtok_r(SMS, ", ",&smsRestPart);
                smsNumber = atoi(strtok_r(smsRestPart, ", ",&smsRestPart));
                strtok_r(smsRestPart, ",",&smsRestPart);
                smsSIMNumber = strtok_r(smsRestPart, ", ",&smsRestPart);
              }
              else if(strstr(SMS,"UN") != NULL) // Check The Initial Message Pattern
              {
                char *command;
                char *password;
                strtok_r(SMS,",",&smsRestPart);
                command = strtok_r(smsRestPart,",",&smsRestPart);
                password = strtok_r(smsRestPart,",",&smsRestPart);
                byte len = 0;
//                Serial.print(command);
                
                if(strcmp(command,"RESETPASSWORD") == 0) // Command to Reset the Device Password to 123456
                {
                  setDevicePassword("123456");
                  memset(message,'\0',sizeof(message));
                  snprintf(message, sizeof(message), "ID: %s\nPassword Reset OK",deviceId);
                  responseFlag = sendReturnSms(message);
                  deleteSMS(smsNumber);
                  responseFlag = 1;
                }
                else if(strcmp(command,"CURRENTPASSWORD") == 0) // Command to Get the Existing Passord
                {
                  memset(message,'\0',sizeof(message));
                  snprintf(message, sizeof(message), "ID: %s\nPassword: %s",deviceId,devicePassword);
                  responseFlag = sendReturnSms(message);
                  deleteSMS(smsNumber);
                  responseFlag = 1;
                }
                else
                {
                  len = strlen(password);
                  password[len - 1] = '\0';
                  password[len - 2] = '\0';
                  if (strcmp(password,devicePassword) == 0)
                  {
                    if (strcmp(command,"Status") == 0 || strcmp(command,"status") == 0) // Command to get the current status of the device
                    {
                      memset(message,'\0',sizeof(message));
                      if (strlen(deviceType) > 1)
                      {
                        snprintf(message, sizeof(message), "ID: %s\nStatus: Online\nDevice Type: %s\nBooster Delay: %d",deviceId,deviceType,boosterDelayTime);
                      }
                      else
                      {
                        snprintf(message, sizeof(message), "ID: %s\nStatus: Online",deviceId);
                      }
                      responseFlag = sendReturnSms(message);
                      deleteSMS(smsNumber);
                      responseFlag = 1;
                    }
                    else if(strcmp(command,"Password") == 0 || strcmp(command,"password") == 0) // Sets the Password of the Device
                    {
                      char *newPassword = strtok_r(smsRestPart,",",&smsRestPart);
                      len = strlen(newPassword);
                      newPassword[len - 1] = '\0';
                      newPassword[len - 2] = '\0';
                      messageFlag = setDevicePassword(newPassword);
                      if (messageFlag == 1)
                      {
                        memset(message,'\0',sizeof(message));
                        snprintf(message, sizeof(message), "ID: %s\nPassword Change OK",deviceId);
                      }
                      else
                      {
                        memset(message,'\0',sizeof(message));
                        snprintf(message, sizeof(message), "ID: %s\nPassword Change NOT OK\nPassword Length Max 7",deviceId);
                      }
                      responseFlag = sendReturnSms(message);
                      deleteSMS(smsNumber);
                      responseFlag = 1;
                    }
                    else if(strcmp(command,"Type") == 0 || strcmp(command,"type") == 0) // Sets device type and booster delay time
                    {
                      char *deviceTypeReq = strtok_r(smsRestPart,",",&smsRestPart);
                      char *boosterDelayTimeStr = strtok_r(smsRestPart,",",&smsRestPart);
                      len = strlen(boosterDelayTimeStr);
                      boosterDelayTimeStr[len - 1] = '\0';
                      boosterDelayTimeStr[len - 2] = '\0';
                      messageFlag = setDeviceTypeDelay(deviceTypeReq,boosterDelayTimeStr);
                      if (messageFlag == 1)
                      {
                        memset(message,'\0',sizeof(message));
                        snprintf(message, sizeof(message), "ID: %s\nDevice Type: %s\nBooster Delay: %d\nDevice Type, Delay Set OK",deviceId,deviceType,boosterDelayTime);
                      }
                      else
                      {
                        memset(message,'\0',sizeof(message));
                        snprintf(message, sizeof(message), "ID: %s\nDelay Set NOT OK",deviceId);
                      }
                      responseFlag = sendReturnSms(message);
                      deleteSMS(smsNumber);
                      responseFlag = 1;
                    }
                    else if(strcmp(command,"Ip") == 0 || strcmp(command,"ip") == 0 || strcmp(command,"IP") == 0) // Sets data sending server Ip address and port number
                    {
                      char *newAddress = strtok_r(smsRestPart,",",&smsRestPart);
                      char *newPort = strtok_r(smsRestPart,",",&smsRestPart);
                      len = strlen(newPort);
                      newPort[len - 1] = '\0';
                      newPort[len - 2] = '\0';
                      messageFlag = setServerIPPort(newAddress,newPort);
                      if (messageFlag == 1)
                      {
                        memset(message,'\0',sizeof(message));
                        snprintf(message, sizeof(message), "ID: %s\nIP: %s\nPort: %s\nIP, Port OK",deviceId,serverIp,serverPort);
                      }
                      else
                      {
                        memset(message,'\0',sizeof(message));
                        snprintf(message, sizeof(message), "ID: %s\nIP, Port NOT OK",deviceId);
                      }
                      responseFlag = sendReturnSms(message);
                      deleteSMS(smsNumber);
                      responseFlag = 1;
                    }
                    else if (strcmp(command,"Pon") == 0 || strcmp(command,"pon") == 0) // Command to turn ON the water pump
                    {
                      setPumpStatus(1);
                      memset(message,'\0',sizeof(message));
                      snprintf(message, sizeof(message), "ID: %s\nPump Status: ON",deviceId);
                      responseFlag = sendReturnSms(message);
                      deleteSMS(smsNumber);
                      responseFlag = 1;
                    }
                    else if (strcmp(command,"Poff") == 0 || strcmp(command,"poff") == 0) // Command to turn OFF the water pump
                    {
                      setPumpStatus(0);
                      memset(message,'\0',sizeof(message));
                      snprintf(message, sizeof(message), "ID: %s\nPump Status: OFF",deviceId);
                      responseFlag = sendReturnSms(message);
                      deleteSMS(smsNumber);
                      responseFlag = 1;
                    }
                    else
                    {
                      memset(message,'\0',sizeof(message));
                      snprintf(message, sizeof(message), "Invalid SMS");
                      responseFlag = sendReturnSms(message);
                      deleteSMS(smsNumber);
                      responseFlag = 1;
                    }
                  }
                  else
                  {
                    memset(message,'\0',sizeof(message));
                    snprintf(message, sizeof(message), "ID: %s\nIncorrect Password",deviceId);
                    responseFlag = sendReturnSms(message);
                    deleteSMS(smsNumber);
                    responseFlag = 1;
                  }
                }
              }
              else
              {
                memset(message,'\0',sizeof(message));
                snprintf(message, sizeof(message), "Invalid SMS");
                responseFlag = sendReturnSms(message);
                deleteSMS(smsNumber);
                responseFlag = 1;
              }
              smsCharCount = -1;
              memset(SMS,'\0',sizeof(SMS));     
            }
            else if (smsCharCount == 100 || SMS[smsCharCount] == '\n')
            {
              smsCharCount = -1;
              memset(SMS,'\0',sizeof(SMS));
            }
            else if (strcmp(SMS, "OK") == 0)
            {
              responseFlag = 1;
              deleteSMS(smsNumber);
            }
            else if (strcmp(SMS, "ERROR") == 0 )
            {
              responseFlag = 1;
            }
            smsCharCount++;
          }
        }while((responseFlag == 0) && ((millis()/1000 - waitForMsg) < 10));    
      }
      else
      {
        turnOFFGSM();
      }
    }
    else
    {
      turnOFFGSM();
    }
  }
  if (strlen(deviceType) > 5 && boosterDelayTime  >= 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void deleteSMS(byte smsNo) // Function to Delete Read SMSs
{
  if (smsNo > 0)
  {
    char aux_str[15];
    snprintf(aux_str,sizeof(aux_str),"AT+CMGD=%d",smsNo);
    sendATcommand(aux_str,"OK", "ERROR", 2000);
  }
}

int8_t sendReturnSms(char *message) // Function to Send Confirmation SMS
{
  char aux_str[50]; 
  sendATcommand("AT+CMGF=1", "OK", "ERROR", 1000);
  snprintf(aux_str, sizeof(aux_str), "AT+CMGS=%s",smsSIMNumber.c_str());
  responseFlag = sendATcommand(aux_str, ">", "ERROR", 2000);
  if (responseFlag == 1)
  {
    mySerial.println(message);
    mySerial.write(0x1A);
    responseFlag = sendATcommand("", "OK", "ERROR", 10000);
    if (responseFlag == 1)
    {
      return 1;    
    }
  }
}
