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

int Node::transmit() {
    Node::data = sensor.sensor_readings();
    uint8_t len = sizeof(Node::buffer);
    uint8_t from;
//  led_state();
    if (manager.recvfromAck(Node::buffer, &len, &from)) {
        Serial.print("got request from : 0x");
        Serial.print(from, HEX);
        Serial.print(": ");
        Serial.println((char*)Node::buffer);
        //led_state();

        // Send a reply back to the originator client
        if (manager.sendtoWait((uint8_t *) Node::data.c_str(), sizeof(Node::data), from) != RH_ROUTER_ERROR_NONE) {
            Serial.println("sendtoWait failed"); 
            // led_state();
        }
        return 0;
    } else {
        return -1;
    }
    
}