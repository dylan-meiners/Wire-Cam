//#define VIRTUAL_WINCHES

#include "Arduino.h"
#include "Servo.h"

#include "Encoder.h"
#include "T:\\Documents\\Coding\\Coding\\Wire-Cam\\arduino\\Constants.h"
#include "SerialUtils.h"

Encoder * leftEncoder = (Encoder*)malloc(sizeof(*leftEncoder));
Encoder * rightEncoder = (Encoder*)malloc(sizeof(*rightEncoder));

unsigned char rxBuffer[2] = {0};//(unsigned char*)malloc(2);
/*
RX BUFFER IS AS FOLLOWS:
{[00000000], [00000000]}
leftWinchSpeedToSet     unsigned char   (1 byte)
rightWinchSpeedToSet    unsigned char   (1 byte)
=================================================
TOTALS                                  2 bytes
*/

unsigned char txBuffer[8] = {0};
long leftMotorPositionToSend;
long rightMotorPositionToSend;
/*
ARDUINO TX BUFFER IS AS FOLLOWS:
{[00000000|00000000|00000000|00000000], [00000000|00000000|00000000|00000000]}
leftEncoderPos      long    (4 bytes)
rightEncoderPos     long    (4 bytes)
=============================================
TOTALS                              8 bytes
*/

int leftSpeedToSet = 127;
int rightSpeedToSet = 127;

Servo leftMotor;
Servo rightMotor;

bool shouldWrite = false;

#ifndef VIRTUAL_WINCHES
void UpdateEncoders() {

    leftEncoder->Update();
    rightEncoder->Update();    
}
#endif

void setup() {

    Serial.begin(57600);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    #ifndef VIRTUAL_WINCHES
    pinMode(LEFT_ENCODER_CLK_PIN, INPUT);
    pinMode(LEFT_ENCODER_DT_PIN, INPUT);
    pinMode(RIGHT_ENCODER_CLK_PIN, INPUT);
    pinMode(RIGHT_ENCODER_DT_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_CLK_PIN), UpdateEncoders, CHANGE);
    attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_CLK_PIN), UpdateEncoders, CHANGE);
    leftMotor.attach(LEFT_MOTOR_PIN);
    rightMotor.attach(RIGHT_MOTOR_PIN);
    #endif

    leftEncoder = new Encoder(LEFT_ENCODER_CLK_PIN, LEFT_ENCODER_DT_PIN);
    rightEncoder = new Encoder(RIGHT_ENCODER_CLK_PIN, RIGHT_ENCODER_DT_PIN);
    
    clearSerial();
}

void loop() {

    if (Serial.available() >= 2) {
        
        leftSpeedToSet = (int)Serial.read();
        rightSpeedToSet = (int)Serial.read();
        clearSerial();
        shouldWrite = true;
    }
    else shouldWrite = false;

    #ifndef VIRTUAL_WINCHES
    leftMotor.writeMicroseconds(MapUCharToMicroseconds(leftSpeedToSet));
    rightMotor.writeMicroseconds(MapUCharToMicroseconds(rightSpeedToSet));
    #else
    leftEncoder->m_virtual_speedToSet = RoundLit(MapUCharToMicroseconds(leftSpeedToSet) - 1500);
    rightEncoder->m_virtual_speedToSet = RoundLit(MapUCharToMicroseconds(rightSpeedToSet) - 1500);
    leftEncoder->VirtualUpdate();
    rightEncoder->VirtualUpdate();
    #endif

    if (shouldWrite) {
        
        leftMotorPositionToSend = leftEncoder->GetPosition();
        rightMotorPositionToSend = rightEncoder->GetPosition();
        memcpy(txBuffer, &leftMotorPositionToSend, sizeof(long));
        memcpy(txBuffer + 4, &rightMotorPositionToSend, sizeof(long));
        Serial.write(txBuffer, 8);
    }
}