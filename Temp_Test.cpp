#include <Arduino.h>
#define c_one                   579.1
#define c_two                  -0.1241
#define c_three                -159.6
#define resistor                  10000
#define ADC_pin                 4
#define Vcc                    3.3

float resistance = 0;
float temp = 0;


void setup() {

  Serial.begin(115200);
  pinMode(ADC_pin, INPUT);

}


void loop() {

    int voltage_in = analogRead(ADC_pin);

    Serial.print("VOLTAGE: "); 
    Serial.println(voltage_in);

    resistance = ((Vcc/voltage_in)  - 1) * resistor;

    Serial.print("RESISTANCE: "); 
    Serial.println(resistance);

    temp = (c_one * pow(resistance, c_two)) + c_three;
    
    Serial.print("Temp: "); 
    Serial.println(temp);

}