#include <wifi_transmit.h>

const char* ssid     = "TestSenior";
const char* password = "JustAnotherPass";
int nodeNumber = 0;
double temp = 0;
const char* host = "192.168.0.1";
//const char* streamId   = "....................";
//const char* privateKey = "....................";

bool wifi_transmit::init() {
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to...");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}


bool wifi_transmit::send_message(String message) {
    Serial.println("WiFi connected :: Start sending data to the server");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    delay(1000);
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 6969;
    if (!client.connect(host, httpPort)) 
    {
        Serial.println("connection failed");
        return false;
    }

    //This will send the request to the server
    client.print(message);
    //client.print(nodePrint+"\n"+SensorPrint+"\n"+PrintValue);
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return false;
        }
    }

    Serial.println();
    Serial.println("closing connection");
    return true;
}
