/*
supremeDuck project - https://github.com/michalmonday/supremeDuck
Created by Michal Borowski

Last edited: 21/07/2019
*/

// The choice of Wifi/Bluetooth/BLE version was moved to Definitions.h

#define APP_Version "1.25"                            // It is used to compare it with the mobile app version.
                                                     // For example: 1.08 is compatible with 1.081 or 1.83148, 
                                                     // but it's not compatible with 1.09 or 1.091319 
                                                     // (in such case notification will be displayed in the app)
               
#include "Definitions.h"      // settings (e.g. choice of wifi/bluetooth)
#include "WirelessModule.h"
#include "Funcs.h"

#include <EEPROM.h>                               // Electrically Erasable Programmable Read-Only Memory, it allows to save some values that will prevail even when the device is disconnected from power.                                              

#include "FingerprintUSBHost.h"                 // for recognizing operating system of the target PC (Windows, Linux, Mac)
#include "Keyboard.h"                           // library which provides all the functions to emulate Human Interface Device
#include "Mouse.h"                              // the same as above


WirelessModule *wireless_module;
char serial_input[MAX_SERIAL_LENGTH];                 //it will contain the text sent from bluetooth module and received in arduino

void SetNewCharEncoding(char *str);

void setup()                                    // setup function is a part of every Arduino sketch, it gets called once at the begining
{     
  #ifdef DEBUG
    Serial.begin(115200);                           // begin serial communication so it's possible to use "Tools -> Serial Monitor" to see the debugging output
  #endif

  #ifdef WAIT_FOR_SERIAL_MONITOR_TO_OPEN
    while(!Serial){;}                               // this works only on Arduino Pro Mirco / Leonardo
  #endif

  #ifdef WIFI_DUCKY_SETUP
    wireless_module = new Esp8266();
  #elif defined(JDY_08_SETUP) || defined (JDY_10_SETUP)
    wireless_module = new JDY();
  #else
    wireless_module = new HC_Bluetooth();
  #endif

  pinMode(PROGRAMMING_MODE_SWITCH_PIN, INPUT_PULLUP);
  if(digitalRead(PROGRAMMING_MODE_SWITCH_PIN) == LOW){
    BlinkLED(3 /*blinks_count*/, 300 /*blink_lingth*/);
    wireless_module->EnterProgrammingMode();        // Keeps being in programming mode even when switch is toggled back. (toggle switch and replug to exit) 
  }
  
  wireless_module->Handshake();   // Implemented solely for the Esp8266 (which didn't boot into the right mode sometimes so it gets resetted until the handshake is successful)
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  Keyboard.begin();
  Mouse.begin();

  if (Keyboard.WasEncodingPreviouslySaved()){
    dbg(F("Encoding was previously saved."));
    Keyboard.LoadEncoding();
  }else {
    dbg(F("Encoding wasn't saved."));
  }

  Keyboard.LoadAltCodesUseState();
  
  wireless_module->ChangeNameAndPasswordIfRequested();

  delay(100); 
  digitalWrite(LED_BUILTIN, LOW);  
}


void loop()                                   // loop function is also a part of every Arduino sketch but gets called over again after it returns/finishes unlike "setup" function
{ 
  if(IsItTimeTo_TurnOffLed()){
    digitalWrite(LED_BUILTIN, LOW);
  }

  memset(serial_input, 0, MAX_SERIAL_LENGTH);  
  if(wireless_module->Receive(serial_input)){
  
    dbg(F("serial_input = "), serial_input);             //it's useful for checking what text arduino receives from android but it makes the mouse movement laggy if the serial monitor is closed 
    
    TurnLedOn();
    
    if(wireless_module->mouse_move_flag){
      wireless_module->mouse_move_flag = false;
      MouseMove(serial_input);
      return;   // don't send response and don't check protocol (to make mouse movement as smooth as possible, that's the purpose of mouse_move_flag)
    }
                        
    Check_Protocol(serial_input);                          // main checking function, all the functionality gets triggered there depending on what it received from the bluetooth module      
    wireless_module->Send("OK");                       // it wasn't necessary before, but the ducky script functionality requires the Arduino to say: "OK, I already typed the last line/key you've sent me, so you can send the next one", otherwise there would have to be a bigger delay   
  }

  if(Keyboard.IsItTimeTo_ReleaseAltTab()){ 
    Keyboard.ReleaseAltTab();
  }
}

