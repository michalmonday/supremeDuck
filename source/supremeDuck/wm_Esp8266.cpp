#include "wm_Esp8266.h"
#include "Definitions.h"
#include "Funcs.h"
#include <Arduino.h>

#define ESP_MODE_WORKING 1
#define ESP_MODE_PROGRAMMING 2


Esp8266::Esp8266(){
  dbg(F("Esp8266::Esp8266"));
  
  pinMode(WIFI_DUCKY_GPIO_0_CONTROL_PIN, OUTPUT);
  pinMode(WIFI_DUCKY_ENABLE_CONTROL_PIN, OUTPUT);
}


void Esp8266::SetEspMode(int mode){
  dbg(F("Esp8266::SetEspMode, mode = "), String(mode));
  
  digitalWrite(WIFI_DUCKY_ENABLE_CONTROL_PIN, LOW); // set it low first to reset the Esp-12F
  delay(200);
  
  if(mode == ESP_MODE_WORKING){
    // set esp to working mode
    digitalWrite(WIFI_DUCKY_GPIO_0_CONTROL_PIN,HIGH); 
  }else if(mode == ESP_MODE_PROGRAMMING){
    digitalWrite(WIFI_DUCKY_GPIO_0_CONTROL_PIN, LOW);
  }
  
  delay(200);  
  digitalWrite(WIFI_DUCKY_ENABLE_CONTROL_PIN, HIGH);
}

void Esp8266::Handshake(){  
/*
This function is necessary for one reason: bug. For some reason sometimes Esp does not run the code. This function will make sure that Esp 
is actually running the code by resetting it if it isn't. (reseting is done by setting "EN"-controlling pin to LOW for a moment.
Keeping in mind that this bug happens rarely (around 1 in 15 plug-ins) I think that the solution will be effective (method was tested with separate code
to make sure that in the bug-occuring circumstances the reset method will actually result in code being run).
 */
  dbg(F("Esp8266::Handshake"));
  
  SetEspMode(ESP_MODE_WORKING);
  delay(1500); // (esp blinks for 1 second at the start)
  
  ser->setTimeout(100); // blocking time of readString

  bool esp_runs_code = false;
  while(!esp_runs_code){
    if(ser->available()){ser->readString();}
    
    // send control message
    ser->print(":you_ok?");

    delay(300);
    
    if(ser->available()){
      if(ser->readString().equals("im_ok")){
        esp_runs_code = true;
        dbg(F("Esp runs"));
      }
    }else{
      dbg(F("Esp didn't respond, resetting."));
    }

    if(!esp_runs_code){
      Reset();
      delay(2000);
    }
  }
}

void Esp8266::Reset(){
  dbg(F("Esp8266::Reset"));
  
  ser->end();
  digitalWrite(WIFI_DUCKY_ENABLE_CONTROL_PIN,LOW);
  delay(50);  
  digitalWrite(WIFI_DUCKY_ENABLE_CONTROL_PIN,HIGH);
  ser->begin(MODULE_BAUDRATE);
  ser->setTimeout(100);
}

void Esp8266::EnterProgrammingMode(){
  // If switch was activated then pass all data through Serial1 and allow programming Esp8266 with "Nodemcu Flasher" program.
  // If the BLE version is used then it allows sending AT commands through "Tools -> Serial Monitor"

  dbg(F("Esp8266::EnterProgrammingMode"));
  
  pinMode(WIFI_DUCKY_GPIO_0_CONTROL_PIN, OUTPUT);
  pinMode(WIFI_DUCKY_ENABLE_CONTROL_PIN, OUTPUT);

  #if !defined(LOG_SERIAL) && !defined(DEBUG)   // if LOG_SERIAL and DEBUG were not defined then Serial.begin was not called yet
    Serial.begin(115200);
  #endif
  
  SetEspMode(ESP_MODE_PROGRAMMING);

  /* Flash LED 3 times quickly to show that the constant loop was reached and the Esp8266 can be programmed.
    Device is unusable in this mode (this mode is for flashing esp only) and has to be replugged in order to work again (plugged out + switch turned the other side + plugged back in) */;

  while(true){
    while(ser->available()){
      Serial.write((uint8_t)ser->read());
    }
  
    while(Serial.available()){
      ser->write((uint8_t)Serial.read());
    }
  }
}
