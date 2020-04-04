#include "Arduino.h"

//#include "Constants.h"
#include "Winch.h"
#include "WireCam.h"
#include "SerialUtils.h"
#include "EncoderUtils.h"

/*                              CONSTANTS                                    */
#define LEFT_ENCODER_CLK_PIN 2
#define LEFT_ENCODER_DT_PIN 4

#define RIGHT_ENCODER_CLK_PIN 3
#define RIGHT_ENCODER_DT_PIN 5

#define ENCODER_CYCLE 20

#define LEFT_MOTOR_PIN 10
#define RIGHT_MOTOR_PIN 11

#define X_COORD 10
#define Y_COORD 11
/*                               END CONSTANTS                               */

int joystick_x = 0;
int joystick_y = 0;
int joystick_throttle = 0;
int joystick_z = 0;

//Servo *testServo = NULL;

Winch *leftWinch = NULL;
Winch *rightWinch = NULL;
WireCam *wireCam = NULL;

double PythagC(const double a, const double b) {

    return sqrt(sq(a) + sq(b));
}

void UpdateEncoders() {

    wireCam->UpdateEncoders();
}

void setup() {

    Serial.begin(9600);

    pinMode(LEFT_ENCODER_CLK_PIN, INPUT);
    pinMode(LEFT_ENCODER_DT_PIN, INPUT);
    pinMode(RIGHT_ENCODER_CLK_PIN, INPUT);
    pinMode(RIGHT_ENCODER_DT_PIN, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_CLK_PIN), UpdateEncoders, CHANGE);
    attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_CLK_PIN), UpdateEncoders, CHANGE);

    leftWinch = new Winch(LEFT_MOTOR_PIN, LEFT_ENCODER_CLK_PIN, LEFT_ENCODER_DT_PIN);
    rightWinch = new Winch(RIGHT_MOTOR_PIN, RIGHT_ENCODER_CLK_PIN, RIGHT_ENCODER_DT_PIN, REVERSE);
    wireCam = new WireCam(leftWinch, rightWinch);
    wireCam->Zero();
    wireCam->Stop();

    /*testServo->attach(6);
    testServo->write(180);*/
}

void loop() {

    //Tell python to send over joystick data
    /*Serial.write(0);
    while (Serial.available() < 4);
    
    joystick_x = -(int)Serial.read();
    joystick_y = (int)Serial.read();
    joystick_throttle = (int)Serial.read();
    joystick_z = -(int)Serial.read();
    clearSerial();

    servo.write(joystick_z * 180.0 / 256.0);*/

    //leftWinch->increasePosition();
    //Serial.println(leftWinch->getPosition());
    //if (Serial.available() > 0) {
        
        rightWinch->GotoPosition(60);
        //Serial.println(leftWinch->getPosition());
    //}
}