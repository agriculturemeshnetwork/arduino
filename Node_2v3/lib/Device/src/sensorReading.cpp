#include <sensorReading.h>
#include <string.h>
#include "tempSensor.h"

#define ADC_pin                 27
#define Low_pin                 14
#define High_pin                26




const int capacity = JSON_OBJECT_SIZE(2);
StaticJsonDocument<capacity> doc;

TempSensor temp(ADC_pin, Low_pin, High_pin);
sensorReading::sensorReading() {
    
}

float soil_moisture() {
    float moisture = 0;


    return moisture;
}

float ground_temp() {
    float ground_temp = 0;
    ground_temp = temp.get_temp();
    return ground_temp;
}

int * environment() {
    int *environment;
    
    return  environment;
}

String sensor_readings() {
    String json_string;
    doc["Tempature: "] = ground_temp();
    doc["Mositure: "] = soil_moisture();
    serializeJson(doc, json_string);
    return json_string;
}
