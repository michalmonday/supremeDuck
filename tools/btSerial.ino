
/*  
 *  The code below allows for communication between: the user <-> PC <-> Arduino <-> Bluetooth module.
 *  The bluetooth module expects specific commands to be delivered to it that start with "AT".
 *  The commands used for changing bluetooth device name and pin are the following:
 *  AT+NAMEmyName
 *  AT+PIN1234
 *  
 *  The 2 commands above would change the name to "myName" and pin to "1234". (the pin should be 4 digits long) 
 *  After each command is submitted we should receive a response from the device, it should be saying: "OKsetname" and "OKsetpin"
 *  To actually input these commands the code below has to be flashed, then the "Serial Monitor" has to be opened 
 *  (at the right bottom corner of the serial monitor pick "No line ending" and "9600 baud")
 *  Then input the initial testing command "AT", it should reply with "OK" which is always kind of satisfying to see :P
 *  
 *  Note that for example on NEXUS 5 you have to pair/forget the device after changing its name for the changes to become visible. (that's just my observation)
 */
 

#define BAUD_RATE 9600

#include <SoftwareSerial.h>

//SoftwareSerial mySerial(15, 16); 
SoftwareSerial mySerial(9, 8); // (main)
//SoftwareSerial mySerial(8, 9);


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(BAUD_RATE);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  } 
  Serial.println("You can communicate between PC and arduino using serial"); // in this tutorial the communication will be used to set the name and pin of the bluetooth module

  // set the data rate for the SoftwareSerial port
  mySerial.begin(BAUD_RATE);
}

void loop() { // run over and over

  //read the bluetooth serial if it's available and write the content to the main USB serial (connected to PC and visible at "Tools -> Serial Monitor")
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }

  //read the input of the user (from Serial Monitor) and pass it into the bluetooth which expects "AT" commands 
  //the list of commands appropriate for the HC-06 bluetooth module is available at: http://www.micro4you.com/files/ElecFreaks/Bluetooth%20HC-06.pdf
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
