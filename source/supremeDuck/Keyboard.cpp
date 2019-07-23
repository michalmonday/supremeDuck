/*
  Keyboard.cpp

  Copyright (c) 2015, Arduino LLC
  Original code (pre-library): Copyright (c) 2011, Peter Barrett

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Keyboard.h"

#if defined(_USING_HID)


#include <EEPROM.h>
#include "Funcs.h"

byte Encoding[3][ENCODING_SIZE] = {
  { 33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,77,81,87,89,90,91,92,93,94,95,96,97,105,109,113,119,121,122,123,124,125,126,127 },
  { 30,52,32,33,34,36,52,38,39,37,46,54,45,55,56,39,30,31,32,33,34,35,36,37,38,51,51,54,46,55,56,31,4,16,20,26,28,29,47,49,48,35,45,53,4,12,16,20,26,28,29,47,49,48,53,42 },
  { 2,2,2,2,2,2,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,2,2,2,2,2,2,2,2,2,0,0,0,2,2,0,0,0,0,0,0,0,0,2,2,2,2,0 },
};
char encoding_name[ENCODING_NAME_SIZE] = {"US"}; 

#define EXCEPTIONS_SIZE 24
char exceptions[EXCEPTIONS_SIZE] = {
  'y','Y',
  'z','Z',
  'q','Q',
  'a','A',
  'm','M',
  'w','W',
  'i','I',
  '0','1','2','3','4','5','6','7','8','9',
};
byte KEYPAD[10] = {234, 225, 226, 227, 228, 229, 230, 231, 232, 233};


//================================================================================
//================================================================================
//	Keyboard

static const uint8_t _hidReportDescriptor[] PROGMEM = {

  //  Keyboard
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)  // 47
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x02,                    //   REPORT_ID (2)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
   
  0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    
  0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    
  0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x73,                    //   LOGICAL_MAXIMUM (115)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    
  0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x73,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0,                          // END_COLLECTION
};

Keyboard_::Keyboard_(void) : 
  default_delay(0), 
  keypress_time(5),
  last_alt_tab_time(0)
{
	static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
	HID().AppendDescriptor(&node);
}

void Keyboard_::begin(void)
{
}

void Keyboard_::end(void)
{
}

void Keyboard_::sendReport(KeyReport* keys)
{
	HID().SendReport(2,keys,sizeof(KeyReport));
}

extern
const uint8_t _asciimap[128] PROGMEM;

#define SHIFT 0x80
const uint8_t _asciimap[128] =
{
	0x00,             // NUL
	0x00,             // SOH
	0x00,             // STX
	0x00,             // ETX
	0x00,             // EOT
	0x00,             // ENQ
	0x00,             // ACK  
	0x00,             // BEL
	0x2a,			// BS	Backspace
	0x2b,			// TAB	Tab
	0x28,			// LF	Enter
	0x00,             // VT 
	0x00,             // FF 
	0x00,             // CR 
	0x00,             // SO 
	0x00,             // SI 
	0x00,             // DEL
	0x00,             // DC1
	0x00,             // DC2
	0x00,             // DC3
	0x00,             // DC4
	0x00,             // NAK
	0x00,             // SYN
	0x00,             // ETB
	0x00,             // CAN
	0x00,             // EM 
	0x00,             // SUB
	0x00,             // ESC
	0x00,             // FS 
	0x00,             // GS 
	0x00,             // RS 
	0x00,             // US 

	0x2c,		   //  ' '
	0x1e|SHIFT,	   // !
	0x34|SHIFT,	   // "
	0x20|SHIFT,    // #
	0x21|SHIFT,    // $
	0x22|SHIFT,    // %
	0x24|SHIFT,    // &
	0x34,          // '
	0x26|SHIFT,    // (
	0x27|SHIFT,    // )
	0x25|SHIFT,    // *
	0x2e|SHIFT,    // +
	0x36,          // ,
	0x2d,          // -
	0x37,          // .
	0x38,          // /
	0x27,          // 0
	0x1e,          // 1
	0x1f,          // 2
	0x20,          // 3
	0x21,          // 4
	0x22,          // 5
	0x23,          // 6
	0x24,          // 7
	0x25,          // 8
	0x26,          // 9
	0x33|SHIFT,      // :
	0x33,          // ;
	0x36|SHIFT,      // <
	0x2e,          // =
	0x37|SHIFT,      // >
	0x38|SHIFT,      // ?
	0x1f|SHIFT,      // @
	0x04|SHIFT,      // A
	0x05|SHIFT,      // B
	0x06|SHIFT,      // C
	0x07|SHIFT,      // D
	0x08|SHIFT,      // E
	0x09|SHIFT,      // F
	0x0a|SHIFT,      // G
	0x0b|SHIFT,      // H
	0x0c|SHIFT,      // I
	0x0d|SHIFT,      // J
	0x0e|SHIFT,      // K
	0x0f|SHIFT,      // L
	0x10|SHIFT,      // M
	0x11|SHIFT,      // N
	0x12|SHIFT,      // O
	0x13|SHIFT,      // P
	0x14|SHIFT,      // Q
	0x15|SHIFT,      // R
	0x16|SHIFT,      // S
	0x17|SHIFT,      // T
	0x18|SHIFT,      // U
	0x19|SHIFT,      // V
	0x1a|SHIFT,      // W
	0x1b|SHIFT,      // X
	0x1c|SHIFT,      // Y
	0x1d|SHIFT,      // Z
	0x2f,          // [
	0x31,          // bslash
	0x30,          // ]
	0x23|SHIFT,    // ^
	0x2d|SHIFT,    // _
	0x35,          // `
	0x04,          // a
	0x05,          // b
	0x06,          // c
	0x07,          // d
	0x08,          // e
	0x09,          // f
	0x0a,          // g
	0x0b,          // h
	0x0c,          // i
	0x0d,          // j
	0x0e,          // k
	0x0f,          // l
	0x10,          // m
	0x11,          // n
	0x12,          // o
	0x13,          // p
	0x14,          // q
	0x15,          // r
	0x16,          // s
	0x17,          // t
	0x18,          // u
	0x19,          // v
	0x1a,          // w
	0x1b,          // x
	0x1c,          // y
	0x1d,          // z
	0x2f|SHIFT,    // {
	0x31|SHIFT,    // |
	0x30|SHIFT,    // }
	0x35|SHIFT,    // ~
	0				// DEL
};


uint8_t USBPutChar(uint8_t c);

// press() adds the specified key (printing, non-printing, or modifier)
// to the persistent key report and sends the report.  Because of the way 
// USB HID works, the host acts like the key remains pressed until we 
// call release(), releaseAll(), or otherwise clear the report and resend.
size_t Keyboard_::press(uint8_t k) 
{
	uint8_t i;
	if (k >= 136) {			// it's a non-printing key (not a modifier)
		k = k - 136;
	} else if (k >= 128) {	// it's a modifier key
		_keyReport.modifiers |= (1<<(k-128));
		k = 0;
	} else {				// it's a printing key
		k = pgm_read_byte(_asciimap + k);
		if (!k) {
			setWriteError();
			return 0;
		}
		if (k & 0x80) {						// it's a capital letter or other character reached with shift
			_keyReport.modifiers |= 0x02;	// the left shift modifier
			k &= 0x7F;
		}
	}
	
	// Add k to the key report only if it's not already present
	// and if there is an empty slot.
	if (_keyReport.keys[0] != k && _keyReport.keys[1] != k && 
		_keyReport.keys[2] != k && _keyReport.keys[3] != k &&
		_keyReport.keys[4] != k && _keyReport.keys[5] != k) {
		
		for (i=0; i<6; i++) {
			if (_keyReport.keys[i] == 0x00) {
				_keyReport.keys[i] = k;
				break;
			}
		}
		if (i == 6) {
			setWriteError();
			return 0;
		}	
	}
	sendReport(&_keyReport);
	return 1;
}

size_t Keyboard_::PressRaw(uint8_t k){
  uint8_t i;
  if (_keyReport.keys[0] != k && _keyReport.keys[1] != k && 
    _keyReport.keys[2] != k && _keyReport.keys[3] != k &&
    _keyReport.keys[4] != k && _keyReport.keys[5] != k) {
    
    for (i=0; i<6; i++) {
      if (_keyReport.keys[i] == 0x00) {
        _keyReport.keys[i] = k;
        break;
      }
    }
    if (i == 6) {
      setWriteError();
      return 0;
    } 
  }
  sendReport(&_keyReport);
  return 1;
}

size_t Keyboard_::ReleaseRaw(uint8_t k){
  uint8_t i;
  for (i=0; i<6; i++) {
    if (0 != k && _keyReport.keys[i] == k) {
      _keyReport.keys[i] = 0x00;
    }
  }
  sendReport(&_keyReport);
  return 1;
}

size_t Keyboard_::PressRawModifier(uint8_t k){
  _keyReport.modifiers |= k;
  sendReport(&_keyReport);
  return 1;
}

size_t Keyboard_::ReleaseRawModifier(uint8_t k){
  _keyReport.modifiers &= ~(k);
  sendReport(&_keyReport);
  return 1;
}

// release() takes the specified key out of the persistent key report and
// sends the report.  This tells the OS the key is no longer pressed and that
// it shouldn't be repeated any more.
size_t Keyboard_::release(uint8_t k) 
{
	uint8_t i;
	if (k >= 136) {			// it's a non-printing key (not a modifier)
		k = k - 136;
	} else if (k >= 128) {	// it's a modifier key
		_keyReport.modifiers &= ~(1<<(k-128));
		k = 0;
	} else {				// it's a printing key
		k = pgm_read_byte(_asciimap + k);
		if (!k) {
			return 0;
		}
		if (k & 0x80) {							// it's a capital letter or other character reached with shift
			_keyReport.modifiers &= ~(0x02);	// the left shift modifier
			k &= 0x7F;
		}
	}
	
	// Test the key report to see if k is present.  Clear it if it exists.
	// Check all positions in case the key is present more than once (which it shouldn't be)
	for (i=0; i<6; i++) {
		if (0 != k && _keyReport.keys[i] == k) {
			_keyReport.keys[i] = 0x00;
		}
	}

	sendReport(&_keyReport);
	return 1;
}

void Keyboard_::releaseAll(void)
{
	_keyReport.keys[0] = 0;
	_keyReport.keys[1] = 0;	
	_keyReport.keys[2] = 0;
	_keyReport.keys[3] = 0;	
	_keyReport.keys[4] = 0;
	_keyReport.keys[5] = 0;	
	_keyReport.modifiers = 0;
	sendReport(&_keyReport);
}

size_t Keyboard_::write(uint8_t c)
{
	uint8_t p = press(c);  // Keydown
	release(c);            // Keyup
	return p;              // just return the result of press() since release() almost always returns 1
}

size_t Keyboard_::write(const uint8_t *buffer, size_t size) {
	size_t n = 0;
	while (size--) {
		if (*buffer != '\r') {
			if (write(*buffer)) {
			  n++;
			} else {
			  break;
			}
		}
		buffer++;
	}
	return n;
}



void Keyboard_::Print(char *inStr)
{   
  int enc_index; 
  for(byte i=0; i<strlen(inStr); i++)               //for each character in the string
  {  
    if (use_alt_codes && !IsModifier(inStr[i]) && ((!isalnum(inStr[i]) && inStr[i] != ' ') || IsException(inStr[i])))       //if character is punctuation or requires different button to be pressed in different keyboard language settings then use alt+numpad method
    {
  
      byte hundreds = (byte)inStr[i] / 100;
      byte dozens = ((byte)inStr[i] - (hundreds*100)) / 10;
      byte singles = (byte)inStr[i] - (hundreds*100) - (dozens*10);

      /*
      Serial.print(hundreds);
      Serial.print(dozens);
      Serial.println(singles);
      */
    
      press(KEY_LEFT_ALT);
      PressRelease((char)KEYPAD[hundreds], keypress_time);
      PressRelease((char)KEYPAD[dozens], keypress_time);
      PressRelease((char)KEYPAD[singles], keypress_time);
      releaseAll();
      continue;
    }
  
    enc_index = GetKeyIndex(inStr[i], Encoding[ENCODING_BYTE_DESIRED]);
    if(enc_index < 256 && !IsModifier(inStr[i]))                            //256 means it's a key not present in the array (key which does not need a substitution because it is the same for any keyboard setting)
    {    
      if(Encoding[ENCODING_BYTE_MODIFIER][enc_index] > 0)
      {
        PressRawModifier(Encoding[ENCODING_BYTE_MODIFIER][enc_index]);
        delay(keypress_time);
      }

      PressRaw(Encoding[ENCODING_BYTE_USED][enc_index]);
      delay(keypress_time);     

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
      PressAndWait(inStr[i]); 
    }
    releaseAll();
  }
}

