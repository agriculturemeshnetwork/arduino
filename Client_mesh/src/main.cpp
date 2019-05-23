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
#include <wifi_transmit.h>


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

RHMesh manager(driver, CLIENT_ADDRESS);

wifi_transmit wyfy;

void led_state(){
  state = !state;
  digitalWrite(led, state);
}

void setup() 
{
  delay(4000);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  pinMode(led, OUTPUT);

  wyfy.init();

  while (!Serial);
  Serial.begin(115200);
  delay(4000);


  Serial.println("Begin MESH testing");
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  led_state();
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  led_state();

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


char data[] = "Hello World";
// Dont put this on the stack:
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];


void loop()
{
  Serial.println("Sending to Server 2");
  // led_state();  
  // Send a message to a rf22_mesh_server
  // A route to the destination will be automatically discovered.
  if (manager.sendtoWait((uint8_t *)data, sizeof(data), SERVER2_ADDRESS) == RH_ROUTER_ERROR_NONE)
  {
    // It has been reliably delivered to the next node.
    // Now wait for a reply from the ultimate server
    
    uint8_t len = sizeof(buf);
    uint8_t from;  
    
    //if(manager.recvfromAck(buf, &len, &from))
    if (manager.recvfromAckTimeout(buf, &len, 500, &from))
    {
      Serial.print("got reply from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);
      //led_state(); 
      String message = (char*)buf;
      
      wyfy.send_message(message);
    }
    else
    {
      Serial.println("No reply, is rf95_mesh_server0, rf95_mesh_server1 and rf95_mesh_server2 running?");
    }
    
  }
  else
     Serial.println("sendtoWait failed. Are the intermediate mesh servers running?");
  
  //delay(1000);
}
