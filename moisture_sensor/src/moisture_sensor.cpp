#include "moisture_sensor.h"

#define resistor                10000
#define Vcc                     3.3



volatile int pos_value = LOW;
volatile int neg_value = LOW;
//unsigned long time;
int ground = 0;

int NEG;
int POS;
int ADC;

Moisture::Moisture(int pos, int neg, int adc) {
   
    NEG = neg;
    POS = pos;
    ADC = adc;

    pinMode(POS, OUTPUT);
    pinMode(NEG, OUTPUT);
    pinMode(ADC, INPUT);

}

void Moisture::init() {

    digitalWrite(POS, pos_value);
    digitalWrite(NEG, neg_value);
   
}

String Moisture::sensorReading() {
    
    int average = 0;
    
    //switchPWM();
    int voltage_in = analogRead(ADC);

    float resistance = 0;
    float temp = 0;
    //Serial.print("VOLTAGE: "); 
    //Serial.println(voltage_in);
    digitalWrite(POS, 1);
    delayMicroseconds(20);
    resistance = -((Vcc/voltage_in)  - 1) * resistor;
    
    delayMicroseconds(20);
    //average = analogRead(ADC);
    //Serial.print("Averaging: ");
    //Serial.println(average);
    
    digitalWrite(POS, 0);
    digitalWrite(NEG, 0);

        //exicitationControl = 0;
    
    Serial.print("resistance: ");
    Serial.println(resistance);
    return String(resistance);

}
