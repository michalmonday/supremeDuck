#include "WirelessModule.h"
#include "Funcs.h"

WirelessModule::WirelessModule(){
  ser = WIRELESS_MODULE_SERIAL;   // Either HarwareSerial (which is Serial1) or SoftwareSerial(9,8)
  ser->begin(MODULE_BAUDRATE);
  dbgf(F("WirelessModule::WirelessModule - MODULE_BAUDRATE = %ld"), (long)MODULE_BAUDRATE);
}

void WirelessModule::Handshake(){ 
  // so far (21/07/19) implemented only in one derived class: Esp8266

  dbg(F("WirelessModule::Handshake"));
}


void WirelessModule::Send(char * str){
  ser->print(str);
  dbgf(F("WirelessModule::Send - str = %s"), str);
} 

void WirelessModule::EnterProgrammingMode(){
  dbg(F("WirelessModule::EnterProgrammingMode"));
  delay(200);
  
  #ifndef DEBUG   // if DEBUG was not defined then Serial.begin was not called yet
    Serial.begin(115200);
  #endif
  
  while(true){
    while(ser->available()){
      Serial.write((uint8_t)ser->read());
    }
  
    while(Serial.available()){
      ser->write((uint8_t)Serial.read());
    }
  }
}


int WirelessModule::Receive(char *in_str){
  int i=0; 
  if (ser->available() > 0)               // check whether there's any data received by the bluetooth module
  {        
    unsigned long previousByteRec = millis(); unsigned long lastByteRec = millis();               // timing functions (needed for reliable reading of the BTser)
    while (100 > lastByteRec - previousByteRec)                                                   // if no further character was received during 100ms then proceed with the amount of chars that were already received (notice that "previousByteRec" gets updated only if a new char is received)
    {      
      lastByteRec = millis();   
      while (ser->available() > 0 && i < MAX_SERIAL_LENGTH - 1) 
      {
        in_str[i++]=ser->read();                                 // read bluetooth data (copy it to an array) 
        if(!in_str[i-1]){i--; continue;}                        // JDY-08 sends byte equal to 0 after transmiting a message, 
                                                                // cool thing but HC-06 doesn't have such thing so this ugly check 
                                                                // is used instead of neat solution using "0" as terminating byte

        if(i == 16 && !strncmp(in_str, "MM:", 3) && !strncmp(in_str + i - 4, ",end", 4)) // mouse movement check
        {
          mouse_move_flag = true;
          return i;
        }
        previousByteRec = lastByteRec;
      }    
    }
    in_str[i]= 0;                                     // end the string with 0
  }

  return i;
}

void WirelessModule::ChangeNameAndPasswordIfRequested(){
  dbg(F("WirelessModule::ChangeNameAndPasswordIfRequested"));
  
  if(WasNameChangeRequested() || WasPasswordChangeRequested()){
    delay(700); // to avoid overwritting if it gets accidentally disconnected (which possibly may happen multiple times during the first miliseconds/microseconds of plug in)

    if(WasNameChangeRequested()){
      dbg(F("Name change was requested"));
      ChangeName();
      ResetNameChangeRequestFlag();
    }
  
    if(WasPasswordChangeRequested()){
      dbg(F("Password change was requested"));
      ChangePassword();
      ResetPasswordChangeRequestFlag();
    }
  }  
}

WirelessModule *wireless_module;
