#ifndef ESP8266_H
#define ESP8266_H

#include "WirelessModule.h"

class Esp8266 : public WirelessModule{
private:
  void Reset();

  void SetEspMode(int mode);

public:

  Esp8266();
  
  void Handshake() override;
  void EnterProgrammingMode() override;

  // Esp gets changed directly from the app, no need to store data in EEPROM for it, so these overriden functions are doing nothing
  bool WasNameChangeRequested() override { return false; }      
  void ResetNameChangeRequestFlag() override {}
  bool WasPasswordChangeRequested() override { return false; }  
  void ResetPasswordChangeRequestFlag() override {}
  void ChangeName() override {}
  void ChangePassword() override {}
};










#endif
