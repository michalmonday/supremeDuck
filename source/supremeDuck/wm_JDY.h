#ifndef JDY_H
#define JDY_H

#include "WirelessModule.h"

class JDY : public WirelessModule{
private:

public:

  JDY();
  
  bool WasNameChangeRequested() override;      void ResetNameChangeRequestFlag() override;
  bool WasPasswordChangeRequested() override;  void ResetPasswordChangeRequestFlag() override;
  void ChangeName() override;
  void ChangePassword() override;
};






#endif
