#ifndef moisture_sensor_h
#define moisture_sensor_h

#include <Arduino.h>


class Moisture {

    public:

            Moisture(int plus, int neg, int adc);
            String sensorReading();
            void init();
            
    private:
            int voltageReading;
            
};




#endif