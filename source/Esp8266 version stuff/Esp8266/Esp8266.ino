/*   It should be uploaded to Esp8266 (not Arduino) 
 *   The code below is based on WiFiAccessPoint example (https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiAccessPoint/WiFiAccessPoint.ino)
 * 
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <EEPROM.h>

/* Set these to your desired credentials. */
#define SSID_LEN 32
#define PASS_LEN 32
char ssid[SSID_LEN] = "supremeDuck";
char password[PASS_LEN] = "37373737";


#define EEPROM_ADDRESS_AP_NAME_CHECK 0
#define EEPROM_ADDRESS_AP_NAME 4

#define EEPROM_ADDRESS_AP_PASS_CHECK 36
#define EEPROM_ADDRESS_AP_PASS 40

void HandleData();
void Read_AP_EEPROM_Creds();

ESP8266WebServer server(80);

#define LED_PIN 2

bool handshake_done = false;
bool ok_received = false;

#define ARDUINO_RESPONSE_TIMEOUT 5000

String response;

void setup() {  
  Serial.setTimeout(100); // Serial.readString function blocking time
  
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  // 1 second blink to show it runs the code
  digitalWrite(LED_PIN, LOW);
  delay(1000);
  digitalWrite(LED_PIN, HIGH);

  //Serial.println();
  //Serial.print("Configuring access point...");
  
  Read_AP_EEPROM_Creds();             // read previously saved credentials (if by some chance they're incorrect then the last proper ones are used, that's how esp8266 seems to work)
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password); 

  IPAddress myIP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(myIP);
  server.on("/", [](){
    server.send(200, "text/plain", "Connected");
  });
  
  server.on("/connection_status", [](){
    server.send(200, "text/plain", "Connected");
  });

  server.on("/ap_name_change", HTTP_POST, [](){    
    if( ! server.hasArg("data") || server.arg("data") == NULL) { // If the POST request doesn't have username and password data
      server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    }else{
      EEPROM.begin(512);

      EEPROM.put(EEPROM_ADDRESS_AP_NAME_CHECK, (int)777);
      //EEPROM.put(EEPROM_ADDRESS_AP_NAME_LEN, (int)server.arg("data").length());
      char buff[32] = {0};
      strcpy(buff, String(server.arg("data")).c_str());
      EEPROM.put(EEPROM_ADDRESS_AP_NAME, buff);
      //EEPROM.put(EEPROM_ADDRESS_AP_NAME, String(server.arg("data")).c_str());     // idk why but it does not write to eeprom properly...
      EEPROM.commit();
      EEPROM.end();
      server.send(200, "text/plain", "OK");
    }
  });  

  server.on("/ap_pass_change", HTTP_POST, [](){
    if( ! server.hasArg("data") || server.arg("data") == NULL) { // If the POST request doesn't have username and password data
      server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    }else{
      EEPROM.begin(512);
      EEPROM.put(EEPROM_ADDRESS_AP_PASS_CHECK, (int)777);
      char buff[32] = {0};
      strcpy(buff, String(server.arg("data")).c_str());
      EEPROM.put(EEPROM_ADDRESS_AP_PASS, buff);
      EEPROM.commit();
      EEPROM.end();
      server.send(200, "text/plain", "OK");
    }
  });

  server.on("/ducky_script_ready_check", [](){   
    if(ok_received){
      server.send(200, "text/plain", "OK");
      ok_received = false;
    }else{
      server.send(200);
    }
  });

  server.on("/data", HTTP_POST, HandleData);
  
  server.onNotFound([](){
    server.send(404, "text/plain", "404: Not found");
  });
 
  server.begin();
  //Serial.println("HTTP server started");
}

void loop() {
  
  if(!handshake_done){
    if(Serial.available() >= 8){ // if first letter is not ':' then don't read it, means that the main chip can't send anything that starts with ":" to the mobile app... (ugly but I feel that it has to be in one way or another)
      String in_str = Serial.readString();
      if(in_str.indexOf(":you_ok?") >= 0){
        handshake_done = true;
        Serial.print("im_ok");    
        // 3 quick blinks (to let know that it communicated with the main chip well
        digitalWrite(LED_PIN, LOW); delay(50); digitalWrite(LED_PIN, HIGH); delay(50); digitalWrite(LED_PIN, LOW); delay(50); digitalWrite(LED_PIN, HIGH); delay(50); digitalWrite(LED_PIN, LOW); delay(50); digitalWrite(LED_PIN, HIGH); delay(50);   
      }
    }
    return;
  }
  
  server.handleClient();

  if(Serial.available()){
    String s = Serial.readString();
    if(s.startsWith("OK") || s.endsWith("OK")){
      ok_received = true; 
      
      if(s.startsWith("OK")){
        s.remove(0,2);
      }else{
        s.remove(s.length()-3);
      }
    }
    response += s;
  }
}

bool IsSerialAvailableBeforeTimeout(unsigned long timeout){
  unsigned long start_time = millis();
  while(millis() - start_time < timeout){
    if(Serial.available()){
      return true;
    }
    delay(1);
  }

  return false;
}

void HandleData(){
  /*
  Serial.print("\nPOST_DATA:");
  for(int i = 0; i < server.args(); i++){
    Serial.print(server.argName(i) + " : " + server.arg(i));
    Serial.println("");
  }
  Serial.print("HEADERS:");
  for(int i = 0; i < server.headers(); i++){
    Serial.print(server.headerName(i) + " : " + server.header(i));
    Serial.println("");
  }  
  */
  
  if( ! server.hasArg("data") || server.arg("data") == NULL) { // If the POST request doesn't have username and password data
    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    //Serial.print("400: Invalid Request");
    return;
  }
  
  Serial.print(server.arg("data"));     // pass data from mobile app to arduino
  ok_received = false;
  
  if(server.arg("data") == "VER" || server.arg("data") == "Request_info"){
    if(IsSerialAvailableBeforeTimeout(1000)){
      server.send(200, "text/plain", Serial.readString().c_str());
    }else{
      server.send(200);
    }   
  }else if(server.hasArg("response") && server.arg("response") == "1"){
    if(IsSerialAvailableBeforeTimeout(1000)){
      String res = Serial.readString();
      if(res.equals("OK")){
        if(IsSerialAvailableBeforeTimeout(ARDUINO_RESPONSE_TIMEOUT)){
          server.send(200, "text/plain", (res + response + Serial.readString()).c_str());
        }else{
          server.send(200, "text/plain", (res + response).c_str());
        }
      }else{
        server.send(200, "text/plain", (res + response).c_str());
      }  
    }else{
      server.send(200, "text/plain", response.c_str());
    }
    response = "";
  }else{
    server.send(200);
  }
}

void Read_AP_EEPROM_Creds(){
  EEPROM.begin(512);  
  int useCustomName=0;
  EEPROM.get(EEPROM_ADDRESS_AP_NAME_CHECK, useCustomName);
  Serial.println(String(useCustomName));
  if(useCustomName == 777){
    memset(ssid, 0, SSID_LEN);
    EEPROM.get(EEPROM_ADDRESS_AP_NAME, ssid);
  }
  int useCustomPass=0;
  EEPROM.get(EEPROM_ADDRESS_AP_PASS_CHECK, useCustomPass);
  if(useCustomPass == 777){
    memset(password, 0, PASS_LEN);
    EEPROM.get(EEPROM_ADDRESS_AP_PASS, password);
  } 
  EEPROM.end();
}
