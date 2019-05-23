#define RH_MESH_MAX_MESSAGE_LEN 50
// In this small artifical network of 4 nodes,
#define CLIENT_ADDRESS 1
#define SERVER0_ADDRESS 2
#define SERVER1_ADDRESS 3
#define SERVER2_ADDRESS 4
//#define RH_TEST_NETWORK 1


#include <Arduino.h>
#include <RHMesh.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <Esp.h>
#include <RadioHead.h>
#include <esp32-hal-cpu.h>
#include <esp32-hal-i2c.h>
#include <RHGenericDriver.h>
#include <RHGenericSPI.h>
#include <moisture.h>
#include "tempSensor.h"
#include "ArduinoJson.h"
#include <WiFi.h>


// SCK pin = 18
// MISO pin = 19
// MOSI pin = 23

#define RFM95_CS 2
#define RFM95_RST 0
#define RFM95_INT 4

#define temp_ADC 27 
#define temp_High 26
#define temp_LOW  14

#define moist_ADC 35
#define moist_High 25
#define moist_LOW  34

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

/* LED pin */
//int led = 5;
/* LED state */
byte state = LOW;

// Singleton instance of the radio driver
RH_RF95 driver(RFM95_CS, RFM95_INT);

RHMesh manager(driver, SERVER2_ADDRESS);

const int capacity = JSON_OBJECT_SIZE(2);
StaticJsonDocument<capacity> doc;

TempSensor Temp(temp_ADC, temp_High, temp_LOW);
MoistureSensor Moist(moist_ADC, moist_High, moist_LOW);

void setup() 
{
  
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  //pinMode(led, OUTPUT);

  while (!Serial);
  Serial.begin(115200);
  delay(4000);


  Serial.println("Begin MESH testing");
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

 while (!driver.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  
  if (!driver.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  driver.setTxPower(23, false);

}




char data[RH_MESH_MAX_MESSAGE_LEN];
// Dont put this on the stack:
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];

long waitFor = 0;
void loop()
{ 
  uint8_t len = sizeof(buf);
  uint8_t from;
  if(waitFor == 0||millis()-waitFor==30000)
  {
    waitFor = millis();
    String json_string;
    doc["Tempature: "] = Temp.get_temp();
    doc["Moisture: "] =  Moist.get_moisture();

    serializeJson(doc, json_string);
    strcpy(data, json_string.c_str());
  }
  //Serial.println("Aloha FUNT");
 
//  led_state();
    
    

  if (manager.recvfromAck(buf, &len, &from))
  {
    Serial.print("got request from : 0x");
    Serial.print(from, HEX);
    Serial.print(": ");
    Serial.println((char*)buf);
    //led_state();

    // Send a reply back to the originator client
    if (manager.sendtoWait((uint8_t *)data, sizeof(data), CLIENT_ADDRESS) != RH_ROUTER_ERROR_NONE) {
      Serial.println("sendtoWait failed"); 
      // led_state();
    }
  }
  //delay(1000);
}
