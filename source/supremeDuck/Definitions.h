#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define APP_VERSION "1.3"                            // It is used to compare it with the mobile app version.
                                                     // For example: 1.08 is compatible with 1.081 or 1.83148, 
                                                     // but it's not compatible with 1.09 or 1.091319 
                                                     // (in such case notification will be displayed in the app)

/* 

        Wireless Module Choice 

*/
// comment-out all to use standard bluetooth version (HC-06)                                                         
//#define WIFI_DUCKY_SETUP                             // uncomment if you're using "Esp8266" (e.g. the same hardware/wiring as wifi_ducky)
//#define JDY_10_SETUP                                 // uncomment if using JDY-10 (BLE module), AT commands are followed by "carriage return" (\r) and "newline" (\n) bytes unlike JDY-08
//#define JDY_08_SETUP                                 // uncomment if you're using JDY-08 (BLE module) based version
                                                       // JDY_08_SETUP and JDY_10_SETUP use "AT+PASS" and 6 digits for setting pin (instead of "AT+PIN" and 4 digits like HC-06)
                                                       // and requires "AT+ISCEN1" to activate pairing password at all (password for pairing is 
                                                       // disabled by default because it's problematic)

/* 

        Debugging 

*/
//#define DEBUG                             //  Should be disabled if you're planning to exfiltrate data through serial
//#define WAIT_FOR_SERIAL_MONITOR_TO_OPEN   //  Othewise the debug output during initialization is lost


#ifndef DEBUG
  #define dbg(...) 0                      // replaces all dbg() calls with 0 so they don't occupy any memory after compilation
  #define dbgf(...) 0
#endif

#if defined(WAIT_FOR_SERIAL_MONITOR_TO_OPEN) && !defined(DEBUG)
  #error "WAIT_FOR_SERIAL_MONITOR_TO_OPEN was defined but DEBUG wasn't. Comment-out this error message if that was intended."
#endif
                                                     
/* 

        Wireless Module Related 

*/                                                    
#define PROGRAMMING_MODE_SWITCH_PIN 3    // 3 of Arduino Pro Micro, SCL, PD0 (18 of Atmega32u4)
/*
Thanks to this switch it will be possible to flash the Esp8266 without the need to upload this code:
https://gist.github.com/spacehuhn/b2b7d897550bc07b26da8464fa7f4b36

It is incorporated into supremeDuck.ino instead and is activated when the switch is turned on = less hassle with reprogramming Esp

For BLE and Bluetooth versions it allows sending AT commands using "Tools -> Serial Monitor"
 */

#define WIFI_DUCKY_GPIO_0_CONTROL_PIN 2           // 2 of Arduino Pro Micro, SDA, PD1 (19 of Atmega32u4)
#define WIFI_DUCKY_ENABLE_CONTROL_PIN 20          // 20 of Arduino Pro Micro, A2, PF5 (38 of Atmega32u4)

#if defined (WIFI_DUCKY_SETUP) || defined (JDY_08_SETUP) || defined (JDY_10_SETUP)
  #define USE_TX_RX_PINS                                //  TX/RX pins of pro micro (hardware serial) instead of 9/8 pins and software serial library 
  #define MODULE_BAUDRATE 115200                        // use higher baudrate for Esp8266 (better speed for plain text/ducky scripts) 
#else
  #define MODULE_BAUDRATE 9600                          //(default baud rate of hc-06) It's the speed of communication between Arduino and HC-06, it shouldn't be changed without additionally changing it on the HC-06.
#endif

#ifdef USE_TX_RX_PINS
  #define WIRELESS_MODULE_SERIAL &Serial1                           
#else
  #include <SoftwareSerial.h>
  #define WIRELESS_MODULE_SERIAL new SoftwareSerial(9,8)                
#endif                                                                                              

#define MAX_SERIAL_LENGTH 200            // Maximum lenght of data received from the wireless module. If you'd like to make it greater than 255 make sure to replace "byte" with "int" inside every "for loop"



/* 

        Main Board Functionality 

*/
#define ALT_TAB_AUTORELEASE 1000
#define LED_SHOW_ACTIVITY_TIME 100  // built-in LED (whenever wireless module provides new data/command)



/* 

        EEPROM 

*/ 

/*
  How this project is using EEPROM space:
  0 - tricky activation for classic rubber ducky plug and execute method (deactivated/commented out, can't be bool, has to be some predefined byte that normally would be unlikely to be there)
  1-300 - encoding (3x char arrays)
  301 - use multilang (bool)
  302-332 - encoding name (char array)
  333-363 - bluetooth name (char array)
  364-369 - bluetooth pin (4 bytes, 1 per digit)
  370-400 - BLE name (char array)
  401-408 - BLE pin (6 bytes, 1 per digit)

  1000 - requested BLE name change (int, equal to EEPROM_CHANGE_REQUESTED_ID if name change was requested)
  1004 - requested BLE pin change (int, equal to EEPROM_CHANGE_REQUESTED_ID if pin change was requested)
  1008 - requested Bluetooth name change (int, equal to EEPROM_CHANGE_REQUESTED_ID if name change was requested)
  1012 - requested Bluetooth pin change (int, equal to EEPROM_CHANGE_REQUESTED_ID if pin change was requested)
  1016 - use saved encoding (int equal to EEPROM_SAVED_ENCODING_AVAILABLE_ID if language encoding was already changed by the user and saved in eeprom)
 */
#define EEPROM_STARTING_ADDRESS_ENCODING_DESIRED 1 
#define EEPROM_STARTING_ADDRESS_ENCODING_USED 101
#define EEPROM_STARTING_ADDRESS_ENCODING_MODIFIER 201
#define EEPROM_ADDRESS_USE_MULTI_LANG_METHOD_WINDOWS 301
#define EEPROM_STARTING_ADDRESS_ENCODING_NAME 302
#define EEPROM_ADDRESS_BLUETOOTH_NAME 333
#define EEPROM_ADDRESS_BLUETOOTH_PIN 364
#define EEPROM_ADDRESS_BLE_NAME 370
#define EEPROM_ADDRESS_BLE_PIN 401

#define EEPROM_ADDRESS_REQUESTED_BLE_NAME_CHANGE 1000
#define EEPROM_ADDRESS_REQUESTED_BLE_PIN_CHANGE 1004
#define EEPROM_ADDRESS_REQUESTED_BLUETOOTH_NAME_CHANGE 1008
#define EEPROM_ADDRESS_REQUESTED_BLUETOOTH_PIN_CHANGE 1012
#define EEPROM_ADDRESS_ENCODING_AVAILABLE 1016   // address at which "778" will be written when the device will receive 
                                                 // its' first instruction to use some other encoding than the default US 
                                                 // (this way I'll know to load EEPROM instead of using default US after re-boot)
// string lengths
#define BLUETOOTH_NAME_SIZE 30
#define BLE_NAME_SIZE 30
#define ENCODING_NAME_SIZE 30

// arbitrary values that trigger some action if present in memory 
// (used instead of bool's because bool's could be in memory by accident, 
// values like 777 are less likely to be there if not indended)
#define EEPROM_CHANGE_REQUESTED_ID 777
#define EEPROM_SAVED_ENCODING_AVAILABLE_ID 778


/* 

        Encoding

*/ 
#define ENCODING_BYTE_DESIRED 0
#define ENCODING_BYTE_USED 1
#define ENCODING_BYTE_MODIFIER 2

#define ENCODING_SIZE 78


#endif
