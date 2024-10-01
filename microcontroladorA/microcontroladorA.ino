#include <Arduino.h>
#include "WiFiConnection.h"
#include "ServerClient.h"
#include "UltrasonicSensor.h"
#include "RestartHandler.h"

const char* ssid = "AndrePruden";
const char* password = "mantequilla2019";
const char* serverIP = "192.168.43.83"; 
const int serverPort = 10000;

WiFiClient client;
WiFiConnection wifiConnection(ssid, password);
ServerClient serverClient(serverIP, serverPort);
UltrasonicSensor ultrasonicSensor(18, 19); 
RestartHandler restartHandler;

unsigned long previousMillis = 0; 
const long interval = 1000; 

void setup() {
    Serial.begin(115200);
    wifiConnection.connect();
    serverClient.connect(client);
}

void loop() {
    unsigned long currentMillis = millis(); 

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis; 

        float distance = ultrasonicSensor.getDistance();
        serverClient.sendDistance(client, distance);
    }

    restartHandler.checkForRestartMessage(client);
}
