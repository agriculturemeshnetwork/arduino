#include <Arduino.h>
#define c_one                  0.00000000679
#define c_two                  0.005226
#define c_three                2.362
#define resistor               10000
#define r_one                  600
#define r_two                  1000
#define ADC_pin                27
#define High_pin               26
#define Low_pin                14

float Vcc = 4095;

float resistance = 0;
float cb = 0;
int wr = 0;
int short_delay = 50;
int long_delay = 1000000;

void setup() {

  Serial.begin(115200);
  pinMode(ADC_pin, INPUT);
  pinMode(High_pin, OUTPUT);
  pinMode(Low_pin, OUTPUT);
  digitalWrite(Low_pin, 0);
 
}


void loop() {
    
    digitalWrite(High_pin, 1);
    
    wr = micros() + short_delay;
    while (micros() < wr){}
    
    float voltage_in = analogRead(ADC_pin);

    wr = micros() + short_delay;
    while (micros() < wr){}
    
    digitalWrite(High_pin, 0);
    
    
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
}
