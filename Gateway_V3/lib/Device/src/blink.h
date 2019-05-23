#include <Arduino.h>

class Blinker {
    private:
            byte led_state = LOW;
            int LED_pin;
    public: 
            Blinker(int led_pin);
            void LED_STATE();
};