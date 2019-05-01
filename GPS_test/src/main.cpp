#include <Arduino.h>

#define RXD2 16

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600,SERIAL_8N1, RXD2);

}

void loop() {
 
  while ( Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }
}