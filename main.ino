#include "Arduino.h"

int zeroPosition = 0;
int position = 0;

int readBuffer = LOW;
int oldReadBuffer = LOW;

void GotoPosition(int positionToGoto) {

    int deltaPosition = position - positionToGoto;
    //Get speedToSet from a regression
    int speedToSet = round(0.009016835 * pow(deltaPosition, 3) + 0.0015151515 * sq(deltaPosition) + 3.503367003 * deltaPosition + 127);
    //If the speedToSet is outside of a byte, make it be in-bounds
    speedToSet = (speedToSet > 255 || speedToSet < 0) ? ((speedToSet > 255) ? 255 : 0) : speedToSet;
    //If the speedToSet is within 5 of off, set the speed to 0
    speedToSet = (speedToSet < 132 && speedToSet > 122) ? 0 : speedToSet;
    //Write the finalized motor speed
    analogWrite(MOTOR_PIN, speedToSet);
}

double PythagC(double a, double b) {

    return sqrt((a * a) + (b * b));
}

void UpdateEncoder() {

    readBuffer = digitalRead(ENCODER_CLK_PIN);
    //If we go from low to high on CLK
    if (readBuffer != oldReadBuffer && readBuffer == HIGH) {

        if (digitalRead(ENCODER_DT_PIN) == HIGH) {

            pos++;
        }
        else {

            pos--;
        }
    }
    oldReadBuffer = readBuffer;
}

void setup() {

    pinMode(ENCODER_CLK_PIN, INPUT);
    pinMode(ENCODER_DT_PIN, INPUT);
    attachInterrupt(0, UpdateEncoder, CHANGE);
    attachInterrupt(1, UpdateEncoder, CHANGE);
    Serial.begin(9600);
}

void loop() {
}