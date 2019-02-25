/* This example shows how to use continuous mode to take
range measurements with the VL53L0X. It is based on
vl53l0x_ContinuousRanging_Example.c from the VL53L0X API.

The range readings are in units of mm. */

#include <Wire.h>
#include <VL53L0X.h>

const unsigned int SensorCount = 4;
unsigned int firstPin = 3;
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
  delay(500);
  for(int i=0; i<SensorCount; i++)  {
    digitalWrite(firstPin+i, LOW);
  }
  delay(500);
  
  for(int i=0; i<SensorCount; i++)  {
    digitalWrite(firstPin+i, HIGH);
    
    delay(100);
    sensors[i].init();
    //sensors[i].setTimeout(100);
    //sensors[i].setMeasurementTimingBudget(20000);
    sensors[i].setAddress(firstAddress+i);
  }
  for(int i=0; i<SensorCount; i++)  {
    String s = String("Sensor Address ");
    s += (i+1);
    s += (": ");
    s += (sensors[i].getAddress());
    Serial.println(s);
    sensors[i].startContinuous();
  }
  delay(1000);
}

void loop()
{
  String out = String();
  for(int i=0; i<SensorCount; i++)  {
    out += "Sensor ";
    out += i;
    out += ": ";
    int tmp = sensors[i].readRangeContinuousMillimeters();
    if(tmp<1000) {
      out += sensors[i].readRangeContinuousMillimeters();
    }
    else  {
      out += -1;
    }
    out += "\t\t";
    if (sensors[i].timeoutOccurred()) { Serial.println(" TIMEOUT"); }
  }
  Serial.println(out);

}
