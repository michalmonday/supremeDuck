#ifndef WIRELESS_MODULE_H
#define WIRELESS_MODULE_H

#include <Arduino.h>

#include "Definitions.h"

class WirelessModule{
private:

protected:

public:

  //Stream * ser;

  #ifdef USE_TX_RX_PINS
    HardwareSerial* ser;  
  #else
    SoftwareSerial* ser;
  #endif

  WirelessModule();
 
  
  int Receive(char *in_str);
  
  virtual void Handshake();
  virtual void EnterProgrammingMode();


  void ChangeNameAndPasswordIfRequested();

  virtual bool WasNameChangeRequested() = 0;
  virtual bool WasPasswordChangeRequested() = 0;
  virtual void ChangeName() = 0;
  virtual void ChangePassword() = 0;
  virtual void ResetNameChangeRequestFlag() = 0;
  virtual void ResetPasswordChangeRequestFlag() = 0;

  void Send(char * str);
  

  bool mouse_move_flag = false;
};


// why this is here? To save sketch space. Only one of these files needs to be included so there's no need to waste program storage space.
#ifdef WIFI_DUCKY_SETUP
  #include "wm_Esp8266.h"
#elif defined(JDY_08_SETUP) || defined (JDY_10_SETUP)
  #include "wm_JDY.h" 
#else
  #include "wm_HC_Bluetooth.h"
#endif


#endif
