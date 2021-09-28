void setup() {
//  Serial.begin(9600);
//  Serial.println("Starting");
  mySerial.begin(9600);
  // Pin mode declaration
  pinMode(volatageSensorPin, INPUT);
  pinMode(sosBtnPin, INPUT_PULLUP);
  pinMode(floatingSensorPin, INPUT_PULLUP);
  pinMode(gsmPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(batteryBackUpPin, INPUT);
  pinMode(boosterPumpPin, OUTPUT);
  pinMode(floatingSensorRelay, OUTPUT);
  pinMode(pumpControlPin, OUTPUT);

  // Restore pumo control status
  if (EEPROM.read(PUMPSTATUSADDRESS) == 1 || EEPROM.read(PUMPSTATUSADDRESS) == 255) digitalWrite(pumpControlPin, HIGH);
  else digitalWrite(pumpControlPin, LOW);
  // Restore floating sensor status
  if (digitalRead(floatingSensorPin)) digitalWrite(floatingSensorRelay, LOW);
  else digitalWrite(floatingSensorRelay, HIGH);
  // Check for configuration status
  confStatus = getDeviceInfoFromEEPROM(1);
  // Turn GSM On
  while(strlen(deviceId) > 14) getIMEINumber();
  // Check if data backup is available
  checkForBackUpData();
}
