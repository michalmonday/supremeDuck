#include "Funcs.h"
#include "Definitions.h"

#include <Arduino.h>

#ifdef DEBUG
  #define DBG_PREFIX F("DEBUG: ")
  void dbg(char * s){ Serial.print(DBG_PREFIX); Serial.print(s); Serial.println(); }
  void dbg(String s){ Serial.print(DBG_PREFIX); Serial.print(s); Serial.println(); }
  void dbg(const __FlashStringHelper *ifsh){ Serial.print(DBG_PREFIX); Serial.print(ifsh); Serial.println(); }
  void dbg(const __FlashStringHelper *ifsh, String s){ Serial.print(DBG_PREFIX); Serial.print(ifsh); Serial.print(s); Serial.println(); }
  void dbg(const __FlashStringHelper *ifsh, char * s){ Serial.print(DBG_PREFIX); Serial.print(ifsh); Serial.print(s); Serial.println(); } 
#else
  // nothing here but in Definitions.h there is: #define dbg(...) 0
  // this way every dbg() will be replaced with zero which does nothing
  // so no additional memory will be occupied when DEBUG is undefined
#endif


// blocking
void BlinkLED(int blinks_count, int blink_length){
  for(byte i = 0; i < blinks_count; ++i){
    digitalWrite(LED_BUILTIN, HIGH); 
    delay(blink_length); 
    digitalWrite(LED_BUILTIN, LOW); 
    delay(blink_length); 
  }   
}


unsigned long led_turn_on_time = 0;

// non-blocking
void TurnLedOn(){
  digitalWrite(LED_BUILTIN, HIGH);
  led_turn_on_time = millis();
}

bool IsItTimeTo_TurnOffLed(){
  return (digitalRead(LED_BUILTIN) && (led_turn_on_time - millis() > LED_SHOW_ACTIVITY_TIME));
}


char HexToChar(char *in_str)                   // function which takes a pointer to a string with 2 characters like "FF" as a parameter and returns a number converted from that string, interpretting it as a 2 digit hexadecimal value 
{
  char buf[3]={ in_str[0], in_str[1], '\0' } ; 
  return (char)strtoul((char*)strtok(buf, " "), NULL, 16);
} 



int SubStrIndex(char *str, char *sfind)
{ 
  int ptr = strstr(str, sfind);
  if(ptr){
    return ptr - (int)str;
  }
  return 0;
}

bool IsCmd(char *str, char *cmdStart){
  return (StrStartsWith(str, cmdStart) && StrEndsWith(str, ",end"));
}

bool StrContains(char *str, char *sfind)
{
    return (bool)(strstr(str, sfind));
}

bool StrStartsWith(char* str, char* desiredStart)
{
  return (bool)(strstr(str, desiredStart) == str);
}

bool StrEndsWith(char* str, char* desiredEnd)
{
  return (bool)(strstr(str, desiredEnd) == (str + strlen(str) - strlen(desiredEnd)));
}
