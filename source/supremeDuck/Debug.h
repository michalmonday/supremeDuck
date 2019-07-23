#ifndef DEBUG_H
#define DEBUG_H

#include "Definitions.h"
#include <Arduino.h>

#ifdef DEBUG
  extern void dbg(const __FlashStringHelper *ifsh);
  extern void dbgf(const __FlashStringHelper *fmt_fsh, ...);
  extern void PrintSetupInfo();
#endif








#endif
