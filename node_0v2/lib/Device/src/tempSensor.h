#ifndef tempSensor_h
#define tempSensor_h

#include <Arduino.h>

class TempSensor {
    public:
            TempSensor(int ADC_pin, int Low_pin, int High_pin);
            float get_temp();
};

#endif