// Pin declaration
#define volatageSensorPin A0                          // Voltage
#define batteryBackUpPin A1                           // Battery Voltage
#define floatingSensorPin A2                          // Float Sensor
#define buzzerPin 2                                   // Buzzer
#define boosterPumpPin 3                              // Booster pump relay                 
#define sosBtnPin 7                                   // SOS 
#define floatingSensorRelay 6                         // Float sensor relay
#define pumpControlPin 4                              // Pump control ON/OFF
// Necassary Macros                  
#define dataBackUpNumber 5                            // Backup number                     
#define volatageThreshold 700                         // Battery voltage threshold    
#define delayTime 1000                                // Main loop delay
// EEPROM starting addresses
#include <EEPROM.h> 
#define PASSWORDADDRESS 105                           // Password                   
#define DEVICETYPEADDRESS 120                         // Device type        
#define BOOSTERDELAYADDRESS 150                       // Booster delay   
#define BACKUPADDRESS 1                               // Data backup starting address
#define SERVERIPADDRESS 185                           // Server IP address
#define SERVERPORTADDRESS 195                         // Server port address
#define PUMPSTATUSADDRESS 100                         // Pump control status address
// GSM connection and flags             
#include <SoftwareSerial.h>                    
#define gsmPin 5                                      // GSM vcc connected pin                     
SoftwareSerial mySerial(8,9);                         // Pin 8 = Tx & Pin 9 = Rx
uint8_t responseFlag = 0;                             // 0->Error, 1->Expected response                             
byte gsmONOFF = 0;                                    // 0->GSM is OFF & 1->GSM is ON
byte gsmStatus = 0;                                   // 0->Not ready for sending & 1->Ready for data sending
float gsmStartTime = 0;                               // To calculate delay between the starting time of GSM to sending first AT command
String smsSIMNumber = "";                             // SIM number to send the response message
byte messageFlag;                                     // To check if the command is handled successfully. 0->Error & 1->Success 
// Device related information                   
char *deviceId;                                       // Device Id(IMEI)
char devicePassword[7];                               // Device access password
char serverIp[16];                                    // Server IP address
char serverPort[5];                                   // Server port address
byte i = 0;
byte confStatus = 0;                                  // Check if the Device is Configured or Not. 0->Not configured & 1->Configured
// Water pump running calculation variables
unsigned long waterStartTime = 0;                     // To calculate the pump running period
boolean pumpStartStatus = false;                      // true->Pump started & false->Pump stopped
float pumpRunningaTime[dataBackUpNumber];             // Variable to store pump running hour data
// SOS button status
byte sosButtonState = 1;                              // SOS button pressed status
// Float sensor & Booster pump related variables
byte floatingSensorStatus = 1;                        // Floating sensor status
boolean boosterPumpFlag = false;                      // true->Booster pump needs to be started after delay & false-> Booster pump started
float boosterStartTime = 0;                           // Calculate delay between empty tank and booster pump start
byte boosterDelayTime = 0;                            // Booster pump starting delay
char deviceType[20];                                  // Purifier type
// Data Sending & Notification Flags
boolean dataSendingFlag = false;                      // true->Data is there to send & false->No new data to send
boolean sendNotiStat = false;                         // true-> SOS pressed send notification & false-> No notification

void turnONBuzzer()                                   // Buzzer when SOS pressed
{
  digitalWrite(buzzerPin, HIGH);
  delay(500);
  digitalWrite(buzzerPin, LOW);
}
// checkSMS
int checkSMSToConfigure();                                               // Function to handle incoming messages
void deleteSMS(byte smsNo);                                              // Function to delete the read message
int8_t sendReturnSms(char *message);                                     // Function to send response message
// dataEEPROM
int getDeviceInfoFromEEPROM(byte calledFrom);                            // Get all the stored information at starting time
void getDeviceTypeDelay();                                               // Get Device type and booster delay information
byte setDeviceTypeDelay(char* deviceTypeReq, char* boosterDelayTimeStr); // Set device type and booster delay information
void setPumpStatus(byte pumpStatus);                                     // Set water pump running status
// commonEEPROM
byte setDevicePassword(char* newPassword);                               // Function to set device password
void getDevicePassword();                                                // Function to get stored device password
//networkInfo
byte setServerIPPort(char* newAddress,char* newPort);                    // Function to set server IP address and port number
void getServerIPPort();                                                  // Function to get server Ip address and port number
// sendGetMethod
byte setNetworkGSM();                                                    // Function to set up network to send data in HTTP Get method
byte sendNotification(char* dataToSendToServer, char* deviceAPI);        // Function to send notification to server
byte sendLastValues(char* dataToSendToServer, char* deviceAPI);          // Function to send data to server
// generateData
char* generateData();                                                    // Function to make desired data sending format
void addNewValue(float value);                                           // Function to add new value to data array
void initializeData();                                                   // Function to initialize data variable
void printAllStoredData();                                               // Print all the stored data
// dataBackup
void initializeBackupState();                                            // Function to reset data backup pointers of EEPROM
void writeDataOnEEPROM();                                                // Function to write  backup data to EEPROM
byte checkForBackUpData();                                               // 0 = Data Backup available, 1 = Not Available
// sendTCPMethod
byte tcpConnectionStatus();                                              // Function to set up network to send data in TCP/IP communication
void sendLastValueTCP(char* send_data);                                  // Function to send data to server
// sensorData
void readVoltageSensorValue();                                           // Function to work with voltage sensor data
void floatingSensorValue();                                              // Function to work with floating sensor data
void sosButtonStatus();                                                  // Function to work with SOS button
// gsmSetup
float turnOnGSM();                                                       // Function to turn ON GSM
void turnOFFGSM();                                                       // Function to turn OFF GSM
void getIMEINumber();                                                    // Get IMEI number from GSM
int8_t sendATcommand(char* ATcommand, char* expected_answer1, char* expected_answer2, unsigned int timeout); // Function to Communicate with GSM
int batteryBackUpCheck();                                                // Function to check battery backup status
