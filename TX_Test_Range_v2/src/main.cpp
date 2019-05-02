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
#include <esp32-hal-cpu.h>
#include <esp32-hal-i2c.h>
#include <RHGenericDriver.h>
#include <RHGenericSPI.h>

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
char SF_Char[7][3] = {"06","07","08","09","10","11","12"};
//bandwidth definitions (10 entries)
long BW[] = {7800, 10400, 15600, 20800, 31250, 41700, 62500, 125000, 250000, 500000};
char BW_Char[10][4] = {"007","010","015","020","031","041","062","125","250","500"};
//transmit power definitions (5 entries)
int TXP[5] = {7, 13, 17, 20, 23};
char TXP_Char[5][3] = {"23","20","17","13","07"};

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
  

void setup() {

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
  rf95.setSignalBandwidth(BW[7]);
  rf95.setSpreadingFactor(SF[1]); // do we need a configuration delay? delay using a counter and millis() or delay()?
  radiopacket[22]=0;

  // begin transmit test code
  // begin deconstruction of message
  // char radiopacket[22] = "0001,N,07,500,06,0000"; is the first packet to be transmitted
  //      int Counter = (buf[0]-48)*10000+(buf[1]-48)*1000+(buf[2]-48)*100+(buf[3]-48)*10+(buf[4]-48);
      
  Serial.println("Beginning Transmit Test");
  
}

int TX_Count = 0;
int a = 0;
void loop(){

  String serialOut; // string to be displayed on serial output
  // start the transmit loop

  if( a < sizeof(TXP)) {
    for ( int i = 0 ; i < sizeof(BW)/sizeof(BW[0]) ; i++ ) {
      for(int j = 0; j < sizeof(SF)/sizeof(SF[0]); j++) {

        
          
        for ( int k = 0 ; k < SF_Loop_Count + 1 ; ) {
          
          // if this is the first transmission, send a reconfiguration packet
         if (k == 0) {
          radiopacket[5] = 'N';
            
          if((millis()-rs)>= 1000 ){ // if no reply could be sent in 1000 millisecondss
            //Serial.println("No reply, switching to long range mode");
            rf95.setTxPower(TXP[0], false); // change to long range mode
            rf95.setSignalBandwidth(BW[7]);
            rf95.setSpreadingFactor(SF[1]);
          }
          //Serial.println("radio packet should be N next");
          }
          // if not, send a regular packet
          else{
            radiopacket[5] = 'S';
          //Serial.println("radio packet should be S next");
          } 
          
          char pnum[4];
          sprintf(pnum, "%04lx", packetnum);
          

          radiopacket[0] = pnum[0];
          radiopacket[1] = pnum[1];
          radiopacket[2] = pnum[2];
          radiopacket[3] = pnum[3];
          //radiopacket[0] = packetnum;
          //radiopacket[1] = packetnum >> 8;
          //radiopacket[2] = packetnum >> 16;
          //radiopacket[3] = packetnum >> 24;
          radiopacket[7] = TXP_Char[k][0];
          radiopacket[8] = TXP_Char[k][1];
          radiopacket[10] = BW_Char[j][0];
          radiopacket[11] = BW_Char[j][1];
          radiopacket[12] = BW_Char[j][2];
          radiopacket[14] = SF_Char[k][0];
          radiopacket[15] = SF_Char[k][1];
          
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
          Serial.println(radiopacket);
           radiopacket[21] = 0;
         // serialOut = String(radiopacket); // + "," + String(ts));
          //Serial.println(serialOut);
         
          rf95.send((uint8_t *)radiopacket, 22 );
          rf95.waitPacketSent();
      
          uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
          uint8_t len = sizeof(buf);

          if( radiopacket[5] == 'S' ) k++;
      
          if (rf95.waitAvailableTimeout(100)) {
            if (rf95.recv(buf, &len)) {
              rs = millis();
              if( radiopacket[5] == 'N' ) k++;
              Serial.print((char*)buf); Serial.println(String(","+String(ts)+","+String(rs)));

            } 
          }
        }
        rf95.setSpreadingFactor(SF[j]);
      }
      rf95.setSignalBandwidth(BW[i]);
    }
    rf95.setTxPower(TXP[++a], false);
  }
}
