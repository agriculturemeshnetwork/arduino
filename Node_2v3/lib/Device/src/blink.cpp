
#include <blink.h>
//#include <Arduino.h>


Blinker::Blinker(int led_pin) {
    Blinker::LED_pin = led_pin;
    pinMode(led_pin, OUTPUT);
}

void Blinker::LED_STATE(){
    led_state = !led_state;
    Serial.println("Hello BLink....");
    digitalWrite(LED_pin, led_state);
}

