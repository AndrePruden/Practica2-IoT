#ifndef SERVERCLIENT_H
#define SERVERCLIENT_H

#include <WiFiClient.h>

class ServerClient {
public:
    ServerClient(const char* serverIP, int serverPort) 
      : serverIP(serverIP), serverPort(serverPort) {}

    void connect(WiFiClient& client) {
        Serial.print("Connecting to server...");
        while (!client.connect(serverIP, serverPort)) {
            Serial.print(".");
            delay(1000);
        }
        Serial.println(" Connected.");
    }

    bool isConnected(WiFiClient& client) {
        return client.connected();
    }

    String readCommand(WiFiClient& client) {
        return client.readStringUntil('\n');
    }

private:
    const char* serverIP;
    int serverPort;
};

#endif