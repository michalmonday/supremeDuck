#include "wm_JDY.h"

#include <EEPROM.h>
#include "Funcs.h"

JDY::JDY(){
 
}


bool JDY::WasNameChangeRequested(){
  /* Make sure to add delay before this function to avoid accidentally sending AT command when unplugging (in case if board resets during unplug and reboots quickly)  */
  int num = 0;
  EEPROM.get(EEPROM_ADDRESS_REQUESTED_BLE_NAME_CHANGE, num);

  dbg(F("JDY::WasNameChangeRequested, num = "), String(num));
  
  return num == EEPROM_CHANGE_REQUESTED_ID;
}

void JDY::ResetNameChangeRequestFlag(){
  EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLE_NAME_CHANGE, 0);
}

bool JDY::WasPasswordChangeRequested(){
  /* Make sure to add delay before this function to avoid accidentally sending AT command when unplugging (in case if board resets during unplug and reboots quickly)  */
  int num;
  EEPROM.get(EEPROM_ADDRESS_REQUESTED_BLE_PIN_CHANGE, num);

  dbg(F("JDY::WasPasswordChangeRequested, num = "), String(num));
  
  return num == EEPROM_CHANGE_REQUESTED_ID;
}

void JDY::ResetPasswordChangeRequestFlag(){
  EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLE_PIN_CHANGE, (int)0);
}

void JDY::ChangeName(){
  char bleName[BLE_NAME_SIZE] = {0};
  EEPROM.get(EEPROM_ADDRESS_BLE_NAME, bleName);
  char at_cmd[BLE_NAME_SIZE + 17] = {0};
  
  #ifdef JDY_10_SETUP
    sprintf(at_cmd, "AT+NAME%s\r\n\0", bleName);
  #else                                                 // else JDY-08 (that doesn't use \r\n)
    sprintf(at_cmd, "AT+NAME%s\0", bleName);
  #endif
  
  delay(700);
  
  ser->print(at_cmd);

  dbg(F("JDY::ChangeName, at_cmd = "), at_cmd);
  
  delay(1000);

  if(ser->available()){
    dbg(F("JDY::ChangeName, at_cmd response = "), ser->readString());
  }else {
    dbg(F("JDY::ChangeName, at_cmd response not received."));
  }
  
  while(ser->available()){char c = ser->read();}
}

void JDY::ChangePassword(){
  #ifdef JDY_10_SETUP
    ser->print("AT+ISCEN1\r\n");
  #else
    ser->print("AT+ISCEN1");
  #endif
  
  delay(1000);

  if(ser->available()){ 
    dbg(F("JDY::ChangePassword, AT+ISCEN1 (enable pin) was sent to BLE module, response = "), ser->readString()); 
  } else {
    dbg(F("JDY::ChangePassword, AT+ISCEN1 (enable pin) was sent to BLE module, response not received"));
  }
  
  while(ser->available()){char c = ser->read();}
  
  char pin[7] = {0};
  EEPROM.get(EEPROM_ADDRESS_BLE_PIN, pin);
  char at_cmd[20] = {0};
  
  #ifdef JDY_10_SETUP
    sprintf(at_cmd, "AT+PASS%s\r\n\0", pin);
  #else                                                 // else JDY-08 (that doesn't use \r\n)
    sprintf(at_cmd, "AT+PASS%s\0", pin);
  #endif
  
  delay(700);
  ser->print(at_cmd);

  dbg(F("JDY::ChangePassword, at_cmd = "), at_cmd);
  
  delay(1000);
  
  if(ser->available()){
    dbg(F("JDY::ChangePassword, at_cmd response = "), ser->readString());
  }else {
    dbg(F("JDY::ChangePassword, at_cmd response not received."));
  }
  while(ser->available()){char c = ser->read();}
}










/* OLD function*/

/*
void ChangeBLECheck(){
  int numCheck = 0;

  
  EEPROM.get(EEPROM_ADDRESS_REQUESTED_BLE_NAME_CHANGE, numCheck);
  if(numCheck == 777)
  {
    delay(700);                                 // delay is there to avoid accidentally sending AT command when unplugging (in case if board resets during unplug and reboots quickly)
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
    
    ser->print(at_cmd);
    
    
    delay(1000);

    #ifdef LOG_SERIAL
      if(ser->available()){Serial.println("Changed BLE name (" + String(at_cmd) + "), response: " + ser->readString());}
      while(ser->available()){char c = ser->read();}                // just to be sure there's no bytes left
    #else
      while(ser->available()){char c = ser->read();}
    #endif
  }

  numCheck = 0;
  
  EEPROM.get(EEPROM_ADDRESS_REQUESTED_BLE_PIN_CHANGE, numCheck);
  if(numCheck == 777)
  {
    delay(700);                                 // delay is there to avoid accidentally sending AT command when unplugging (in case if board resets during unplug and reboots quickly)
    #ifdef JDY_10_SETUP
      ser->print("AT+ISCEN1\r\n");
    #else
      ser->print("AT+ISCEN1");
    #endif
    delay(1000);
    #ifdef LOG_SERIAL
      if(ser->available()){Serial.println("AT+ISCEN1 (enable pin) was sent to BLE module, response: " + ser->readString());}
      while(ser->available()){char c = ser->read();}                // just to be sure there's no bytes left
    #else
      while(ser->available()){char c = ser->read();}
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
    ser->print(at_pin_cmd);
    
    delay(1000);
    #ifdef LOG_SERIAL
      if(ser->available()){Serial.println("Changed BLE pin (" + String(at_pin_cmd) + "), response: " + ser->readString());}
      while(ser->available()){char c = ser->read();}                // just to be sure there's no bytes left
    #else
      while(ser->available()){char c = ser->read();}
    #endif
  }
}
*/
