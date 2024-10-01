#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include <WiFi.h>

class WiFiConnection {
public:
    WiFiConnection(const char* ssid, const char* password) 
      : ssid(ssid), password(password) {}

    void connect() {
        Serial.print("Connecting to WiFi...");
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.print(".");
        }
        Serial.println(" Connected.");
    }

private:
    const char* ssid;
    const char* password;
};

#endif
