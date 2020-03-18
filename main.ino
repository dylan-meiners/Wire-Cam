#include "Arduino.h"

int pos = 0;

int readBuffer = LOW;
int oldReadBuffer = LOW;

double PythagC(double a, double b) {

    return sqrt((a * a) + (b * b));
}

void UpdateEncoder() {

    readBuffer = digitalRead(encoderCLK);
    //If we go from low to high on CLK
    if (readBuffer != oldReadBuffer && readBuffer == HIGH) {

        if (digitalRead(encoderDT) == HIGH) {

            pos++;
        }
        else {

            pos--;
        }
    }
    oldReadBuffer = readBuffer;
}

void setup() {

    pinMode(encoderCLK, INPUT);
    pinMode(encoderDT, INPUT);
    attachInterrupt(0, UpdateEncoder, CHANGE);
    attachInterrupt(1, UpdateEncoder, CHANGE);
    Serial.begin(9600);
}

void loop() {
}