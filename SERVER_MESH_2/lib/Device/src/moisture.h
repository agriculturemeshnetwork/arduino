#ifndef moisture_h
#define moisture_h

#include <Arduino.h>

class MoistureSensor {
    public:
            MoistureSensor(int ADC_pin, int Low_pin, int High_pin);
            float get_moisture();
            int adc = 0;
            int high = 0;
            int low = 0;
            float Vcc = 4095;

            float resistance = 0;
            float cb = 0;
            int wr = 0;
            int short_delay = 50;
            int long_delay = 1000000;
};

#endif