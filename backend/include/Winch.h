#pragma once

#define REVERSE -1

#include "Utils.h"
#include <math.h>
#include <iostream>

class Winch {

    public:

        Winch(const int reverseDirectionMultiplierToSet = 1) {

            m_reverseDirectionMultiplier = reverseDirectionMultiplierToSet;
            m_targetPosition = -1;
            
            #ifdef NO_MOTOR_TESTING

                m_motorSpeedToSet = 0;
            #endif

            m_positionOffset = CENTER_POSITION_IN_CLICKS;
            m_position = 0;
        }

        /*
        Returns a value to the winch that has been passed through a regression.
        */
        //TODO fix relative positioning
        unsigned int GotoPosition(const int positionToGoto, const bool relative = false) {

            int deltaPosition;
            if (relative) {
            
                deltaPosition = GetPosition() + positionToGoto;
            }
            else {
                
                deltaPosition = GetPosition() - positionToGoto;
            }

            deltaPosition = (deltaPosition < -13 || deltaPosition > 13) ? ((deltaPosition < -13) ? -13 : 13) : deltaPosition;
            //Get speedToSet from a regression (negative to flip over x axis)
            int speedToSet = RoundLit(-((-0.0155218855 * pow(deltaPosition, 3)) + (4.329004329 * pow(10, -4) * (deltaPosition * deltaPosition)) + (12.47895623 * deltaPosition)) + 127.3722944);
            //If the speedToSet is outside of an unsigned char, make it be in-bounds
            speedToSet = (speedToSet > 255 || speedToSet < 0) ? ((speedToSet > 255) ? 255 : 0) : speedToSet;
            //If the speedToSet is within 1 of off, set the speed to 0
            speedToSet = (speedToSet > 130 && speedToSet < 134) ? 137 : speedToSet;
            //If the speedToSet is at the bounds, set it to the fastest or slowest
            //speedToSet = (speedToSet == 26);

            //std::cout << "Delta: " << deltaPosition << std::endl;
            //std::cout << "Pos: " << m_position << std::endl;

            #ifdef NO_MOTOR_TESTING
                
                m_motorSpeedToSet = speedToSet;
            #else

                //Return the finalized motor speed in a range of 2000 to 1000 based on 0 to 255
                //Make sure to reverse motor if needed
                return (unsigned char)speedToSet;
            #endif
        }

        /*
        Calls GotoPosition using target position member variable.
        Returns true if targetPosition is not set or if we made it to the target
        position.
        */
        unsigned char GotoTargetPosition(const bool relative = false) {
            
            if (m_targetPosition != -1) { return GotoPosition(m_targetPosition, relative); }
            else return 127;
        }

        bool ShouldSetANewTarget() {

            if (GotoTargetPosition() == 127) {

                m_targetPosition = -1;
                return true;
            }
            else return false;
        }

        unsigned int Jog(const bool in) {

            if (in) { return 0; }
            else { return 255; }
        }

        unsigned char Stop() { return 127; }

        void Zero() {

            m_positionOffset = CENTER_POSITION_IN_CLICKS - m_position;
        }

        void SetTargetPosition(const int targetPositionToSet) {

            m_targetPosition = targetPositionToSet;
        }

        int GetPosition() {

            return m_position + m_positionOffset;
        }

        int GetTargetPosition() {

            return m_targetPosition;
        }

        bool TargetPositionSet() {

            return m_targetPosition != -1;
        }

        #ifdef NO_MOTOR_TESTING

            int getMotorSpeedToSet() {

                return m_motorSpeedToSet;
            }
        #endif

        int m_position;
        int m_positionOffset;

    private:

        int m_reverseDirectionMultiplier;
        int m_targetPosition;
        
        #ifdef NO_MOTOR_TESTING

            double m_motorSpeedToSet;
        #endif
};