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

    void sendDistance(WiFiClient& client, float distance) {
        if (client.connected()) {
            Serial.print("Sending...\n");
            client.print("DISTANCE:");
            client.println(distance);
        } else {
            Serial.println("Disconnected from the server. Attempting to reconnect...");
            connect(client);
        }
    }

private:
    const char* serverIP;
    int serverPort;
};

#endif
