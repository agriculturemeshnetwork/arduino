#include <moisture.h>

#define c_one                  0.0000000679
#define c_two                  0.005226
#define c_three                2.362
#define resistor               10000
#define r_one                  600
#define r_two                  1000

MoistureSensor::MoistureSensor(int ADC, int Low, int High) {
    adc = ADC;
    low = Low;
    high = High;

    pinMode(ADC, INPUT);
    pinMode(Low, PULLDOWN);
    pinMode(High,PULLUP);
    digitalWrite(Low, 0);
}

float MoistureSensor::get_moisture() {
    digitalWrite(high, 1);
    
    wr = micros() + short_delay;
    while (micros() < wr){}
    
    float voltage_in = analogRead(adc);

    wr = micros() + short_delay;
    while (micros() < wr){}
    
    digitalWrite(high, 0);
    
    
    Serial.print("VOLTAGE: "); 
    Serial.println(voltage_in);
    Serial.print("VOLTAGE RATIO: "); 
    float voltageRatio = float(4095)/float(voltage_in);
    Serial.println(voltageRatio);
    
    resistance = (voltageRatio  - 1) * resistor;

    Serial.print("RESISTANCE: ");
    Serial.println(resistance);
    
    if ( resistance < 600 ) {
      cb = 0;
    }
    else if ( resistance <= 1000 ) {
      cb = (resistance - 550)/50;
    }
    else {
      cb = c_one * pow(resistance, 2) + c_two * resistance + c_three;
    }
    
    
    Serial.print("Centibars: "); 
    Serial.println(cb);
    
    wr = micros() + long_delay;
    while (micros() < wr){}
    return cb;
    
}