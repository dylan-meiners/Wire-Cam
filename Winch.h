#pragma once
//#define NO_MOTOR_TESTING

#define REVERSE -1

#include "Arduino.h"

#include <Servo.h>

class Winch {

    public:

        Winch(const int motorPinToSet,
              const int encoderCLKPinToSet,
              const int encoderDTPinToSet,
              const int reverseDirectionMultiplierToSet = 1) {

            m_motor.attach(motorPinToSet);
            m_encoderCLKPin = encoderCLKPinToSet;
            m_encoderDTPin = encoderDTPinToSet;
            m_oldRead = LOW;
            m_reverseDirectionMultiplier = reverseDirectionMultiplierToSet;
            
            #ifdef NO_MOTOR_TESTING

                m_motorSpeedToSet = 0;
            #endif
        }

        void GotoPosition(const int positionToGoto, const bool relative = false) {

            int deltaPosition;
            if (relative) {
            
                deltaPosition = m_position + positionToGoto;
            }
            else {
                
                deltaPosition = m_position - positionToGoto;
            }

            deltaPosition = (deltaPosition < -9.5 || deltaPosition > 9.5) ? ((deltaPosition < -9.5) ? -9.5 : 9.5) : deltaPosition;
            //Get speedToSet from a regression
            int speedToSet = round((-0.0155218855 * pow(deltaPosition, 3)) + (4.329004329 * pow(10, -4) * sq(deltaPosition)) + (12.47895623 * deltaPosition) + 127.3722944);
            //If the speedToSet is outside of a byte, make it be in-bounds
            speedToSet = (speedToSet > 255 || speedToSet < 0) ? ((speedToSet > 255) ? 255 : 0) : speedToSet;
            //If the speedToSet is trying to move the motor, but can't
            //speedToSet = (speedToSet < 140 && speedToSet > 128) ? 140 : speedToSet;
            //speedToSet = (speedToSet > 114 && speedToSet < 126) ? 114 : speedToSet;
            //If the speedToSet is within 1 of off, set the speed to 0
            speedToSet = (speedToSet > 120 && speedToSet < 134) ? 127 : speedToSet;

            Serial.print("Delta: "); Serial.println(deltaPosition);
            Serial.print("Pos: "); Serial.println(m_position);

            speedToSet *= m_reverseDirectionMultiplier;

            #ifdef NO_MOTOR_TESTING
                
                m_motorSpeedToSet = map(speedToSet, 0, 255, 2000, 1000);
            #else

                //Write the finalized motor speed in a range of 2000 to 1000 based on 0 to 255
                //Make sure to reverse motor if needed
                m_motor.writeMicroseconds(map(speedToSet, 0, 255, 2000, 1000));
            #endif
        }

        void UpdateEncoder() {

            int readBuffer = digitalRead(m_encoderCLKPin);
            //If we go from low to high on CLK
            if (readBuffer != m_oldRead && readBuffer == HIGH) {

                if (digitalRead(m_encoderDTPin) == HIGH) {

                    increasePosition();
                }
                else {

                    increasePosition(-1);
                }
            }
            m_oldRead = readBuffer;
        }

        void increasePosition(const int amount = 1) {

            m_position = m_position + amount;
        }

        void Stop() {

            m_motor.writeMicroseconds(1500);
        }

        void Zero() {

            m_position = 0;
        }

        int getPosition() {

            return m_position;
        }

        #ifdef NO_MOTOR_TESTING

            int getMotorSpeedToSet() {

                return m_motorSpeedToSet;
            }
        #endif

    private:

        Servo m_motor;
        int m_encoderCLKPin;
        int m_encoderDTPin;
        int m_position;
        int m_oldRead;
        int m_reverseDirectionMultiplier;
        
        #ifdef NO_MOTOR_TESTING

            double m_motorSpeedToSet;
        #endif
};