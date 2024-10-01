#ifndef RESTART_HANDLER_H
#define RESTART_HANDLER_H

class RestartHandler {
public:
    void checkForRestartMessage(WiFiClient& client) {
        if (client.available()) {
            String message = client.readStringUntil('\n');
            if (message == "RESTART") {
                Serial.println("Restart...");
                ESP.restart(); // Reinicia el microcontrolador
            }
        }
    }
};

#endif