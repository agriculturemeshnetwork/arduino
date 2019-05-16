#include "moisture_sensor.h"

hw_timer_t * timer1 = NULL;
hw_timer_t * timer2 = NULL;

volatile int pos_value = LOW;
volatile int neg_value = LOW;
unsigned long time;
int ground = 0;

volatile int exicitationControl = 10;
int value[10];

int totalInterruptCounter = 0;

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
    
    float average = 0;
    
    switchPWM();

    if( exicitationControl > 10 ) {

        for(int i = 0; i < 10; i++) {
            average = average + value[i];
        }

        average = float(average) / 10;

        digitalWrite(POS, 0);
        digitalWrite(NEG, 0);

        exicitationControl = 0;
    }

    return String(average);

}


void Moisture::switchPWM() {
    pos_value = HIGH;
    neg_value = LOW;
    for(int i =0; i < exicitationControl; i++) {

        digitalWrite(POS, !pos_value);
        digitalWrite(NEG, !neg_value);
        value[exicitationControl] = analogRead(ADC);

    }
    pos_value = LOW;
    neg_value = LOW;

}











