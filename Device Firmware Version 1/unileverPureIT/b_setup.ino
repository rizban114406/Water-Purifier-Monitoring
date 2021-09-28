void setup() {
//  Serial.begin(9600);
//  Serial.println("Starting");
  mySerial.begin(9600);
  // Pin mode declaration
  pinMode(volatageSensorPin, INPUT);
  pinMode(sosBtnPin, INPUT_PULLUP);
  pinMode(gsmPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(batteryBackUpPin, INPUT);
  pinMode(pumpControlPin, OUTPUT);

  // Restore pumo control status
  if (EEPROM.read(PUMPSTATUSADDRESS) == 1 || EEPROM.read(PUMPSTATUSADDRESS) == 255) digitalWrite(pumpControlPin, HIGH);
  else digitalWrite(pumpControlPin, LOW);
  // Check for configuration status
  confStatus = getDeviceInfoFromEEPROM(1);
  // Turn GSM On
  while(strlen(deviceId) > 14) getIMEINumber();
  // Check if data backup is available
  checkForBackUpData();
}
