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

void SetNewCharEncoding(char *str);

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
      sprintf(data,"ver=%s,end", APP_Version);          //format string
      wireless_module->ser->write(data);    
   }},

   /* 

      commands ended by: ",end" 
   
   */

   {"PT:", true, [](char * str){
      ExtractDeliveredText(str, 3);
      Keyboard.Print(str);
      delay(Keyboard.GetDefaultDelay());
   }},

   {"TE:", true, [](char * str){
      ExtractDeliveredText(str, 3);
      Keyboard.Print(str);
      delay(Keyboard.GetKeypressTime());
      Keyboard.PressAndWait(KEY_RETURN);
      Keyboard.release(KEY_RETURN);   
   }},

   {"ML:", true, [](char * str){
      ExtractDeliveredText(str, 3);
      if(!strcmp(str,"Enabled")){
        Keyboard.UseAltCodes(true);
      }
      else if(!strcmp(str,"Disabled")){
        Keyboard.UseAltCodes(false);
      }
   }},

   {"ENC,", true, [](char * str){
      SetNewCharEncoding(str);
   }},

   {"PDK_HC:", true, [](char * str){
      ExtractDeliveredText(str, 7);
      str[4] = 0;
      Keyboard.PressAndWait(HexToChar(str));
      Keyboard.PressAndWait(str[3]);
      Keyboard.releaseAll(); 
      delay(Keyboard.GetDefaultDelay());
   }},

   {"PDK_HH", true, [](char * str){
      ExtractDeliveredText(str, 7);        
      int key_1;
      int key_2;
      sscanf(str, "%X,%X", &key_1, &key_2);   
      Keyboard.PressAndWait(key_1);
      Keyboard.PressAndWait(key_2);
      Keyboard.releaseAll();  
      delay(Keyboard.GetDefaultDelay());
   }},

   {"PTK_HHC", true, [](char * str){
      int key[3];
      sscanf(str, "%*[^:]:%X,%X,%c", &key[0], &key[1], &key[2]);
      for(int i = 0; i < 3; ++i){
        Keyboard.PressAndWait(key[i]);     
      }
      Keyboard.releaseAll();  
      delay(Keyboard.GetDefaultDelay());
   }},

   {"PK:", true, [](char * str){
      ExtractDeliveredText(str, 3);
      str[1] = 0;
      Keyboard.Print(str[0]);
      delay(Keyboard.GetDefaultDelay());
   }},

   {"PKH:", true, [](char * str){
      ExtractDeliveredText(str, 4);
      char key[2] = {HexToChar(str), '\0'};
      Keyboard.Print(key);  
      delay(Keyboard.GetDefaultDelay()); 
   }},

   {"DELAY:", true, [](char * str){
      int val;
      sscanf(str, "%*[^:]:%d", &val);
      delay(val);    
      delay(Keyboard.GetDefaultDelay());
   }},

   
   {"DEFAULTDELAY:", true, [](char * str){
      int d = 0;
      sscanf(str, "%*[^:]:%d", &d); 
      Keyboard.SetDefaultDelay(d);
      delay(d);
   }},
   {"DEFAULT_DELAY:", true, [](char * str){int d = 0; sscanf(str, "%*[^:]:%d", &d);  Keyboard.SetDefaultDelay(d); delay(d); }},

   {"ds_begin", true, [](char * str){
      Keyboard.SetDefaultDelay(0);
   }},

   {"ds_end", true, [](char * str){
      Keyboard.SetDefaultDelay(0);
   }},

   {"CBN:", true, [](char * str){
      ExtractDeliveredText(str, 4);
      char bluetoothName[BLUETOOTH_NAME_SIZE] = {0};
      sprintf(bluetoothName, "%s\0", str);
      
      EEPROM.put(EEPROM_ADDRESS_BLUETOOTH_NAME, bluetoothName); 
      EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_NAME_CHANGE, EEPROM_CHANGE_REQUESTED_ID);   
   }},

   {"CBP:", true, [](char * str){
      ExtractDeliveredText(str, 4);
      char pin[5] = {0};
      sprintf(pin, "%s\0", str);
      
      EEPROM.put(EEPROM_ADDRESS_BLUETOOTH_PIN, pin); 
      EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_PIN_CHANGE, EEPROM_CHANGE_REQUESTED_ID);   
   }},

   {"CBLEN:", true, [](char * str){
      ExtractDeliveredText(str, 6);
      char bleName[BLE_NAME_SIZE] = {0};
      sprintf(bleName, "%s\0", str);
      
      EEPROM.put(EEPROM_ADDRESS_BLE_NAME, bleName); 
      EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLE_NAME_CHANGE, EEPROM_CHANGE_REQUESTED_ID);   
   }},

   {"CBLEP:", true, [](char * str){
      ExtractDeliveredText(str, 6);
      char pin[7] = {0};
      sprintf(pin, "%s\0", str);
      
      EEPROM.put(EEPROM_ADDRESS_BLE_PIN, pin); 
      EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLE_PIN_CHANGE, EEPROM_CHANGE_REQUESTED_ID);   
   }},

};


void ExtractDeliveredText(char *str, byte ignoredStartingChars)             //gets rid of the first few chars (e.g. "YT:") and the ",end" (it assumes that every message received ends with ",end", if the message would end like ",ending" the function won't work properly without adding additional parameter, somthing like: "byte ignoredEndingChars")
{
  byte len = SubStrIndex(str, ",end") - ignoredStartingChars;      
   
  for(byte i=0;i<=len;i++)
  {
    str[i] = str[i+ignoredStartingChars];
    if(i == len)
    {
      str[i] = '\0';
    }
  }
}

void SetNewCharEncoding(char *str){
  byte segment_index = 255;             // arbitrary high/invalid value
  switch (str[4])                       //5th letter is D, U or M depending on the type of 3 encoding segments
  {
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
    case 'D': 
      {
        segment_index = 0;
      }     
      break;
      
    case 'U':
      {
        segment_index = 1;  
      }
      break;
    case 'M':
      {
        segment_index = 2; 
      }
      break;
    case 'N':
      {
        ExtractDeliveredText(str, 6);
        Keyboard.SetEncodingName(str);
        return;
      }
      break;
    default:
      {
        dbg(F("Incorrect encoding segment_index. String(str[4]) = "), String(str[4]));
        return;
      }
      break;
  }   

  ExtractDeliveredText(str, 6); 
  Keyboard.SetEncoding(segment_index, str);
}

namespace Protocol {
  int protocol_size = sizeof(protocol) / sizeof(ProtocolFunc);

  void Check(char *str){
    for(short i = 0; i < protocol_size; ++i){
      ProtocolFunc *p = &protocol[i];
      if(!p->has_ending && !strcmp(str, p->begining)){
          protocol[i].func(str);  
      } else if (IsCmd(str, p->begining)){      
          protocol[i].func(str);
      }else {
        continue;
      }
      dbg(F("Protocol::Check, executed func = "), p->begining);
    }
  }
}
