#include "Protocol.h"
#include "Definitions.h"
#include "WirelessModule.h"
#include "Keyboard.h"                           // library which provides all the functions to emulate Human Interface Device
#include "Mouse.h"                              // the same as above
#include "Funcs.h"

#include "FingerprintUSBHost.h"                 // for recognizing operating system of the target PC (Windows, Linux, Mac)

#include <EEPROM.h>
#include <Arduino.h>

void ExtractDeliveredText(char *str, byte ignoredStartingChars);

char GetEncodingSegmentIndex(char c);

ProtocolFunc protocol[] = {

  /* 
  
    commands NOT ended by: ",end"  (that's why there's "false")
  
  */
  
  {"Request_info", false, [](char * str){
      char data[70];
      
      String operating_system;
      FingerprintUSBHost.guessHostOS(operating_system);
      sprintf(data,"data=%i,%s,%s,end", Keyboard.IsUsingAltCodes(), Keyboard.GetEncodingName(), operating_system.c_str());           //format string
      wireless_module->Send(data); // send the data to the mobile app or any other bluetooth device that is connected to it right now    
    }},

  {"Ctrl_alt_del", false, [](char * str){
      Keyboard.releaseAll();
      delay(Keyboard.GetKeypressTime());
      Keyboard.PressAndWait(KEY_LEFT_CTRL);
      Keyboard.PressAndWait(KEY_LEFT_ALT);
      Keyboard.press(KEY_DELETE);
      delay(200);
      Keyboard.releaseAll();
   }},

   {"Right_click", false, [](char * str){
      Mouse.click(MOUSE_RIGHT);
   }},
   
   {"Disconnect", false, [](char * str){

   }},

   {"Alt+Tab", false, [](char * str){
      Keyboard.AltTabOnce();
   }},

   {"click", false, [](char * str){
      Mouse.click();
   }},

   {"VER", false, [](char * str){
      char data[13];
      sprintf(data,"ver=%s,end", APP_VERSION);          //format string
      wireless_module->ser->write(data);    
   }},

   {"ds_begin", false, [](char * str){
      Keyboard.SetDefaultDelay(0);
   }},

   {"ds_end", false, [](char * str){
      Keyboard.SetDefaultDelay(0);
   }},

   /* 

      commands ended by: ",end" 
   
   */

   {"PT:", true, [](char * str){
      Keyboard.Print(str);
      delay(Keyboard.GetDefaultDelay());
   }},

   {"TE:", true, [](char * str){
      Keyboard.Print(str);
      delay(Keyboard.GetKeypressTime());
      Keyboard.PressAndWait(KEY_RETURN);
      Keyboard.release(KEY_RETURN);   
   }},

   {"ML:", true, [](char * str){
      if(!strcmp(str,"Enabled")){
        Keyboard.UseAltCodes(true);
      }
      else if(!strcmp(str,"Disabled")){
        Keyboard.UseAltCodes(false);
      }
   }},
   
   /*
     * the encoding data is large so the app sends the data in 4 steps, first sends the desired characters, the ones that the user wants to type (marked by the letter "D")
     * then it sends the used characters, the ones that have to be "pressed" in order to type the desired characters while using a specific language settings (these "Used characters" are marked by the letter "U")
     * then it sends the modifier keys, they are used to know whether some shift or alt has to be pressed together with the "Used char" to achieve "Desired char" being typed on the target PC  
     * the last message sent is just a name of the language (e.g. UK - gb)
     * 
     * Let's pretend that I want to type the letter "n" which is 0x6D in ascii table. Let's assume hypothetically that in order to type that letter on a PC with a Japanese keyboard setting
     * I have to press "z" + shift that are respectively 0x7A in ascii table and 0x81 (according to not ascii table but this: https://www.arduino.cc/en/Reference/KeyboardModifiers)
     * The encoding data which will allow me to correctly type letter "c" in such case would be:
     * ENC,D:6D,end
     * ENC,U:7A,end
     * ENC,M:81,end
     * ENC,N:HypotheticalJapanese - hj,end
     * 
     * The above example would send the data required for 1 char, the implementation of this system sends no more than 78 bytes/chars at once.
   */
   {"ENC,", true, [](char * str){
      if(str[0] == 'N'){
        Keyboard.SetEncodingName(str + 2);
      }else{
        char segment_index = GetEncodingSegmentIndex(str[0]);
        if(segment_index >= 0){
          Keyboard.SetEncoding(segment_index, str + 2);
        }
      }
   }},

   {"PDK_HC:", true, [](char * str){
      str[4] = 0;
      Keyboard.PressAndWait(HexToChar(str));
      Keyboard.PressAndWait(str[3]);
      Keyboard.releaseAll(); 
      delay(Keyboard.GetDefaultDelay());
   }},

   {"PDK_HH:", true, [](char * str){       
      int key_1;
      int key_2;
      sscanf(str, "%X,%X", &key_1, &key_2);   
      Keyboard.PressAndWait(key_1);
      Keyboard.PressAndWait(key_2);
      Keyboard.releaseAll();  
      delay(Keyboard.GetDefaultDelay());
   }},

   {"PTK_HHC:", true, [](char * str){
      int key[3];
      sscanf(str, "%X,%X,%c", &key[0], &key[1], &key[2]);
      for(int i = 0; i < 3; ++i){
        Keyboard.PressAndWait(key[i]);     
      }
      Keyboard.releaseAll();  
      delay(Keyboard.GetDefaultDelay());
   }},

   {"PK:", true, [](char * str){
      str[1] = 0;
      Keyboard.Print(str[0]);
      delay(Keyboard.GetDefaultDelay());
   }},

   {"PKH:", true, [](char * str){
      char key[2] = {HexToChar(str), '\0'};
      Keyboard.Print(key);  
      delay(Keyboard.GetDefaultDelay()); 
   }},

   {"DELAY:", true, [](char * str){
      int val;
      sscanf(str, "%d", &val);
      delay(val);    
      delay(Keyboard.GetDefaultDelay());
   }},

   
   {"DEFAULTDELAY:", true, [](char * str){
      int d = 0;
      sscanf(str, "%d", &d); 
      Keyboard.SetDefaultDelay(d);
      delay(d);
   }},
   {"DEFAULT_DELAY:", true, [](char * str){int d = 0; sscanf(str, "%d", &d);  Keyboard.SetDefaultDelay(d); delay(d); }},

   {"CBN:", true, [](char * str){
      char bluetoothName[BLUETOOTH_NAME_SIZE] = {0};
      sprintf(bluetoothName, "%s\0", str);
      
      EEPROM.put(EEPROM_ADDRESS_BLUETOOTH_NAME, bluetoothName); 
      EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_NAME_CHANGE, EEPROM_CHANGE_REQUESTED_ID);   
   }},

   {"CBP:", true, [](char * str){
      char pin[5] = {0};
      sprintf(pin, "%s\0", str);
      
      EEPROM.put(EEPROM_ADDRESS_BLUETOOTH_PIN, pin); 
      EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_PIN_CHANGE, EEPROM_CHANGE_REQUESTED_ID);   
   }},

   {"CBLEN:", true, [](char * str){
      char bleName[BLE_NAME_SIZE] = {0};
      sprintf(bleName, "%s\0", str);
      
      EEPROM.put(EEPROM_ADDRESS_BLE_NAME, bleName); 
      EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLE_NAME_CHANGE, EEPROM_CHANGE_REQUESTED_ID);   
   }},

   {"CBLEP:", true, [](char * str){
      char pin[7] = {0};
      sprintf(pin, "%s\0", str);
      
      EEPROM.put(EEPROM_ADDRESS_BLE_PIN, pin); 
      EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLE_PIN_CHANGE, EEPROM_CHANGE_REQUESTED_ID);   
   }},

};

