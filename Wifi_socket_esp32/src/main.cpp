#include <wifi_transmit.h>
#include <Arduino.h>

#define LED 5

byte state = LOW;
wifi_transmit test;

void led_state(){
  state = !state;
  digitalWrite(LED, state);
}


void setup() {
    Serial.begin(111500);
    pinMode(LED, OUTPUT);    
    delay(3000);
    Serial.print("Begin Test");
}

void loop() {
    led_state();
    test.send_message("hello world");
    delay(3000);

}


