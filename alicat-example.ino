/*
This sketch is a demo for establishing serial communications with an RS-232 enabled Alicat device. The program monitors for data
in the serial buffer using the loop() function.

Physical connection to the Alicat:
Use 2 digital pins on the Arduino and the ground pin. The Rx pin on the Arduino should be connected
to the Tx pin on the Alicat, and the Tx pin on the Arduino should be connected to the Rx pin on the Alicat. Ground on the Alicat needs to be
shared with an Arduino pin. Do not use the Tx0 and Rx0 pins on the Arduino, these are reserved for the USB adapter.

Usage:
Use the serial monitor set to "No Line Ending".

Type the ASCII Alicat command into the "Message" dialog box in the serial monitor just as you would using another serial terminal.

Refer to https://documents.alicat.com/Alicat-Serial-Primer.pdf for a list of Alicat ASCII commands.

After sending the command, the sent message will be displayed, and the return from the Alicat will appear below.


ZW, v0.2 4/30/23
*/

#include <SoftwareSerial.h>

/*
Software serial required even if using a device with multiple hardware serial channels as the alicat uses RS-232 logic at TTL voltage levels.
This means high is 0, low is 1. Arduinos cannot invert logic levels on their serial native serial pins. Software Serial is an easy workaround without
using a hardware solution like a MAX3232 chip.

*/

// User Configuration: Arduino Tx/Rx pins, Alicat Baud Rate, Arduino baud rate

int ArdBaud = 19200;    // Baud rate for the serial monitor
int AlctBaud = 19200;   // Baud rate on the Alicat (see RS-232 settings), default is 19200

int TxPin = 11;  // Arduino Tx pin, connected to the Alicat Rx RS-232 pin (pin 3 on the 8 pin mini-DIN)
int RxPin = 10;  // Arduino Rx pin, connected to the Alicat Tx RS-232 pin (pin 5 on the 8 pin mini-DIN)

// end of User Config

SoftwareSerial mySerial(RxPin, TxPin, 1); // (Arduino Rx pin, Arduino Tx pin, Invert=true). 

char usrInput[255];      // an array to store user input
char AlicatOutput[255];  // an array to store Alicat returns
byte msgLen = 0;         // variable to store length of user inputs

boolean newData = false;    // flag to indicate new data has been stored into receivedChars
boolean newAData = false;   // flag to indicate new data has been stored into AlicatChars

void setup() {
  Serial.begin(ArdBaud);                  // Arduino Serial monitor interface
  mySerial.begin(AlctBaud);                // Alicat Serial interface (make sure baud rate matches Alicat setting)
  Serial.print("\n");
  Serial.println("<Arduino is ready>");
}

void loop() {
  recvUsr();        // function receives input from the Arduino USB serial monitor. Use the "No Line Ending" setting
  sendNewData();    // function sends the string recieved from recvUsr() and sends it to the Alicat
  recvAlicat();     // function receives data from the Alicat 
  DisplayAlicat();  // function displays data received from the Alicat
}

void recvUsr() {
  byte ndx = 0;
  char rc;
  if (Serial.available() > 0 && newData == false) {   // check if serial buffer has data and if "userInput" has unsent data
    delay(50);                                        // delay to allow full data string to arrive into the serial buffer, about 96 characters at 19200 baud
    while (Serial.available() > 0) {                  // read serial buffer into usrInput
      rc = Serial.read();
      usrInput[ndx] = rc;
      ndx++;
    }
    usrInput[ndx] = '\r';       // Alicat's need a CR after ASCII commands
    usrInput[(ndx + 1)] = '\0'; // ignore end of the receivedChars string from previously received data
    msgLen = ndx + 1;           // Length of sent message
    newData = true;             // Flag set to indicate a new message has been received from the Serial Monitor
  }
}


void recvAlicat() {
  byte ndx = 0;
  char rc;
  if (mySerial.available() > 0 && newAData == false) {  // check if data is waiting in serial buffer and if undisplayed data is in AlicatOutput
    delay(100);                                          // delay to allow full data string to arrive into the serial buffer, about 96 characters at 19200 baud
    while (mySerial.available()) {                      // read data from Alicat serial buffer into AlicatOutput
      rc = mySerial.read();
      AlicatOutput[ndx] = rc;
      ndx++;
    }
    AlicatOutput[ndx] = "\0";                           // ignore end of AlicatOutput data from previously received data
    newAData = true;
  }
}


void sendNewData() {
  if (newData == true) {
    for(int i = 0; i < msgLen; i++) {   // send characters up to "\0" from userInput
      mySerial.print(usrInput[i]);
    }
    Serial.print("Sent: ");
    Serial.println(usrInput);
    newData = false;
  }
}

void DisplayAlicat() {
  if (newAData == true) {
    Serial.print("Received: ");
    Serial.println(AlicatOutput);
    newAData = false;
  }
}
