// This script reads the FMA1700 mass flow meter's 0-5VDC output and put it to the serial port
// for manual and automatic monitoring

#include <SoftwareSerial.h>

#define SOF '$'
#define SEN_NAME "MFM"
#define EOF '%'

#define MASS_FLOW_RATE_RATIO 46.0 // Nitrogen/CO2

double f = 10.0; //Sample frequency
unsigned long dt = (1 / f) * 1E6;
unsigned long cur, prev;

SoftwareSerial ssAlicat(10, 11, 1);
double co2Flow = 16.00;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  ssAlicat.begin(19200);
  //Initialize timing variables
  cur = micros();
  prev = cur;
}

void loop() {
  // put your main code here, to run repeatedly:
  cur = micros();
  if (cur - prev >= dt)
  {
    int x = analogRead(0);
    double v = 5.0 * x / 1023.0;
    //Convert voltage to mass flow rate of Nitrogen

    //Calculate desired mass flow rate of CO2

    //Send mass flow rate CO2 to alicat
    
    ssAlicat.print("AS");
    ssAlicat.print(co2Flow, 2);
    ssAlicat.print("\r");

    char buf[255];
    ssAlicat.readBytes(buf, ssAlicat.available());
    Serial.println(buf);
    /*Serial comms protocol naming is
     * SOF: $
     * Sensor name: MFM
     * X.XXXX voltage reading
     * EOF: %
     */
//    Serial.print(SOF);
//    Serial.print(SEN_NAME);
//    Serial.print(v, 4);
//    Serial.println(EOF);
    prev = cur;
  }
}
