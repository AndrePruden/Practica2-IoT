// Main.ino
#include <Arduino.h>
#include "WiFiConnection.h"
#include "ServerClient.h"
#include "Led.h"
#include "RestartHandler.h"

const char* ssid = "AndrePruden";
const char* password = "mantequilla2019";
const char* serverIP = "192.168.43.83";
const int serverPort = 10000;

WiFiClient client;
WiFiConnection wifiConnection(ssid, password);
ServerClient serverClient(serverIP, serverPort);
RestartHandler restartHandler;

Led greenLed(12);
Led yellowLed(27);
Led redLed(25);

unsigned long previousConnectionAttempt = 0;
const long connectionAttemptInterval = 1000;

void setup() {
    Serial.begin(115200);
    wifiConnection.connect();
    serverClient.connect(client);
}

void loop() {
    if (serverClient.isConnected(client)) {
        if (client.available()) {
            String command = serverClient.readCommand(client);
            processCommand(command);
        }
    } else {
        if (millis() - previousConnectionAttempt >= connectionAttemptInterval) {
            Serial.println("Disconnected from server. Attempting to reconnect...");
            previousConnectionAttempt = millis();
            serverClient.connect(client);
        }
    }

    restartHandler.checkForRestartMessage(client);
}

void processCommand(String command) {
    Serial.println(command);
    if (command == "STATE 3") {
        setGreenLed();
    } else if (command == "STATE 2") {
        setYellowLed();
    } else if (command == "STATE 1") {
        setRedLed();
    } else if (command == "STATE 0") {
        turnOffAllLeds();
    }
}

void setGreenLed() {
    greenLed.turnOn();
    yellowLed.turnOff();
    redLed.turnOff();
    Serial.println("Green LED ON");
}

void setYellowLed() {
    greenLed.turnOff();
    yellowLed.turnOn();
    redLed.turnOff();
    Serial.println("Yellow LED ON");
}

void setRedLed() {
    greenLed.turnOff();
    yellowLed.turnOff();
    redLed.turnOn();
    Serial.println("Red LED ON");
}

void turnOffAllLeds() {
    greenLed.turnOff();
    yellowLed.turnOff();
    redLed.turnOff();
    Serial.println("All LEDs OFF");
}
