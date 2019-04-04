
#define RH_MESH_MAX_MESSAGE_LEN 50
// In this small artifical network of 4 nodes,
#define CLIENT_ADDRESS 1
#define SERVER1_ADDRESS 2
#define SERVER2_ADDRESS 3
#define SERVER3_ADDRESS 4

#include <RHMesh.h>
#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <Esp.h>
#include <RadioHead.h>
#include <esp32-hal-cpu.h>
#include <esp32-hal-i2c.h>

// SCK pin = 18
// MISO pin = 19
// MOSI pin = 23

#define RFM95_CS 2
#define RFM95_RST 15
#define RFM95_INT 4

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

/* LED pin */
int led = 5;
/* LED state */
volatile byte state = LOW;

// Singleton instance of the radio driver
RH_RF95 driver(RFM95_CS, RFM95_INT);

RHMesh manager(driver, SERVER1_ADDRESS);

void setup() 
{
  delay(2000);
  Serial.begin(9600);
  if (!manager.init())
    Serial.println("RF22 init failed");
  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
}

char data[] = "And hello back to you from server1";
// Dont put this on the stack:
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
void loop()
{
  Serial.println();
  uint8_t len = sizeof(buf);
  uint8_t from;
  if (manager.recvfromAck(buf, &len, &from))
  {
    Serial.print("got request from : 0x");
    Serial.print(from, HEX);
    Serial.print(": ");
    Serial.println((char*)buf);

    // Send a reply back to the originator client
    if (manager.sendtoWait((uint8_t *)data, sizeof(data), from) != RH_ROUTER_ERROR_NONE)
      Serial.println("sendtoWait failed");
  }
  delay(1000);
}

