float turnOnGSM() // Function to Shutdown the GSM
{
//  Serial.println("GSM Turning on");
  digitalWrite(gsmPin, HIGH);
  gsmONOFF = 1;
  return (millis()/1000);
}

void turnOFFGSM() // Function to Shutdown the GSM
{
  digitalWrite(gsmPin, LOW);
  gsmStartTime = 0;
  gsmONOFF = 0;
  gsmStatus = 0;
  delay(1000);
}

void getIMEINumber()
{
  uint8_t x=0;
  char ch;
  if(gsmONOFF == 0)
  {
    gsmStartTime = turnOnGSM();
    delay(5000);
  }
  mySerial.println("AT+GSN");
  delay(1000);
  while(mySerial.available())
  {
    ch = mySerial.read();
    if (ch == '\n' || ch == '\r') break;
    deviceId[x] = ch;
    x++;
  }
  deviceId[x] = '\0';
  while( mySerial.available() > 0) mySerial.read();
  if (strlen(deviceId) < 15) turnOFFGSM();
}

int8_t sendATcommand(char* ATcommand, char* expected_answer1, char* expected_answer2, unsigned int timeout) // Function to Communicate with GSM
{
  //  Serial.println(ATcommand);
  uint8_t x=0;
  char response[51];
  unsigned long previous;
  responseFlag = 0;
  memset(response, '\0', sizeof(response));    // Initialize the string
  delay(100);
  while( mySerial.available() > 0) mySerial.read();
  mySerial.println(ATcommand);
  x = 0;
  previous = millis();
  do{
    if(mySerial.available() != 0)
    {    
      response[x] = mySerial.read();
      x++;
//      Serial.println(x);
      // check if the desired responseFlag is in the response of the module
      if (strstr(response, expected_answer1) != NULL)    
      {
        responseFlag = 1;
      }
      else if (strstr(response, expected_answer2) != NULL)    
      {
        responseFlag = 2;
      }
      else if(x == 50)
      {
        x = 0;
        memset(response,'\0',sizeof(response));
      }
    }
  }
  while((responseFlag == 0) && ((millis() - previous) < timeout));
//  Serial.println(response);
  return responseFlag;
}
