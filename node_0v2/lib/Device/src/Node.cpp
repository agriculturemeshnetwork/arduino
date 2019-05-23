#include <Node.h>
#include <sensorReading.h>
#include <RHMesh.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <Esp.h>
#include <RadioHead.h>
#include <esp32-hal-cpu.h>
#include <esp32-hal-i2c.h>
#include <RHGenericDriver.h>
#include <RHGenericSPI.h>
#include <string.h>

sensorReading sensor;
RHMesh *Managerptr;
#define manager (*Managerptr)

Node::Node(RHMesh *managerptr) {
    Managerptr = managerptr;
}

int Node::transmit(int node_num) {
    uint8_t len = sizeof(Node::BUFFER);
    uint8_t from;
//  led_state();
    Serial.println("Sending data to Gatway");
  // led_state();  
  // Send a message to a rf22_mesh_server
  // A route to the destination will be automatically discovered.
  if (manager.sendtoWait((uint8_t *)sensor.sensor_readings().c_str(), sizeof(data), node_num) == RH_ROUTER_ERROR_NONE)
  {
    // It has been reliably delivered to the next node.
    // Now wait for a reply from the ultimate server
    
   // uint8_t len = sizeof(BUFFER);
   // uint8_t from;  
    /*
    //if(manager.recvfromAck(buf, &len, &from))
    if (manager.recvfromAckTimeout(buf, &len, 10000, &from))
    {
      Serial.print("got reply from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);
      //led_state(); 
    }
    else
    {
      Serial.println("No reply, is rf95_mesh_node0, rf95_mesh_node1 and rf95_mesh_node2 running?");
    }
    */
  }
  else
     Serial.println("sendtoWait failed. Are the intermediate mesh servers running?");
  
  //delay(5000);
    
}