#ifndef FUNCS_H
#define FUNCS_H

#include "Debug.h"

extern void BlinkLED(int blinks_count, int blink_length); // blocking
extern void TurnLedOn(); // non-blocking
extern bool IsItTimeTo_TurnOffLed();

extern char HexToChar(char *in_str);
extern int SubStrIndex(char *str, char *sfind);
extern bool IsCmd(char *str, char *cmdStart);
extern bool StrContains(char *str, char *sfind);
extern bool StrStartsWith(char* str, char* desiredStart);
extern bool StrEndsWith(char* str, char* desiredEnd);


//extern unsigned int stackAvailable();
//extern unsigned int heapAvailable();


#endif