char GetEncodingSegmentIndex(char c){
  const char * segments_ids = "DUM";
  for (char i = 0; i < strlen(segments_ids); ++i){
    dbgf(F("GetEncodingSegmentIndex - segments_ids[i] = %c, c = %c"), segments_ids[i], c);
    if (segments_ids[i] == c){
      return i;
    }
  }
  
  dbgf(F("GetEncodingSegmentIndex - Incorrect encoding segment_index. c = %c (ERROR)"), c);
  delay(500);
  return -1;
}


namespace Protocol {
  int protocol_size = sizeof(protocol) / sizeof(ProtocolFunc);

  void Check(char *str){
    for(short i = 0; i < protocol_size; ++i){
      ProtocolFunc *p = &protocol[i];
      
      if(!p->has_ending && !strcmp(str, p->begining)){
          protocol[i].func(str);  
          
      } else if (IsCmd(str, p->begining)){    
          if(char * end_ptr = strstr(str, ",end")){
            *end_ptr = '\0'; 
            protocol[i].func(str + strlen(p->begining));
          }else{
            dbg(F("Protocol::Check - The ',end' wasn't found in the command. (ERROR)")); 
            delay(500);
            dbgf(F("Protocol::Check - str = %s"), str);
          }    
          
      }else {       
        continue;
      }
      
      dbgf(F("Protocol::Check - p->begining of executed ProtocolFunc = %s"), p->begining);
    }
  }
}
