#include "Arduino.h"
#include "Servo.h"

#include "Encoder.h"
#include "C:\\Users\\dylan\\Desktop\\Github\\Wire-Cam\\cpp\\arduino\\Constants.h"
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

int leftSpeedToSet;
int rightSpeedToSet;

Servo leftMotor;
Servo rightMotor;

bool shouldWrite = false;

void UpdateEncoders() {

    leftEncoder->Update();
    rightEncoder->Update();    
}

void setup() {

    Serial.begin(57600);

    pinMode(2, INPUT);
    pinMode(4, INPUT);
    pinMode(RIGHT_ENCODER_CLK_PIN, INPUT);
    pinMode(RIGHT_ENCODER_DT_PIN, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    attachInterrupt(digitalPinToInterrupt(2), UpdateEncoders, CHANGE);
    attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_CLK_PIN), UpdateEncoders, CHANGE);

    leftMotor.attach(7);
    //rightMotor.attach(RIGHT_MOTOR_PIN);

    leftEncoder = new Encoder(LEFT_ENCODER_CLK_PIN, LEFT_ENCODER_DT_PIN);
    rightEncoder = new Encoder(RIGHT_ENCODER_CLK_PIN, RIGHT_ENCODER_DT_PIN);
}

void loop() {

    if (Serial.available() >= 2) {
        
        leftSpeedToSet = (int)Serial.read();
        rightSpeedToSet = (int)Serial.read();
        clearSerial();
        shouldWrite = true;
        //if (leftSpeedToSet == 127 && rightSpeedToSet == 127) digitalWrite(LED_BUILTIN, HIGH);
        //selse digitalWrite(LED_BUILTIN, LOW);
    }
    else { shouldWrite = false; }
    leftMotor.writeMicroseconds(map(leftSpeedToSet, 0, 255, 1000, 2000));
    //rightMotor.writeMicroseconds(map(rightSpeedToSet, 0, 255, 1000, 2000));

    if (shouldWrite) {
        
        leftMotorPositionToSend = leftEncoder->GetPosition();
        rightMotorPositionToSend = 0;//rightEncoder->GetPosition();
        memcpy(txBuffer, &leftMotorPositionToSend, sizeof(long));
        memcpy(txBuffer + 4, &rightMotorPositionToSend, sizeof(long));
        Serial.write(txBuffer, 8);
    }
}