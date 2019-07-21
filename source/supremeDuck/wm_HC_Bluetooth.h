#ifndef HC_BLUETOOTH_H
#define HC_BLUETOOTH_H

#include "WirelessModule.h"

class HC_Bluetooth : public WirelessModule{
private:


public:

  bool WasNameChangeRequested() override;      void ResetNameChangeRequestFlag() override;
  bool WasPasswordChangeRequested() override;  void ResetPasswordChangeRequestFlag() override;
  void ChangeName() override;
  void ChangePassword() override;
};










#endif