int Keyboard_::GetKeyIndex(byte c, byte* char_array)           // find and the position of the value in the array
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

void Keyboard_::PressRelease(char c, byte timeDelay)
{
  press(c);
  delay(timeDelay);
  release(c);
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


bool Keyboard_::IsException(char c)                    // check whether this character is one of these that have to be typed differently using other language settings
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


bool Keyboard_::IsModifier(char c)                   // is key like shift, alt, "GUI" key, etc.
{
  byte b = (byte)c;
  if((b >= 128 && b <=135) || (b >= 176 && b <=179) || (b >= 193 && b <=205) || (b >= 209 && b <=218))
  {
    return true;
  }
  return false;
}


bool Keyboard_::WasEncodingPreviouslySaved(){
 
  int num = 0;                                                     //this has to be changed to int and saved elsewhere in the EEPROM, it has to be certain value so it won't give false positives
  EEPROM.get(EEPROM_ADDRESS_ENCODING_AVAILABLE, num);

  dbgf(F("Keyboard_::WasEncodingPreviouslySaved - num = %d"), num); 
 
  return num == EEPROM_SAVED_ENCODING_AVAILABLE_ID;
}

void Keyboard_::LoadEncoding(){
  for(byte i=0;i<3; i++) {
    for(byte offset=0; offset<ENCODING_SIZE; offset++) {
      EEPROM.get(offset+1+(ENCODING_SIZE*i), Encoding[i][offset]);            //+1 because the 0 address holds trigger bool for tricky activation method
    }
  }
  EEPROM.get(EEPROM_STARTING_ADDRESS_ENCODING_NAME, encoding_name);
  
  dbgf(F("Keyboard_::LoadEncoding - encoding_name = %s"), encoding_name); 
}

void Keyboard_::UseAltCodes(bool decision){ 
  use_alt_codes = decision; 
  EEPROM.put(EEPROM_ADDRESS_USE_MULTI_LANG_METHOD_WINDOWS, use_alt_codes);

  dbgf(F("Keyboard_::UseAltCodes - use_alt_codes = %d"), use_alt_codes); 
}
 
void Keyboard_::LoadAltCodesUseState() {
  EEPROM.get(EEPROM_ADDRESS_USE_MULTI_LANG_METHOD_WINDOWS, use_alt_codes);

  dbgf(F("Keyboard_::LoadAltCodesUseState - use_alt_codes = %d"), use_alt_codes); 
}

/* previous functions */
/*
void SavedMultiLangMethodWindowsCheck()                   // check whether the MultiLang method was used before the device turned off last time (access EEPROM by using function "EEPROM.get(address, my_var);")
{
  EEPROM.get(EEPROM_ADDRESS_USE_MULTI_LANG_METHOD_WINDOWS, useMultiLangWindowsMethod);                //read from EEPROM (persistent memory of ATMEGA 32U4) to see whether it should use MultiLang method
}

void SavedEncodingAvailabilityCheck()                               //rewrites the default US encoding with the one which was used last time and saved to EEPROM
{
  int numCheck = 0;                                                     //this has to be changed to int and saved elsewhere in the EEPROM, it has to be certain value so it won't give false positives
  EEPROM.get(EEPROM_ADDRESS_ENCODING_AVAILABLE, numCheck);
  if(numCheck == EEPROM_SAVED_ENCODING_AVAILABLE_ID)
  {
    for(byte i=0;i<3; i++)
    {
      for(byte offset=0; offset<ENCODING_SIZE; offset++)
      {
        EEPROM.get(offset+1+(ENCODING_SIZE*i), Encoding[i][offset]);            //+1 because the 0 address holds trigger bool for tricky activation method
      }

    }

    EEPROM.get(EEPROM_STARTING_ADDRESS_ENCODING_NAME, encodingName);
  }
  else
  {

  }
}

*/

char * Keyboard_::GetEncodingName(){
  return encoding_name;
}

void Keyboard_::SetEncodingName(char * name_){
  strcpy(encoding_name, name_); 
  EEPROM.put(EEPROM_STARTING_ADDRESS_ENCODING_NAME, encoding_name); 
}

void Keyboard_::SetEncoding(int segment_index, char * enc_str){
  for(byte offset = 0; offset < strlen(enc_str); offset += 2)
  {
    byte strValBuff[3]={enc_str[offset], enc_str[offset + 1],'\0'}; 

    Encoding[segment_index][offset / 2] = (byte)strtoul((char*)strtok(strValBuff, " "), NULL, 16);               //(((byte)inStr[offset]) * 16) + (byte)inStr[offset+1];         

  }  

  //save to EEPROM
  for(byte offset=0; offset<ENCODING_SIZE; offset++)
  {
    Encoding[segment_index][offset] = ((offset < (strlen(enc_str) / 2)) ? Encoding[segment_index][offset] : 0);
    EEPROM.put(offset + 1 + (ENCODING_SIZE * segment_index), Encoding[segment_index][offset]);                    //+1 because the 0 address holds trigger bool for tricky activation method
  }

  EEPROM.put(EEPROM_ADDRESS_ENCODING_AVAILABLE, EEPROM_SAVED_ENCODING_AVAILABLE_ID);
}





bool Keyboard_::IsItTimeTo_ReleaseAltTab(){
  return (last_alt_tab_time > 0 && (millis() - last_alt_tab_time > ALT_TAB_AUTORELEASE));
}

void Keyboard_::ReleaseAltTab(){ 
  last_alt_tab_time =  0;
  release(KEY_TAB);       // TODO: see if glitches occur, it used to be "Keyboard.releaseAll();" (as far as I remember it prevented glitch)
    
  release(KEY_LEFT_ALT);

}

void Keyboard_::AltTabOnce(){
  if(last_alt_tab_time == 0){
    PressAndWait(KEY_LEFT_ALT);
  }
  PressAndWait(KEY_TAB);
  release(KEY_TAB);
  last_alt_tab_time = millis();
}



/*  //Not needed but may stay
void Keyboard_::TypeKey(int key) {
  Keyboard.press(key);
  delay(keypress_time);
  Keyboard.release(key);
}

void Keyboard_::OpenRun() {
  Keyboard.press(KEY_LEFT_GUI);
  delay(200);
  Keyboard.press('r');
  delay(200);
  Keyboard.releaseAll();
  delay(700);
}

void Keyboard_::OpenCmd() {
  OpenRun();  
  Keyboard.Print("cmd");
  TypeKey(KEY_RETURN); 
}

void Keyboard_::EnterCommand(char *text) {
  Keyboard.Print(text);
  delay(10);
  TypeKey(KEY_RETURN);  
}

*/
















Keyboard_ Keyboard;

#endif
