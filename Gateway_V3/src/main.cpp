#include <Arduino.h>
#include <WiFi.h>
#include <wifi_transmit.h>
#include <Gateway.h>
#include <Node.h>

#define RH_MESH_MAX_MESSAGE_LEN 50
// In this small artifical network of 4 nodes,
#define CLIENT_ADDRESS 1
#define SERVER0_ADDRESS 2
#define SERVER1_ADDRESS 3
#define SERVER2_ADDRESS 4

#define RFM95_CS 2
#define RFM95_RST 0
#define RFM95_INT 4

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

int node_num = 0;
int number_of_nodes = 3;
int time_to_wait = 500;
RH_RF95 driver(RFM95_CS, RFM95_INT);

RHMesh manager(driver, CLIENT_ADDRESS);


bool mode;
wifi_transmit device;

Node node(&manager);

gateway gway(&manager);

void setup() {
  
  if(device.init()) {
    mode = true;
  } else {
    mode = false;
  }
  // put your setup code here, to run once:
}

void loop() {
  if(mode == true ){
      gway.sendRequest(node_num, time_to_wait);
  } else { 
      node.transmit();
  }
}