#include "Gateway.h"
RHMesh *Managerptr;
#define manager (*Managerptr)

sensorReading readings;
wifi_transmit wyfy;
Blinker blink(5);

//uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];

gateway::gateway(RHMesh *Manager) {
    wyfy.init();
    Managerptr = Manager;
}


int gateway::upload_to_wifi(String message) {
    if(!wyfy.send_message(message)) {
        return -1; 
    }
    return 1;
}

int gateway::RecieveData() {
    char data[10] = "send data";
    Serial.println("Sending to manager_mesh_server2");
    // led_state();  
    // Send a message to a rf22_mesh_server
    // A route to the destination will be automatically discovered.
    uint8_t len = sizeof(BUFFER);
    uint8_t from;
//  led_state();
    String data;
    memcpy(data, BUFFER, len);
    if (manager.recvfromAck(BUFFER, &len, &from))
    {
      Serial.print("got request from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)BUFFER);
      blink.LED_STATE();


    if(!upload_to_wifi(data + read_own_sensors())) {
      return -1;
    }

    // Send a reply back to the originator client
    /*
    if (manager.sendtoWait((uint8_t *)data, sizeof(data), from) != RH_ROUTER_ERROR_NONE) {
      Serial.println("sendtoWait failed"); 
      // led_state();
    }
    */
   
    blink.LED_STATE();
  }
  //delay(1000);
  return 1;
}

String gateway::read_own_sensors() {
    String reading = readings.sensor_readings();
    return reading;
}






