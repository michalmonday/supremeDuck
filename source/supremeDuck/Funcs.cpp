#include "Funcs.h"
#include "Definitions.h"
#include <Arduino.h>

#ifdef DEBUG

  unsigned long last_debug_line = 0;

  // prepend "DEBUG" and append time since last debug message
  #define DBG_PREFIX Serial.print(F("DEBUG: "))
  #define DBG_SUFFIX Serial.println(" (" + String(millis() - last_debug_line) + "ms)"); last_debug_line = millis()
  
  void dbg(char * s){ DBG_PREFIX;  Serial.print(s); DBG_SUFFIX; }
  void dbg(String s){ DBG_PREFIX; Serial.print(s); DBG_SUFFIX;}
  void dbg(const __FlashStringHelper *ifsh){ DBG_PREFIX; Serial.print(ifsh); DBG_SUFFIX; }
  void dbg(const __FlashStringHelper *ifsh, String s){ DBG_PREFIX; Serial.print(ifsh); Serial.print(s); DBG_SUFFIX; }
  void dbg(const __FlashStringHelper *ifsh, char * s){ DBG_PREFIX; Serial.print(ifsh); Serial.print(s); DBG_SUFFIX; } 
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




//    /* memory check*/
//    
//    //    dbg(F("Stack available = "), String(stackAvailable()));
//    //    dbg(F("Heap available = "), String(heapAvailable()));
//    struct __freelist
//    {
//      size_t sz;
//      struct __freelist *nx;
//    };
//    
//    // Posted by vbextreme - https://github.com/arduino/Arduino/issues/5289#issuecomment-242770325
//    unsigned int stackAvailable() 
//    {
//        extern int __heap_start, *__brkval; 
//        unsigned int v; 
//        return (unsigned int)&v - (__brkval == 0 ? (unsigned int)&__heap_start : (unsigned int)__brkval); 
//    }
//    
//    // Posted by vbextreme - https://github.com/arduino/Arduino/issues/5289#issuecomment-242770325
//    unsigned int heapAvailable()
//    {
//        unsigned int total = stackAvailable();
//    
//        struct __freelist* current;
//        extern struct __freelist *__flp;
//        for (current = __flp; current; current = current->nx)
//        {
//            total += 2; /* Add two bytes for the memory block's header  */
//            total += (unsigned int) current->sz;
//        }
//    
//        return total;
//    }
