#pragma once

#define REVERSE -1

#include "Utils.h"
#include "Constants.h"
#include <math.h>
#include <iostream>

class Winch {

    public:

        Winch(const int reverseDirectionMultiplierToSet = 1) {

            m_reverseDirectionMultiplier = reverseDirectionMultiplierToSet;
            
            #ifdef NO_MOTOR_TESTING

                m_motorSpeedToSet = 0;
            #endif

            m_positionOffset = CENTER_POSITION_IN_CLICKS;
            m_position = 0;
            m_speedToSet = 0;
        }

        void Set(const double speed) {

            m_speedToSet = (speed < -1.0 || speed > 1.0) ? (speed < -1.0 ? -1.0 : 1.0) : speed;
        }

        void Stop() {

            Set(0);
        }

        void Zero() {

            m_positionOffset = CENTER_POSITION_IN_CLICKS - m_position;
        }

        void Jog(const bool in) {

            Set(in ? -1.0 : 1.0);
        }

        double GetSpeed() {

            return m_speedToSet;
        }

        int GetPosition() {

            return m_position + m_positionOffset;
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
        double m_speedToSet;

        #ifdef NO_MOTOR_TESTING

            double m_motorSpeedToSet;
        #endif
};