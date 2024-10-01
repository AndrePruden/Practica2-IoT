#ifndef LED_H
#define LED_H

#include <Arduino.h>

class Led {
public:
    Led(int pin) : pin(pin) {
        pinMode(pin, OUTPUT);
        turnOff();
    }

    void turnOn() {
        digitalWrite(pin, HIGH);
    }

    void turnOff() {
        digitalWrite(pin, LOW);
    }

private:
    int pin;
};

#endif 
