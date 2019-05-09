#include "wifi_transmit.h"

const char* ssid     = "testSenior";
const char* password = "JustAnotherPass";
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
   
	if (TCPbuffer.length() <1000)
	{
		TCPbuffer = TCPbuffer + message+ "\r\n";
		return true;
	}
	else {
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
