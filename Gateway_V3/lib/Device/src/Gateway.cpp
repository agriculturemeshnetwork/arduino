#include "Gateway.h"
RHMesh *Managerptr;
#define manager (*Managerptr)

sensorReading readings;
wifi_transmit wyfy;

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

int gateway::sendRequest(int node_num) {
    String buf  = LoRaTransmitt(node_num);
    if(!upload_to_wifi(buf)) {
        return -1;
    }
    return 1;
}

String gateway::read_own_sensors() {
    String reading = readings.sensor_readings();
    return reading;
}

String gateway::LoRaTransmitt(int node_num, int time_to_wait) {
    
    char data[10] = "send data";
    Serial.println("Sending to manager_mesh_server2");
    // led_state();  
    // Send a message to a rf22_mesh_server
    // A route to the destination will be automatically discovered.
    if (manager.sendtoWait((uint8_t *)data, sizeof(data), node_num) == RH_ROUTER_ERROR_NONE)
    {
    // It has been reliably delivered to the next node.
    // Now wait for a reply from the ultimate server
        uint8_t len = sizeof(BUFFER);
        uint8_t from;    
        if (manager.recvfromAckTimeout(BUFFER, &len, time_to_wait, &from))
        {
            Serial.print("got reply from : 0x");
            Serial.print(from, HEX);
            Serial.print(": ");
            Serial.println((char*)BUFFER);

            char buf[RH_MAX_MESSAGE_LEN];
            memcpy(buf, BUFFER, RH_MAX_MESSAGE_LEN);
            return buf;
            
        //led_state(); 
        } else {
            String error = "No reply, is rf22_mesh_server0, rf22_mesh_server1 and rf22_mesh_server2 running?";
            return error;
            
        }
    } else {
        String error = "sendtoWait failed. Are the intermediate mesh servers running?";
        return error;
    }
}







