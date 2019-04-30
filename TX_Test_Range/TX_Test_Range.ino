// LoRa 9x_TX_Rangetest
// -*- mode: C++ -*-
// This sketch pings another LoRa module through the all of
// spreading factor, bandwidth and transmission power options,
// and reports signal strength, transmit and receive strengths.
// This sketch is transmitter part of a pair of transmit test 
// (the other is the sender 'RX_Test_Range') for checking a range
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
volatile byte state = LOW;

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

//spreading factor definition (7 entries)
uint8_t SF[] = {6, 7, 8, 9, 10, 11, 12};

//bandwidth definitions (10 entries)
long BW[] = {7800, 10400, 15600, 20800, 31250, 41700, 62500, 125000, 250000, 500000};

//transmit power definitions (5 entries)
int TXP[] = {23, 20, 17, 13, 7};


void setup()
{

String serialOut; // string to be displayed on serial output
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  Serial.begin(115200);
  delay(100);

  Serial.println("Arduino LoRa TX Test!");

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
  Serial.println("Initializing TX power to 23 dBm, bandwidth to 500kHz, and SF to 6");
  // rf95.setTxPower(23, false);
  rf95.setTxPower(TXP[0], false);
  rf95.setSignalBandwidth(BW[9]);
  rf95.setSpreadingFactor(SF[1]); // do we need a configuration delay? delay using a counter and millis() or delay()?


// begin transmit test code
      // begin deconstruction of message
      // char radiopacket[22] = "0001,N,07,500,06,0000"; is the first packet to be transmitted
      //      int Counter = (buf[0]-48)*10000+(buf[1]-48)*1000+(buf[2]-48)*100+(buf[3]-48)*10+(buf[4]-48);
      
int32_t packetnum = 1;  // packet counter, we increment per xmission

int TX_Fail = 0; // fail counters, breaks loops in case of transmissions incrementally failing more difficult tests
int BW_Fail = 0; 
int SF_Fail = 0; 

int TX_Loop_Count = 5; // sizeof(TXP)/sizeof(TXP[0]); 
int BW_Loop_Count = 10; // sizeof(BW)/sizeof(BW[0]);   
int SF_Loop_Count = 7; // sizeof(SF)/sizeof(SF[0]);   

unsigned long ts = 0;  // time sent, for ping time
unsigned long rs = 0;  // time received, for ping time

char radiopacket[23] = "0001,S,23,500,07,0000"; // first packet to be transmitted
radiopacket[22]=0;

Serial.println("Beginning Transmit Test");

// test loop
/*for (int i = 0; i <= 255; i++) { ts = millis(); // record time sent
      serialOut = String(String(radiopacket) + "," + String(ts));
      Serial.println(serialOut);
      radiopacket[21] = 0;
      rf95.send((uint8_t *)radiopacket, 22 );
      rf95.waitPacketSent();
    
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
    
      if (rf95.waitAvailableTimeout(1000))
        {
        if (rf95.recv(buf, &len))
          {
          rs = millis();

          Serial.println(String(String((char*)buf)+","+rs));
          }
        }
  }*/
// end test loop
  
// start the transmit loop
for ( int TX_Count = 0 ; TX_Count < TX_Loop_Count ; TX_Count++) { 
  if ( TX_Fail >= 5 ) break; // if there are too many fail counters, break the loop
  //Serial.println("TX loop" + String(TX_Count));
  //start the bandwidth loop
  if (BW_Fail >= 5){
  BW_Fail = 0;
  TX_Fail++;
  }
  for ( int BW_Count = 0 ; BW_Count < BW_Loop_Count ; BW_Count++ ) {
    if ( BW_Fail >= 5 ) break; // if there are too many fail counters, break the loop
    // if spreading factor failure
    if (SF_Fail >= 5){
      SF_Fail = 0;
      BW_Fail++;
      }
    //Serial.println("BW loop" + String(BW_Count));
    //start the spreading factor loop
    for ( int SF_Count = 1 ; SF_Count < SF_Loop_Count ; SF_Count++ ) {
      if ( SF_Fail >= 5 ) break; // if there are too many fail counters, break the loop
      
      
      
      
      /* packet is a vector of characters, a total of 22 bytes, and follows this structure:
       *  Each char is an ASCII character 
      PacketNumber is packet number (4 bytes)
      , is a comma for organization purposes (1 byte per comma)
      SENDER is the sender of the packet (1 byte): S for sender (the node with this TX_Rangetest)
                                                  R for receiver (the node with this RX_Rangetest)
                                                  N for new command
      XMITdBm is the transmit power of the signal (2 bytes); changes the receiver transmit profiles following a 'N'
      BW is the bandwidth of the signal (3 bytes); changes the receiver transmit profiles following a 'N'
      SF is the spreading factor of a signal (2 byes); changes the receiver transmit profiles following a 'N'
      SIG.STRdBm is the received power of a signal (4 bytes). Only a reply contains this value. The console lists both send and reply strenghts on the reply.
      Stop is the ASCII stop character (1 byte)
      PacketNumber,SENDER,XMITdBm,BW,SF,SIG.STRdBm,Stop
      4,1,2,3,2,4 1 => 22 bytes */
      //                          P#   S TX BW  SF RXdB         
      // char radiopacket[22] = "0001,N,07,500,06,0000"; is the first packet to be transmitted
      
      //Send transmission
      ts = millis(); // record time sent
      serialOut = String(String(radiopacket)); // + "," + String(ts));
      Serial.println(serialOut);
      radiopacket[21] = 0;
      rf95.send((uint8_t *)radiopacket, 22 );
      rf95.waitPacketSent();
      
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      
      if (rf95.waitAvailableTimeout(100))
        {
        if (rf95.recv(buf, &len))
          {
          rs = millis();
          
          Serial.print((char*)buf); Serial.println(String(","+String(ts)+","+String(rs)));

           // successful transmission proves parameters for TX  reset TX fail counters
           BW_Fail = 0;
           

          // if message sent was a reconfigure, change transmission settings after getting acknowledgement from receiver
          if (radiopacket[5]=='N'){
            rf95.setTxPower(TXP[TX_Count], false);
            rf95.setSignalBandwidth(BW[BW_Count]);
            rf95.setSpreadingFactor(SF[SF_Count]); // do we need a configuration delay? delay using a counter and millis() or delay()?
            }
          }
        }
      // if no reply received
      else {
        // increase number of failures on this spreading factor
        SF_Fail++;
      }
      /*RX = rf95.lastRssi();
    
      if (radioPacket[6]="R")
      
      serialOut = String("," + P + "," + S + "," + TX + "," + BW + "," + SF + "," + RX);
      radiopacket = 
      serialOut = String("," + radiopacket + "," + ts);
      Serial.println(serialOut);*/
        
      /* Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
          
      }
      else
      {
        // Serial.println("Receive failed");
      }
      }
      else
      {
        // Serial.println("No reply, is there a listener around?");
      }
      delay(400);
      */
      
      }

    }
  }
  Serial.println("Test complete");
}

void loop(){

  
}