void MouseMove(char *str) // str = MM:L,U,FF,FF,end // MM = mouse move, L = left, U = up, FF = horizontal speed, FF = vertical speed
{ 
  byte x, y;
  str[12] = '\0';
  y = (byte)strtoul(str + 10, NULL, 16);
  str[9] = '\0';
  x = (byte)strtoul(str + 7, NULL, 16);
  if (str[3] == 'L') { x = x * -1; }
  if (str[5] == 'D') { y = y * -1; }  
  Mouse.move(x, y, 0);
}

void Check_Protocol(char *str)
{       
    if(!strcmp(str, "Request_info")){
      char data[70];
      
      String operating_system;
      FingerprintUSBHost.guessHostOS(operating_system);
      sprintf(data,"data=%i,%s,%s,end", Keyboard.IsUsingAltCodes(), Keyboard.GetEncodingName(), operating_system.c_str());           //format string
      wireless_module->Send(data); // send the data to the mobile app or any other bluetooth device that is connected to it right now    
    }

    if(!strcmp(str, "Ctrl_alt_del")){
      Keyboard.releaseAll();
      delay(Keyboard.GetKeypressTime());
      Keyboard.PressAndWait(KEY_LEFT_CTRL);
      Keyboard.PressAndWait(KEY_LEFT_ALT);
      Keyboard.press(KEY_DELETE);
      delay(200);
      Keyboard.releaseAll();
    }

    if(!strcmp(str, "Right_click")){
      Mouse.click(MOUSE_RIGHT);
    }

    if(!strcmp(str, "Disconnect\0")){
      delay(1000); 
    }

    if(!strcmp(str, "Alt+Tab\0")){
        Keyboard.AltTabOnce(); // allows further continuation (releases tab immediately, releases alt after certain time if not used again)
    }
    
    
    if(!strcmp(str, "click")){Mouse.click();}
    
    if(!strcmp(str, "VER")){                            // if the mobile phone app asks what version of the code is used on Arduino, to make sure that the same it's not different from the mobile app
        char data[13];
        sprintf(data,"ver=%s,end", APP_Version);          //format string
        wireless_module->ser->write(data);                                  // send the data to the mobile app or any other bluetooth device that is connected to it right now
    }
    
    if(IsCmd(str, "PT:")){                     // plain text
        ExtractDeliveredText(str, 3);
        Keyboard.Print(str);
        delay(Keyboard.GetDefaultDelay());
    }
    
    if(IsCmd(str, "TE:")){                     //(direct text instruction but with enter at the end )    
        ExtractDeliveredText(str, 3);
        Keyboard.Print(str);
        delay(Keyboard.GetKeypressTime());
        Keyboard.PressAndWait(KEY_RETURN);
        Keyboard.release(KEY_RETURN);   
    }
    
    if(IsCmd(str ,"ML:")){                     // enable/disable multi lang method
        ExtractDeliveredText(str, 3);
        if(!strcmp(str,"Enabled")){
          Keyboard.UseAltCodes(true);
        }
        else if(!strcmp(str,"Disabled")){
          Keyboard.UseAltCodes(false);
        }
    }
      
    if(IsCmd(str, "ENC,")){SetNewCharEncoding(str);}

    // ducky script                                    
    //*  PDK_HC:FF,a,end    // Press double key: hex + char
    //*  PDK_HH:FF,FF,end   // Press double key: hex + hex
    //*  PK:a,end  // Press key char
    //*  PKH:FF,end // Press key hex
    //*  WAIT:5000,end // delay(5000);
    //*  PTK_HHH:FF,FF,a,end // Press triple key: hex + hex + char
    
    if(IsCmd(str, "PDK_HC:")){                   // press double key (when hex and char received)            
        ExtractDeliveredText(str, 7);
        str[4] = 0;
        Keyboard.PressAndWait(HexToChar(str));
        Keyboard.PressAndWait(str[3]);
        Keyboard.releaseAll(); 
        delay(Keyboard.GetDefaultDelay());
    }
    
    if(IsCmd(str, "PDK_HH:")){                   // press double key ( when hex and hex values received)         
        ExtractDeliveredText(str, 7);        
        int key_1;
        int key_2;
        sscanf(str, "%X,%X", &key_1, &key_2);   
        Keyboard.PressAndWait(key_1);
        Keyboard.PressAndWait(key_2);
        Keyboard.releaseAll();  
        delay(Keyboard.GetDefaultDelay());
    }

  if(IsCmd(str, "PTK_HHC:")){                  // Press triple key: hex + hex + char (e.g. CTRL ALT t - to open terminal on Ubuntu)     
        int key[3];
        sscanf(str, "%*[^:]:%X,%X,%c", &key[0], &key[1], &key[2]);
        for(int i = 0; i < 3; ++i){
          Keyboard.PressAndWait(key[i]);     
        }
        Keyboard.releaseAll();  
        delay(Keyboard.GetDefaultDelay());
    }

    if(IsCmd(str, "PK:")){                       // press key
        ExtractDeliveredText(str, 3);
        str[1] = 0;
        Keyboard.Print(str[0]);
        delay(Keyboard.GetDefaultDelay());
    }
    
    if(IsCmd(str, "PKH:")){                      // press key (when hex received)
        ExtractDeliveredText(str, 4);
        char key[2] = {HexToChar(str), '\0'};
        Keyboard.Print(key);  
        delay(Keyboard.GetDefaultDelay());  
    }
    
    if(IsCmd(str, "DELAY:")){
        //ExtractDeliveredText(str, 5);
        int val;
        sscanf(str, "%*[^:]:%d", &val);
        delay(val);    
        delay(Keyboard.GetDefaultDelay());
    }

    if(IsCmd(str, "DEFAULTDELAY:") || IsCmd(str, "DEFAULT_DELAY:")){
        int d = 0;
        sscanf(str, "%*[^:]:%d", &d); 
        Keyboard.SetDefaultDelay(d);
        delay(d);
    }

    if(!strcmp(str, "ds_begin") || !strcmp(str, "ds_end")){ // when ducky script begins
      Keyboard.SetDefaultDelay(0);
    }

    // END OF DUCKY SCRIPT COMMANDS


  if(IsCmd(str, "CBN:")){                     //CBN:supremeDuck,end (change bluetooth name, after next reboot)
      ExtractDeliveredText(str, 4);
      char bluetoothName[BLUETOOTH_NAME_SIZE] = {0};
      sprintf(bluetoothName, "%s\0", str);
      
      EEPROM.put(EEPROM_ADDRESS_BLUETOOTH_NAME, bluetoothName); 
      EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_NAME_CHANGE, EEPROM_CHANGE_REQUESTED_ID);   
  }

  if(IsCmd(str, "CBP:")){                     //CBP:3737,end (change bluetooth 4-digit pin, after next reboot)
      ExtractDeliveredText(str, 4);
      char pin[5] = {0};
      sprintf(pin, "%s\0", str);
      
      EEPROM.put(EEPROM_ADDRESS_BLUETOOTH_PIN, pin); 
      EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLUETOOTH_PIN_CHANGE, EEPROM_CHANGE_REQUESTED_ID);   
  }

  if(IsCmd(str, "CBLEN:")){                     //CBN:supremeDuck,end (change BLE name, after next reboot)
      ExtractDeliveredText(str, 6);
      char bleName[BLE_NAME_SIZE] = {0};
      sprintf(bleName, "%s\0", str);
      
      EEPROM.put(EEPROM_ADDRESS_BLE_NAME, bleName); 
      EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLE_NAME_CHANGE, EEPROM_CHANGE_REQUESTED_ID);   
  }

  if(IsCmd(str, "CBLEP:")){                     //CBP:373737,end (change BLE 6-digit pin, after next reboot)
      ExtractDeliveredText(str, 6);
      char pin[7] = {0};
      sprintf(pin, "%s\0", str);
      
      EEPROM.put(EEPROM_ADDRESS_BLE_PIN, pin); 
      EEPROM.put(EEPROM_ADDRESS_REQUESTED_BLE_PIN_CHANGE, EEPROM_CHANGE_REQUESTED_ID);   
  }
}

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
  byte segment_index = 255;
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
     * The above example would send the data required for 1 char, the implementation of this system sends no more than 72 bytes/chars at once.
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
