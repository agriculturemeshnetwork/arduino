#ifndef Node_h
#define Node_h

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


class Node {
    public:
            Node(RHMesh *manager);
            int transmit(int node_num);
            String data;
            uint8_t BUFFER[RH_MESH_MAX_MESSAGE_LEN];
    private:

};


#endif