#include <Arduino.h>
#include "moisture_sensor.h"

#define plus_pin            25
#define neg_pin             35
#define ADC_pin             34

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