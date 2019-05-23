
#include "tempSensor.h"

#define c_one                   579.1
#define c_two                  -0.1241
#define c_three                -159.6
#define resistor                10000
#define Vcc                     3.3


TempSensor::TempSensor(int ADC, int Low, int High) {
    adc = ADC;
    pinMode(ADC, INPUT);
    pinMode(Low, PULLDOWN);
    pinMode(High,PULLUP);
}

float TempSensor::get_temp() {
    //int voltage_in = analogRead(adc);

    float voltage_in = analogRead(adc);

    //Serial.print("VOLTAGE: "); 
    //Serial.println(voltage_in);
    //Serial.print("VOLTAGE RATIO: "); 
    float voltageRatio = float(4095)/float(voltage_in);
    Serial.println(voltageRatio);
    
    resistance = (voltageRatio  - 1) * resistor;

    //Serial.print("RESISTANCE: "); 
    //Serial.println(resistance);

    temp = (c_one * pow(resistance, c_two)) + c_three;
    
    //Serial.print("Temp: "); 
    //Serial.println(temp);
    
    //Serial.print("Temp: "); 
    //Serial.println(temp);
    return temp;
    
}