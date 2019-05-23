#ifndef tempSensor_h
#define tempSensor_h

#include <Arduino.h>

class TempSensor {
    public:
            TempSensor(int ADC_pin, int Low_pin, int High_pin);
            float get_temp();
            float resistance = 0;
            float temp = 0;
            int adc = 0;
};

#endif