#ifndef moisture_sensor_h
#define moisture_sensor_h

#include <Arduino.h>


class Moisture {

    public:
            
            Moisture::Moisture(int plus, int neg, int adc);
            String Moisture::sensorReading();
            void Moisture::init();
            
    private:
            void Moisture::switchPWM();
            int voltageReading;
            
};






#endif