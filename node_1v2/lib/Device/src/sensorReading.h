#ifndef sensorReading_h
#define sensorReading_h



#include <Arduino.h>
#include <string.h>
#include "tempSensor.h"

class sensorReading {
    
    public: 
            sensorReading();
            float soil_moisture();
            float ground_temp();
            int * environment_readings();
            String sensor_readings();

    private:
    
};

#endif