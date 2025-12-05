// This script reads the FMA1700 mass flow meter's 0-5VDC output and put it to the serial port
// for manual and automatic monitoring

#define SOF '$'
#define SEN_NAME "MFM"
#define EOF '%'

double f = 10.0; //Sample frequency
unsigned long dt = (1 / f) * 1E6;
unsigned long cur, prev;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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

    /*Serial comms protocol naming is
     * SOF: $
     * Sensor name: MFM
     * X.XXXX voltage reading
     * EOF: %
     */
    Serial.print(SOF);
    Serial.print(SEN_NAME);
    Serial.print(v, 4);
    Serial.println(EOF);
    prev = cur;
  }
}
