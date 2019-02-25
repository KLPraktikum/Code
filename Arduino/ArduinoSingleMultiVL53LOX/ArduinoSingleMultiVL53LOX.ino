/* This example shows how to use continuous mode to take
range measurements with the VL53L0X. It is based on
vl53l0x_ContinuousRanging_Example.c from the VL53L0X API.

The range readings are in units of mm. */

#include <Wire.h>
#include <VL53L0X.h>

const unsigned int SensorCount = 2;
unsigned int firstPin = 4;
uint8_t firstAddress = 0x2A;

VL53L0X sensors[SensorCount];

void setup()
{
  Serial.begin(115200);
  for(int i=0; i<SensorCount; i++)  {
    pinMode(firstPin+i, HIGH);
  }
  

  //Put all sensors to sleep
  for(int i=0; i<SensorCount; i++)  {
    digitalWrite(firstPin+i, LOW);
  }
  delay(500);

  Wire.begin();

  //Wake all sensors
  for(int i=0; i<SensorCount; i++)  {
    digitalWrite(firstPin+i, HIGH);
  }
  
  for(int i=0; i<SensorCount; i++)  {
    digitalWrite(firstPin+i, HIGH);
    for(int j=1; j<SensorCount-i; j++)  {
       digitalWrite(firstPin+j, LOW);
    }
    delay(100);
    sensors[i].init();
    sensors[i].setTimeout(500);
    sensors[i].setAddress(firstAddress+i);
  }
  for(int i=0; i<SensorCount; i++)  {
    String s = String("Sensor Address ");
    s += (i+1);
    s += (": ");
    s += (sensors[i].getAddress());
    Serial.println(s);
    //sensors[i].startContinuous();
  }
  delay(1000);
  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
}

void loop()
{
  String out = String();
  for(int i=0; i<SensorCount; i++)  {
    out += "Sensor ";
    out += i;
    out += ": ";
    out += sensors[i].readRangeSingleMillimeters();
    out += "\t\t";
    if (sensors[i].timeoutOccurred()) { Serial.println(" TIMEOUT"); }
    delay(500);
  }
  Serial.println(out);

}
