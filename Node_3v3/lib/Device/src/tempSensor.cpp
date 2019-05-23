
#include "tempSensor.h"

#define c_one                   579.1
#define c_two                  -0.1241
#define c_three                -159.6
#define resistor                10000
#define Vcc                     3.3

float resistance = 0;
float temp = 0;
int adc = 0;
TempSensor::TempSensor(int ADC, int Low, int High) {
    adc = ADC;
    pinMode(ADC, INPUT);
    pinMode(Low, PULLDOWN);
    pinMode(High,PULLUP);
}

float get_temp() {
    int voltage_in = analogRead(adc);

    //Serial.print("VOLTAGE: "); 
    //Serial.println(voltage_in);

    resistance = -((Vcc/voltage_in)  - 1) * resistor;

    //Serial.print("RESISTANCE: "); 
    //Serial.println(resistance);

    temp = (c_one * pow(resistance, c_two)) + c_three;
    
    //Serial.print("Temp: "); 
    //Serial.println(temp);
    return temp;
    
}