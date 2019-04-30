#include <Arduino.h>
#include <WiFi.h>

const char* ssid     = "University of Washington";
const char* password = "";
int nodeNumber = 0;
double temp = 0;
const char* host = "10.16.30.219";
//const char* streamId   = "....................";
//const char* privateKey = "....................";

void setup()
{
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to...");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
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
        return;
    }

    //This will send the request to the server
    client.print(String("GET: Hello World!!!, ")+"Host: " + host + ", " + "Connection: "+ httpPort);
    //client.print(nodePrint+"\n"+SensorPrint+"\n"+PrintValue);
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    Serial.println();
    Serial.println("closing connection");
}
