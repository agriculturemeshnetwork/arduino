#include <Arduino.h>
#include "moisture_sensor.h"

// for ESP32 The Thing
//#define plus_pin            26
//#define neg_pin             27
//#define ADC_pin             14

// for Arduino Mega 2560
#define plus_pin            34
#define neg_pin             25
#define ADC_pin             35

Moisture moist(plus_pin, neg_pin, ADC_pin);

String moisture_reading = "";

void setup() {
  moist.init();
  Serial.begin(115200);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  moisture_reading = moist.sensorReading();
  Serial.println(moisture_reading);
  delay(2000);

}
