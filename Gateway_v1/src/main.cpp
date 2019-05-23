#define RH_MESH_MAX_MESSAGE_LEN 50
// In this small artifical network of 4 nodes,
#define GATEWAY_ADDRESS 1
#define NODE0_ADDRESS 2
#define NODE1_ADDRESS 3
#define NODE2_ADDRESS 4
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

// SCK pin = 18
// MISO pin = 19
// MOSI pin = 23

#define RFM95_CS 2
#define RFM95_RST 0
#define RFM95_INT 4

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

/* LED pin */
int led = 5;
/* LED state */
byte state = LOW;

// Singleton instance of the radio driver
RH_RF95 driver(RFM95_CS, RFM95_INT);

RHMesh manager(driver, GATEWAY_ADDRESS);


void led_state(){
  state = !state;
  digitalWrite(led, state);
}

void setup() 
{
  
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  pinMode(led, OUTPUT);

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


char data[] = "And hello back to you from server1";
// Dont put this on the stack:
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
void loop()
{
  //Serial.println("Aloha FUNT");
  uint8_t len = sizeof(buf);
  uint8_t from;
//  led_state();
  
  if (manager.recvfromAck(buf, &len, &from))
  {
    Serial.print("got request from : 0x");
    Serial.print(from, HEX);
    Serial.print(": ");
    Serial.println((char*)buf);
    led_state();

    // Send a reply back to the originator client
    /*
    if (manager.sendtoWait((uint8_t *)data, sizeof(data), from) != RH_ROUTER_ERROR_NONE) {
      Serial.println("sendtoWait failed"); 
      // led_state();
    }
    */
    led_state();
  }
  //delay(1000);
}
