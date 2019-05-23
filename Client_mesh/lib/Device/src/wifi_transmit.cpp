#include "wifi_transmit.h"
#include "Def.h"

#ifdef WIFI_TRANSMIT_TRUE
const char* ssid     = "testSenior";
const char* password = "JustAnotherTest";
int nodeNumber = 0;
double temp = 0;
String TCPbuffer = "";

//const char* host = "192.168.0.1";
//const char* streamId   = "....................";
//const char* privateKey = "....................";

bool wifi_transmit::init() {
    //Serial.begin(115200);
    //delay(10);

    // We start by connecting to a WiFi network

    //Serial.println();
    //Serial.println();
    //Serial.print("Connecting to...");
    //Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

  
}


bool wifi_transmit::send_message(String message) {
   wifi_transmit::flush(message);
	/*
	{
		TCPbuffer = TCPbuffer + message+ "\r\n";
	}
	if (TCPbuffer.length() > 1000) {
		// Use WiFiClient class to create TCP connections
		WiFiClient client;
		const int httpPort = 6969;
		if (!client.connect(WiFi.gatewayIP(), httpPort))
		{
			return false;
		}

		//This will send the request to the server
		client.print(TCPbuffer);
		TCPbuffer = "";
		//client.print(nodePrint+"\n"+SensorPrint+"\n"+PrintValue);
		unsigned long timeout = millis();
		while (client.available() == 0) {
			if (millis() - timeout > 5000) {

				client.stop();
				return false;
			}
		}
		

		return true;
	}*/
}
bool wifi_transmit::flush(String message) {


	{
		TCPbuffer = TCPbuffer + message + "\r\n";
		
	}
 {
		// Use WiFiClient class to create TCP connections
		WiFiClient client;
		const int httpPort = 6969;
		if (!client.connect(WiFi.gatewayIP(), httpPort))
		{
			return false;
		}

		//This will send the request to the server
		client.print(TCPbuffer);
		TCPbuffer = "";
		//client.print(nodePrint+"\n"+SensorPrint+"\n"+PrintValue);
		unsigned long timeout = millis();
		while (client.available() == 0) {
			if (millis() - timeout > 5000) {

				client.stop();
				return false;
			}
		}


		return true;
	}
}
#else 

bool wifi_transmit::init() {
    Serial.begin(115200);
    
    }

  



bool wifi_transmit::send_message(String message) {
   
  Serial.println(message);
  
}
bool wifi_transmit::flush(String message) {
  Serial.println(message);
}
#endif
