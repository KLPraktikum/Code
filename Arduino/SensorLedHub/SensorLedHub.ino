#include <Wire.h>
#include <VL53L0X.h>
#include <FastLED.h>
#include <SoftwareSerial.h>
#include <Chronos.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN    9
#define CLK_PIN   10
#define LED_TYPE    APA102
#define COLOR_ORDER BGR
#define NUM_LEDS    189
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

SoftwareSerial controller(11, 12);

const unsigned int SensorCount = 4;
const unsigned int firstPin = 3;
const uint8_t firstAddress = 0x2A;
const unsigned int maxMeasurement = 300; //Maximal valid measurement in mm
const unsigned int measurementTolerance = 10; //Allowed Sensor jitter before triggering
const unsigned int assignableLEDSegments = 2; //Number of LED segemnts assinged to every sensor

struct Sensor {
  VL53L0X sensor;
  unsigned int calibration; //Inactive Distance
  unsigned int LEDSegment[assignableLEDSegments]; //Start of the LED segment
  unsigned int LEDSegmentSize[assignableLEDSegments]; //Length of the LED segment
  CRGB color = CRGB::HotPink;
};

Sensor assembly[SensorCount];
bool initSensors();
void flashLeds(int selection);


void setup()
{
  assembly[0].LEDSegment[0] = 59;
  assembly[0].LEDSegment[1] = 179;
  assembly[0].LEDSegmentSize[0] = 12;
  assembly[0].LEDSegmentSize[1] = 12;
  assembly[0].color = CRGB::Yellow;

  assembly[1].LEDSegment[0] = 74;
  assembly[1].LEDSegment[1] = 163;
  assembly[1].LEDSegmentSize[0] = 12;
  assembly[1].LEDSegmentSize[1] = 12;
  assembly[1].color = CRGB::Blue;

  assembly[2].LEDSegment[0] = 90;
  assembly[2].LEDSegment[1] = 148;
  assembly[2].LEDSegmentSize[0] = 12;
  assembly[2].LEDSegmentSize[1] = 12;
  assembly[2].color = CRGB::Green;

  assembly[3].LEDSegment[0] = 105;
  assembly[3].LEDSegment[1] = 134;
  assembly[3].LEDSegmentSize[0] = 12;
  assembly[3].LEDSegmentSize[1] = 12;
  assembly[3].color = CRGB::Red;
  Serial.begin(115200);

  if (!initSensors()) {
    Serial.println("Sensors not connected properly!");
    while (1);
  }

  for (int i = 0; i < SensorCount; i++)  {
    String s = String("Sensor Address ");
    s += (i + 1);
    s += (": ");
    s += (assembly[i].sensor.getAddress());
    unsigned int testMeasurement = assembly[i].sensor.readRangeSingleMillimeters();
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(testMeasurement);
    if (testMeasurement == 65535)  {
      s += "\nSensor ";
      s += i;
      s += " not connected properly";
    }
    else if (testMeasurement > maxMeasurement) {
      s += "\nSensor ";
      s += i;
      s += " not aligned properly";
    }
    Serial.println(s);
  }

  //Calibrate Sensors
  for (int i = 0; i < SensorCount; i++)  {
    unsigned int calibration = 0;
    for (int j = 0; j < 10; j++) {
      calibration += assembly[i].sensor.readRangeSingleMillimeters();
      delay(100);
    }
    assembly[i].calibration = calibration / 10;
  }

  FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.println("Ready");
}

void loop() {
  //Serial.println("Awaiting Input");
  delay(100);
  if (Serial.available() > 0)  {
    //Serial.println("Processing Input");
    int selection = Serial.parseInt();
    while (Serial.available()) {
      Serial.read();
    }

    if (selection == 7)  {
      fill_rainbow(leds, NUM_LEDS, 15);
      FastLED.show();
      delay(1000);
      fill_solid(leds, NUM_LEDS, CHSV( 0, 0, 0));
      FastLED.show();
    }
    else if(selection < SensorCount)  {
      flashLeds(selection);
      assembly[selection].sensor.startContinuous();
      Chronos::DateTime timeout = Chronos::DateTime::now() + Chronos::Span::Seconds(10);
      bool success = false;
      while (Chronos::DateTime::now() < timeout && !success) {
        if (assembly[selection].sensor.readRangeContinuousMillimeters() <= assembly[selection].calibration - measurementTolerance) {
          Serial.println("s");
          success = true;
        }
      }
      if (!success)  {
        Serial.println("t");
      }
      assembly[selection].sensor.stopContinuous();
    }
  }

}

void flashLeds(int selection) {
  for (int i = 0; i < assignableLEDSegments; i++)  {
    for (int j = assembly[selection].LEDSegment[i]; j < assembly[selection].LEDSegment[i] + assembly[selection].LEDSegmentSize[i] - 1; j++) {
      leds[j] = assembly[selection].color;
    }
  }
  FastLED.show();
  delay(3000);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}



bool initSensors()  {
  //Wake all sensors
  for (int i = 0; i < SensorCount; i++)  {
    pinMode(firstPin + i, HIGH);
  }


  //Put all sensors to sleep
  for (int i = 0; i < SensorCount; i++)  {
    digitalWrite(firstPin + i, LOW);
  }
  delay(500);

  Wire.begin();

  //Wake all sensors
  for (int i = 0; i < SensorCount; i++)  {
    digitalWrite(firstPin + i, HIGH);
  }

  for (int i = 0; i < SensorCount; i++)  {
    digitalWrite(firstPin + i, HIGH);

    for (int j = i + 1; j < SensorCount - i; j++)  {
      digitalWrite(firstPin + j, LOW);
    }

    delay(100);
    if (!assembly[i].sensor.init())  {
      return false;
    }
    assembly[i].sensor.setTimeout(100);
    assembly[i].sensor.setMeasurementTimingBudget(20000);
    assembly[i].sensor.setAddress(firstAddress + i);
  }
  return true;
}
