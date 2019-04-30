// Arduino9x_rx
// -*- mode: C++ -*-
// This sketch is receiver part of a pair of transmit test 
// (the other is the sender 'TX_Test_Range') for checking a range
// of LoRa parameters and a signal strength (RSSI)
// measurement. This code reply's the RSSI of a transmitted 
// signal and changes LoRa transmission configurations
// when prompted by the sender. The configuration parameters
// are transmit power, bandwidth, and spreading factor.
// I borrowed heavily for this code, but it is authored by
// Aric Gerspacher

#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <Esp.h>
#include <RadioHead.h>
#include <RHMesh.h>
#include <string.h>
//#include <cstring.h>

#define HexCon(x) = (x <= 9 ? '0' + x : 'A' - 10 + x ); // single in to hex conversion

String serialOut;
String rx;
//spreading factor definition (7 entries)
uint8_t SF[] = {6, 7, 8, 9, 10, 11, 12};

//bandwidth definitions (10 entries)
long BW[] = {7800, 10400, 15600, 20800, 31250, 41700, 62500, 125000, 250000, 500000};

//transmit power definitions (5 entries)
int TXP[] = {7, 13, 17, 20, 23};

// SCK pin = 18
// MISO pin = 19
// MOSI pin = 23

#define RFM95_CS 2
#define RFM95_RST 0
#define RFM95_INT 4

// Change to 434.0 or other frequency, must match rx's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Blinky on receipt
#define LED 5

void setup()
{

  delay(2000);
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  Serial.begin(115200);
  delay(100);

  Serial.println("Arduino LoRa rx Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(TXP[4], false);
  rf95.setSignalBandwidth(BW[9]);
  rf95.setSpreadingFactor(SF[1]); // do we need a configuration delay? delay using a counter and millis() or delay()?


}

void loop()
{
  if (rf95.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len))
    {

      digitalWrite(LED, HIGH);
      RH_RF95::printBuffer("Received: ", buf, len);
      Serial.print("Got: ");
      Serial.println((char*)buf);
      // begin deconstruction of message
      // char radiopacket[22] = "0001,N,07,500,06,0000"; is the first packet to be transmitted
      //      int Counter = (buf[0]-48)*10000+(buf[1]-48)*1000+(buf[2]-48)*100+(buf[3]-48)*10+(buf[4]-48);
      char Counter[5] = {buf[0], buf[1], buf[2], buf[3], '\0'};
      char Sender[2] = {buf[5], '\0'};
      char TXdB[3] = {buf[7], buf[8], '\0'};
      char Bandwidth[4] = {buf[10], buf[11], buf[12], '\0'};
      char SFactor[3] = {buf[14], buf[15], '\0'};

      // begin switch statements for new transmission configurations
      if ( Sender[0] = 'N' ) {
        switch ( atoi(TXdB) ) { // transmit power switch {7, 13, 17, 20}
          case '07':
            rf95.setTxPower(TXP[0], false);
            break;
          case '13':
            rf95.setTxPower(TXP[1], false);
            break;
          case '17':
            rf95.setTxPower(TXP[2], false);
            break;
          case '20':
            rf95.setTxPower(TXP[3], false);
            break;
          case '23':
            rf95.setTxPower(TXP[4], false);
            break;
        }
        switch (atoi(Bandwidth)) { // bandwidth switch {7.8, 10.4, 15.6, 20.8, 31.25, 41.7, 62.5, 125, 250, 500};
          case '500':
              rf95.setSignalBandwidth(BW[9]);
            break;
          case '250':
              rf95.setSignalBandwidth(BW[8]);
            break;
          case '125':
              rf95.setSignalBandwidth(BW[7]);
            break;
          case '062':
              rf95.setSignalBandwidth(BW[6]);
            break;
          case '041':
              rf95.setSignalBandwidth(BW[5]);
            break;
          case '031':
              rf95.setSignalBandwidth(BW[4]);
            break;
          case '020':
              rf95.setSignalBandwidth(BW[3]);
            break;
          case '015':
              rf95.setSignalBandwidth(BW[2]);
            break;
          case '010':
              rf95.setSignalBandwidth(BW[1]);
            break;
          case '007':
              rf95.setSignalBandwidth(BW[0]);
            break;
        }
        switch (atoi(SFactor)) { // Spreading factor switch
          case '06':
              rf95.setSpreadingFactor(SF[0]);
            break;
          case '07':
              rf95.setSpreadingFactor(SF[1]);
            break;
          case '08':
              rf95.setSpreadingFactor(SF[2]);
            break;
          case '09':
              rf95.setSpreadingFactor(SF[3]);
            break;
          case '10':
              rf95.setSpreadingFactor(SF[4]);
            break;
          case '11':
              rf95.setSpreadingFactor(SF[5]);
            break;
          case '12':
              rf95.setSpreadingFactor(SF[6]);
            break;
        }
      }


      //      Serial.print("RSSI: ");
      int lastRx = rf95.lastRssi();
      rx = String(lastRx);
      if  ( lastRx <= -100 ) rx = String(" " + rx);
      serialOut = String(String(Counter) + ",R," + String(TXdB) + "," + String(Bandwidth) + "," + String(SFactor) + "," + rx);
      Serial.println(serialOut);  
      char radioPacket[serialOut.length()];  
      strcpy(radioPacket, serialOut.c_str());  
      // Send a reply
      rf95.send((uint8_t *)radioPacket, 22 );
      rf95.waitPacketSent();
      //Serial.println("Sent a reply");
      digitalWrite(LED, LOW);
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
}
