/*
supremeDuck project - https://github.com/michalmonday/supremeDuck
Created by Michal Borowski

Last edited: 23/07/2019
*/

// The choice of Wifi/Bluetooth/BLE version was moved to Definitions.h
               
#include "Definitions.h"                        // settings (e.g. choice of wifi/bluetooth)
#include "WirelessModule.h"
#include "Funcs.h"
#include "Protocol.h"
#include "Keyboard.h"                           // modified a lot

#include <Mouse.h>
#include <EEPROM.h>                             /* Electrically Erasable Programmable Read-Only Memory, it allows to save some values 
                                                   that will prevail even when the device is disconnected from power. */                                         


void setup()                                    // setup function is a part of every Arduino sketch, it gets called once at the begining
{     
  #ifdef DEBUG
    Serial.begin(115200);                       // begin serial communication so it's possible to use "Tools -> Serial Monitor" to see the debugging output
  #endif

  #ifdef WAIT_FOR_SERIAL_MONITOR_TO_OPEN
    while(!Serial){;}                           // this "!Serial" check works only on Arduino Pro Mirco / Leonardo
    PrintSetupInfo();                           // from Debug.cpp
  #endif

  #ifdef WIFI_DUCKY_SETUP
    wireless_module = new Esp8266();
  #elif defined(JDY_08_SETUP) || defined (JDY_10_SETUP)
    wireless_module = new JDY();
  #else
    wireless_module = new HC_Bluetooth();
  #endif

  pinMode(PROGRAMMING_MODE_SWITCH_PIN, INPUT_PULLUP);
  if(digitalRead(PROGRAMMING_MODE_SWITCH_PIN) == LOW){
    BlinkLED(3 /*blinks_count*/, 300 /*blink_lingth*/);
    wireless_module->EnterProgrammingMode();  // Keeps being in programming mode even when switch is toggled back. (toggle switch and replug to exit) 
  }
  
  wireless_module->Handshake();               // Implemented solely for the Esp8266 (which didn't boot into the right mode sometimes so it gets resetted until the handshake is successful)
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  Keyboard.begin();
  Mouse.begin();

  if (Keyboard.WasEncodingPreviouslySaved()){
    dbg(F("setup - Encoding was previously saved."));
    Keyboard.LoadEncoding();
  }else {
    dbg(F("setup - Encoding wasn't saved."));
  }

  Keyboard.LoadAltCodesUseState();
  
  wireless_module->ChangeNameAndPasswordIfRequested();

  delay(100); 
  digitalWrite(LED_BUILTIN, LOW);  
}

void loop()                                   // loop function is also a part of every Arduino sketch but gets called over again after it returns/finishes unlike "setup" function
{ 
  if(IsItTimeTo_TurnOffLed()){
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  char serial_input[MAX_SERIAL_LENGTH] = {0};           /* it will contain the text sent from bluetooth module and received in arduino. "= {0}" works like: 
                                                           memset(serial_input, 0, MAX_SERIAL_LENGTH); */
                                                           
  if(wireless_module->Receive(serial_input)){  
    dbgf(F("loop - serial_input = %s"), serial_input);     /* it's useful for checking what text arduino receives from android but it makes the mouse movement 
                                                           laggy if the serial monitor is closed */ 
    TurnLedOn();
    
    if(wireless_module->mouse_move_flag){
      wireless_module->mouse_move_flag = false;
      MouseMove(serial_input);
      return;   // don't send response and don't check protocol (to make mouse movement as smooth as possible, that's the purpose of mouse_move_flag)
    }
    
    Protocol::Check(serial_input);
    wireless_module->Send("OK");                       /* it wasn't necessary before, but the ducky script functionality requires the Arduino to say: 
                                                          "OK, I already typed the last line/key you've sent me, so you can send the next one", 
                                                          otherwise there would have to be a bigger delay */
  }

  if(Keyboard.IsItTimeTo_ReleaseAltTab()){ 
    Keyboard.ReleaseAltTab();
  }
}

void MouseMove(char *str) // str = MM:L,U,FF,FF,end   // MM = mouse move, L = left, U = up, FF = horizontal speed, FF = vertical speed
{ 
  char x, y;
  str[12] = '\0';
  y = (char)strtoul(str + 10, NULL, 16);
  str[9] = '\0';
  x = (char)strtoul(str + 7, NULL, 16);
  if (str[3] == 'L') { x = x * -1; }
  if (str[5] == 'D') { y = y * -1; }  
  Mouse.move(x, y, 0);
}
