#ifndef Gateway_h
#define Gateway_h
#define RH_MESH_MAX_MESSAGE_LEN 50

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
#include <string.h>
#include "sensorReading.h"
#include "wifi_transmit.h"



class gateway {
            
    public:
            gateway(RHMesh *manager);
            int sendRequest(int node_num, int time_to_wait);
            int upload_to_wifi(String message);
            uint8_t BUFFER[RH_MAX_MESSAGE_LEN] = "\n";
            

    private:
            String read_own_sensors();
            String LoRaTransmitt(int node_num);
};



#endif
