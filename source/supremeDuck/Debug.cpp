#include "Debug.h"
#include "Protocol.h"

#ifdef DEBUG
  unsigned long last_debug_line = 0;

  /* prepend "dbg: " and append time since last debug message so it looks like:
  dgb: Message content (10ms)  */
  #define DBG_PREFIX Serial.print(F("dbg: "))
  #define DBG_SUFFIX Serial.println(" (" + String(millis() - last_debug_line) + "ms)"); last_debug_line = millis()
 
  void dbg(const __FlashStringHelper *ifsh){ 
    DBG_PREFIX; 
    Serial.print(ifsh); 
    DBG_SUFFIX; 
  }

  /*
      WRONG USE:
      dbgf(F("number = %d"), (long)1);
      dbgf(F("number = %d"), 100000);
      dbgf(F("number = %ld"), 1);
      dbgf(F("%s"), String("hello world"));

      CORRECT USE:
      dbgf(F("number = %d"), 1);
      dbgf(F("number = %ld"), 100000);
      dbgf(F("number = %ld"), (long)1);
      dbgf(F("number = %d"), (char)1);
      
      dbgf(F("%s"), "hello world");
      dbgf(F("%s"), String("hello world").c_str());

      dbgf(F("decimal 100 is %x in hex"), 100);
      dbgf(F("decimal 40000 is %lx in hex"), 40000);
  */
  void dbgf(const __FlashStringHelper *fmt_fsh, ...) {
    DBG_PREFIX; 
    
    va_list ap;
    va_start(ap, fmt_fsh);
    
    PGM_P fmt = reinterpret_cast<PGM_P>(fmt_fsh);

    while (char c = pgm_read_byte(fmt++)) {
      if (c == '%') {
        switch (c = pgm_read_byte(fmt++)) {
          case '%': Serial.write(c); break;
          case 'c': Serial.write(va_arg(ap, int)); break;                   // promoted
          
          case 'd': Serial.print(va_arg(ap, int), DEC); break; 
          case 'x': Serial.print(va_arg(ap, int), HEX); break; 
          case 'u': Serial.print(va_arg(ap, unsigned int), DEC); break; 
          case 'f': Serial.print(va_arg(ap, double)); break;                // promoted
          case 's': Serial.print(va_arg(ap, char*)); break; 
          
          case 'l': {
            switch (c = pgm_read_byte(fmt++)){
              case 'd': Serial.print(va_arg(ap, long), DEC); break; 
              case 'x': Serial.print(va_arg(ap, long), HEX); break; 
              case 'u': Serial.print(va_arg(ap, unsigned long), DEC); break;
              
              default:
                delay(500);
                dbgf(F("Wrong character after %%l (expecting '%%ld', '%%li', '%%lx' or '%%lu') in dbgf format string, char = %c (ERROR)"), c); 
                delay(500);
                break;
            }
            break;
          }
          
          default :
            delay(500);
            dbgf(F("Wrong character after %% sign in dbgf format string, char = %c (ERROR)"), c);
            delay(500);
            break;
        }
      }else {
        Serial.write(c);
      }
    }
    
    va_end(ap);
    DBG_SUFFIX;
  }

  void PrintSetupInfo(){
    dbgf(F("APP_VERSION = %s"), APP_VERSION);

    #ifdef WIFI_DUCKY_SETUP
      dbg(F("WIFI_DUCKY_SETUP is defined"));
      dbgf(F("WIFI_DUCKY_GPIO_0_CONTROL_PIN = %d"), WIFI_DUCKY_GPIO_0_CONTROL_PIN);
      dbgf(F("WIFI_DUCKY_ENABLE_CONTROL_PIN = %d"), WIFI_DUCKY_ENABLE_CONTROL_PIN);
    #endif

    #ifdef JDY_10_SETUP
      dbg(F("JDY_10_SETUP is defined"));
    #endif

    #ifdef JDY_08_SETUP
      dbg(F("JDY_08_SETUP is defined"));
    #endif

    #if !defined(WIFI_DUCKY_SETUP) && !defined(JDY_10_SETUP) && !defined(JDY_08_SETUP)
      dbg(F("Setting up default HC_Bluetooth version"));
    #endif

    #ifdef USE_TX_RX_PINS
      dbg(F("USE_TX_RX_PINS is defined"));
    #else 
      dbg(F("USE_TX_RX_PINS is not defined, using SoftwareSerial"));
    #endif

    dbgf(F("PROGRAMMING_MODE_SWITCH_PIN = %d"), PROGRAMMING_MODE_SWITCH_PIN);
    dbgf(F("digitalRead(PROGRAMMING_MODE_SWITCH_PIN) = %d"), digitalRead(PROGRAMMING_MODE_SWITCH_PIN));    
    dbgf(F("Protocol size = %d"), Protocol::protocol_size);
    
    Serial.println();
  }
#else
  // nothing here but in Definitions.h there is: #define dbg(...) 0
  // this way every dbg() will be replaced with zero which does nothing
  // so no additional memory will be occupied when DEBUG is undefined
#endif



/* 
  When using dbgf() function keep in mind that on Atmega32U4 programmed with Arduino:
  int has 16 bits (value from -32,768 to 32,767),                  https://www.arduino.cc/reference/en/language/variables/data-types/int/
  long has 32 bits (value from -2,147,483,648 to 2,147,483,647)    https://www.arduino.cc/reference/en/language/variables/data-types/long/

  Using the line below may result in incorrect value of the remaining parameters after "(long)1" being printed:
  dbgf(F("numbers = %d, %d"), (long)1, 1);
  Result: numbers = 1, 0

  That's because %d will "consume" only 16-bits by "va_arg(ap, int)" call, the remaining 16-bits of long 
  will be "consumed" in the next va_arg call that supposed to return the value of the next parameter. 
  
  Correct use would be:
  dbgf(F("numbers = %ld, %d"), (long)1, 1);

  Another example:
  dbgf(F("numbers = %d, %d"), 10000000, 1);
  Result: numbers = -27008, 152

  Correct use would be:
  dbgf(F("numbers = %ld, %d"), 10000000, 1); // %ld must be used because 10000000 is of type "long" (any number higher than 32,767 will be automatically passed as long)
  
  The same goes to the opposite situation where we use too big datatype identifier (e.g. "%ld") in format string,
  and too small datatype as an argument (e.g. char):
  dbgf(F("numbers = %ld, %ld"), (char)1, 100000);

  Result numbers = -2036334591, 82182145

  The correct use would be:
  dbgf(F("numbers = %d, %ld"), (char)1, 100000); 
  
  Despite char having 8-bits, it gets promoted to int (the same happens with float, being promoted to double, as described here:
  https://stackoverflow.com/a/11270603/4620679

*/
