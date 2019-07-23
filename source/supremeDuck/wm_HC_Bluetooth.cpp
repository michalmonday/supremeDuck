#include "wm_HC_Bluetooth.h"

#include <EEPROM.h>
#include "Funcs.h"


bool HC_Bluetooth::WasNameChangeRequested(){
  /* Make sure to add delay before this function to avoid accidentally sending AT command when unplugging (in case if board resets during unplug and reboots quickly)  */
  int num = 0;
  EEPROM.get(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_NAME_CHANGE, num);

  dbgf(F("HC_Bluetooth::WasNameChangeRequested - num = %d"), num);
  
  return num == EEPROM_CHANGE_REQUESTED_ID;
}

void HC_Bluetooth::ResetNameChangeRequestFlag(){
  EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_NAME_CHANGE, 0);
}

bool HC_Bluetooth::WasPasswordChangeRequested(){
  /* Make sure to add delay before this function to avoid accidentally sending AT command when unplugging (in case if board resets during unplug and reboots quickly)  */
  int num;
  EEPROM.get(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_PIN_CHANGE, num);
  
  dbgf(F("HC_Bluetooth::WasPasswordChangeRequested - num = %d"), num);
  return num == EEPROM_CHANGE_REQUESTED_ID;
}

void HC_Bluetooth::ResetPasswordChangeRequestFlag(){
  EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_PIN_CHANGE, (int)0);
}

void HC_Bluetooth::ChangeName(){
  char bluetoothName[BLUETOOTH_NAME_SIZE] = {0};
  //Serial.println(bluetoothName);
  EEPROM.get(EEPROM_ADDRESS_BLUETOOTH_NAME, bluetoothName);
  char at_cmd[BLUETOOTH_NAME_SIZE + 15] = {0};
  sprintf(at_cmd, "AT+NAME%s\0", bluetoothName);
  delay(700);

  ser->print(at_cmd);

  dbgf(F("HC_Bluetooth::ChangeName - at_cmd = %s"), at_cmd);
  
  delay(1000);
  if(ser->available()){
    dbgf(F("HC_Bluetooth::ChangeName - at_cmd response = %s"), ser->readString().c_str());
  }
  
  while(ser->available()){char c = ser->read();}
}

void HC_Bluetooth::ChangePassword(){
  char pin[5] = {0};
  EEPROM.get(EEPROM_ADDRESS_BLUETOOTH_PIN, pin);
  char at_cmd[16] = {0};
  sprintf(at_cmd, "AT+PIN%s\0", pin);
  delay(700);
  ser->print(at_cmd);

  dbgf(F("HC_Bluetooth::ChangePassword - at_cmd = %s"), at_cmd);

  delay(1000);
  
  if(ser->available()){
    dbgf(F("HC_Bluetooth::ChangePassword - at_cmd response = %s"), ser->readString().c_str());
  }
  
  while(ser->available()){char c = ser->read();}
}


/* OLD FUNCTION */

/*
void ChangeBluetoothCheck(){
  int numCheck = 0;

  
  EEPROM.get(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_NAME_CHANGE, numCheck);
  if(numCheck == 777)
  {
    delay(700);                                 // delay is there to avoid accidentally sending AT command when unplugging (in case if board resets during unplug and reboots quickly)
    EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_NAME_CHANGE, 0);
    char bluetoothName[BLUETOOTH_NAME_SIZE] = {0};
    //Serial.println(bluetoothName);
    EEPROM.get(EEPROM_ADDRESS_BLUETOOTH_NAME, bluetoothName);
    char at_cmd[BLUETOOTH_NAME_SIZE + 15] = {0};
    sprintf(at_cmd, "AT+NAME%s\0", bluetoothName);
    delay(700);
    
    wm->ser->print(at_cmd);
    
    
    delay(1000);
    #ifdef LOG_SERIAL
      if(wm->ser->available()){Serial.println("Changed bluetooth name (" + String(at_cmd) + "), response: " + wm->ser->readString());}
      while(wm->ser->available()){char c = wm->ser->read();}                // just to be sure there's no bytes left
    #else
      while(wm->ser->available()){char c = wm->ser->read();}
    #endif
  }

  numCheck = 0;
  
  EEPROM.get(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_PIN_CHANGE, numCheck);
  if(numCheck == 777)
  {
    delay(700);                                 // delay is there to avoid accidentally sending AT command when unplugging (in case if board resets during unplug and reboots quickly)
    EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_PIN_CHANGE, (int)0);
    char pin[5] = {0};
    EEPROM.get(EEPROM_ADDRESS_BLUETOOTH_PIN, pin);
    char at_pin_cmd[16] = {0};
    sprintf(at_pin_cmd, "AT+PIN%s\0", pin);
    delay(700);
    wm->ser->print(at_pin_cmd);

    delay(1000);
    
    #ifdef LOG_SERIAL
      if(wm->ser->available()){Serial.println("Changed bluetooth pin (" + String(at_pin_cmd) + "), response: " + wm->ser->readString());}
      while(wm->ser->available()){char c = wm->ser->read();}                // just to be sure there's no bytes left
    #else
      while(wm->ser->available()){char c = wm->ser->read();}
    #endif
  }
}
*/
